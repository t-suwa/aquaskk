/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008-2009 Tomotaka SUWA <t.suwa@mac.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "SKKInputEngine.h"
#include "SKKInputContext.h"
#include "SKKConfig.h"
#include "SKKClipboard.h"
#include "SKKBackEnd.h"
#include <iostream>
#include <cctype>

// ----------------------------------------------------------------------

// RAII による SKKInputContext の同期管理
//
// SKKInputEngine::SetState* メソッドで使用する。
class SKKInputEngine::Synchronizer {
    SKKInputEngine* engine_;

public:
    Synchronizer(SKKInputEngine* engine) : engine_(engine) {
        // 直近の状態を SKKInputContext に反映する
        engine_->UpdateInputContext();

        // 初期化
        engine_->initialize();
    }

    ~Synchronizer() {
        // Top エディタを初期化する
        engine_->top()->ReadContext();

        // 最新の状態を SKKInputContext に反映する
        engine_->UpdateInputContext();
    }
};

// ----------------------------------------------------------------------

SKKInputEngine::SKKInputEngine(SKKInputEnvironment* env)
    : env_(env)
    , param_(env->InputSessionParameter())
    , context_(env->InputContext())
    , config_(env->Config())
    , inputQueue_(this)
    , composingEditor_(env->InputContext())
    , okuriEditor_(env->InputContext(), this)
    , candidateEditor_(env->InputContext())
    , entryRemoveEditor_(env->InputContext()) {
    SetStatePrimary();
}

void SKKInputEngine::SelectInputMode(SKKInputMode mode) {
    env_->InputModeSelector()->Select(mode);
    inputQueue_.SelectInputMode(mode);

    context_->event_handled = true;
}

void SKKInputEngine::SetStatePrimary() {
    Synchronizer sync(this);
}

void SKKInputEngine::SetStateComposing() {
    Synchronizer sync(this);

    push(&composingEditor_);

    SKKEntry& entry = context_->entry;

    if(!config_->DeleteOkuriWhenQuit()) {
        entry.AppendEntry(entry.OkuriString());
    }
}

void SKKInputEngine::SetStateOkuri() {
    Synchronizer sync(this);

    push(&composingEditor_);
    push(&okuriEditor_);
}

void SKKInputEngine::SetStateSelectCandidate() {
    Synchronizer sync(this);

    push(&candidateEditor_);
}

void SKKInputEngine::SetStateEntryRemove() {
    Synchronizer sync(this);

    push(&entryRemoveEditor_);
}

void SKKInputEngine::SetStateRegistration() {
    UpdateInputContext();

    context_->registration.Start();
}

void SKKInputEngine::HandleChar(char code, bool direct) {
    inputQueue_.AddChar(code, direct);
}

void SKKInputEngine::HandleBackSpace() {
    if(inputQueue_.IsEmpty()) {
        invoke(SKKBaseEditor::BackSpace);
    } else {
        inputQueue_.RemoveChar();
    }
}

void SKKInputEngine::HandleDelete() {
    invoke(SKKBaseEditor::Delete);
}

void SKKInputEngine::HandleCursorLeft() {
    invoke(SKKBaseEditor::CursorLeft);
}

void SKKInputEngine::HandleCursorRight() {
    invoke(SKKBaseEditor::CursorRight);
}

void SKKInputEngine::HandleCursorUp() {
    invoke(SKKBaseEditor::CursorUp);
}

void SKKInputEngine::HandleCursorDown() {
    invoke(SKKBaseEditor::CursorDown);
}

void SKKInputEngine::HandlePaste() {
    top()->Input(param_->Clipboard()->PasteString());
}

void SKKInputEngine::HandlePing() {
    env_->InputModeSelector()->Show();
}

void SKKInputEngine::HandleEnter() {
    Commit();

    study(context_->entry, SKKCandidate(word_, false));

    if(word_.empty()) {
        context_->registration.Abort();
    } else {
        std::string output = word_ + context_->entry.OkuriString();

        context_->registration.Finish(output);
    }

    context_->event_handled = false;
}

void SKKInputEngine::HandleCancel() {
    if(!inputQueue_.IsEmpty()) {
        terminate();
        return;
    }

    context_->registration.Abort();
    context_->event_handled = false;
}

void SKKInputEngine::Commit() {
    terminate();

    word_.clear();

    // Top のフィルターから Commit していき、最終的な単語を取得する
    std::vector<SKKBaseEditor*>::reverse_iterator iter;
    for(iter = stack_.rbegin(); iter != stack_.rend(); ++ iter) {
        (*iter)->Commit(word_);
    }
}

void SKKInputEngine::Reset() {
    terminate();

    context_->event_handled = false;
}

void SKKInputEngine::ToggleKana() {
    terminate();

    SKKEntry& entry = context_->entry;

    study(entry, SKKCandidate());

    insert(entry.ToggleKana(inputMode()));
}

void SKKInputEngine::ToggleJisx0201Kana() {
    terminate();

    SKKEntry& entry = context_->entry;
    
    study(entry, SKKCandidate());

    insert(entry.ToggleJisx0201Kana(inputMode()));
}

void SKKInputEngine::UpdateInputContext() {
    context_->output.Clear();

    std::for_each(stack_.begin(), stack_.end(),
                  std::mem_fun(&SKKBaseEditor::WriteContext));

    // 非確定文字があれば挿入(ex. "ky" など)
    if(config_->DisplayShortestMatchOfKanaConversions()
       && !inputState_.intermediate.empty()) {
        context_->output.Compose(inputState_.intermediate);
    } else {
        context_->output.Compose(inputState_.queue);
    }

    env_->InputModeSelector()->Notify();
}

bool SKKInputEngine::CanConvert(char code) const {
    return inputQueue_.CanConvert(code);
}

bool SKKInputEngine::IsOkuriComplete() const {
    return okuriEditor_.IsOkuriComplete();
}

// ----------------------------------------------------------------------

SKKBaseEditor* SKKInputEngine::top() const {
    return stack_.back();
}

SKKInputMode SKKInputEngine::inputMode() const {
    return *(env_->InputModeSelector());
}

void SKKInputEngine::initialize() {
    stack_.clear();
    stack_.push_back(env_->BaseEditor());

    context_->dynamic_completion = false;
    context_->annotation = false;

    if(context_->registration == SKKRegistration::Aborted) {
        context_->registration.Clear();
        env_->InputModeSelector()->Refresh();
    }
}

void SKKInputEngine::push(SKKBaseEditor* editor) {
    stack_.push_back(editor);
}

void SKKInputEngine::terminate() {
    // ローマ字かな変換を打ち切る
    if(config_->FixIntermediateConversion()) {
        inputQueue_.Terminate();
    } else {
        inputQueue_.Clear();
    }
}

void SKKInputEngine::invoke(SKKBaseEditor::Event event) {
    if(!inputQueue_.IsEmpty()) {
        inputQueue_.Clear();
        context_->event_handled = false;
    } else {
        top()->Input(event);
    }
}

void SKKInputEngine::study(const SKKEntry& entry, const SKKCandidate& candidate) {
    if(entry.IsEmpty()) return;
    if(entry.IsOkuriAri() && entry.OkuriString().empty()) return;
    if(entry.IsOkuriAri() && candidate.IsEmpty()) return;

    SKKBackEnd::theInstance().Register(entry, candidate);
}

void SKKInputEngine::insert(const std::string& str) {
    stack_.front()->Input(str, "", 0);
}

// ----------------------------------------------------------------------

void SKKInputEngine::SKKInputQueueUpdate(const SKKInputQueueObserver::State& state) {
    inputState_ = state;

    if(inputMode() == AsciiInputMode) {
        top()->Input(state.fixed);
    } else {
        top()->Input(state.fixed, state.queue, state.code);
    }
}

const std::string SKKInputEngine::SKKCompleterQueryString() {
    SKKEntry entry = SKKSelectorQueryEntry();

    return entry.EntryString();
}

void SKKInputEngine::SKKCompleterUpdate(const std::string& entry) {
    composingEditor_.SetEntry(entry);
}

const SKKEntry SKKInputEngine::SKKSelectorQueryEntry() {
    terminate();

    SKKEntry entry = context_->entry.Normalize(inputMode());

    context_->entry = entry;

    return entry;
}

void SKKInputEngine::SKKSelectorUpdate(const SKKCandidate& candidate) {
    candidateEditor_.SetCandidate(candidate);
}

void SKKInputEngine::SKKOkuriListenerAppendEntry(const std::string& fixed) {
    composingEditor_.Input(fixed, "", 0);
}
