/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008 Tomotaka SUWA <t.suwa@mac.com>

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

#ifndef SKKInputEngine_h
#define SKKInputEngine_h

#include "SKKInputEnvironment.h"
#include "SKKInputMode.h"
#include "SKKInputQueue.h"
#include "SKKComposingEditor.h"
#include "SKKOkuriEditor.h"
#include "SKKCandidateEditor.h"
#include "SKKEntryRemoveEditor.h"
#include "SKKCompleter.h"
#include "SKKSelector.h"
#include <vector>

class SKKInputEngine : public SKKInputQueueObserver,
                       public SKKCompleterBuddy,
                       public SKKSelectorBuddy,
                       public SKKOkuriListener {
    class Synchronizer;
    friend class Synchronizer;

    SKKInputEnvironment* env_;
    SKKInputSessionParameter* param_;
    SKKInputContext* context_;
    SKKConfig* config_;
    std::vector<SKKBaseEditor*> stack_;

    SKKInputQueue inputQueue_;
    SKKInputQueueObserver::State inputState_;

    std::string word_;

    SKKComposingEditor composingEditor_;
    SKKOkuriEditor okuriEditor_;
    SKKCandidateEditor candidateEditor_;
    SKKEntryRemoveEditor entryRemoveEditor_;

    SKKInputEngine();
    SKKInputEngine(const SKKInputEngine&);
    SKKInputEngine& operator=(const SKKInputEngine&);

    SKKBaseEditor* top() const;
    SKKInputMode inputMode() const;
    void initialize();
    void push(SKKBaseEditor* editor);
    void invoke(SKKBaseEditor::Event event);
    void terminate();
    void study(const SKKEntry& entry, const SKKCandidate& candidate);
    void insert(const std::string& str);

    // ローマ字かな変換通知
    virtual void SKKInputQueueUpdate(const SKKInputQueueObserver::State& state);

    // 見出し語の取得
    virtual const std::string SKKCompleterQueryString();

    // 現在の見出し語の通知
    virtual void SKKCompleterUpdate(const std::string& entry);

    // SKKSelector::Execute() 時に呼び出される
    virtual const SKKEntry SKKSelectorQueryEntry();

    // SKKSelector で現在選択中の候補が変更された場合に呼び出される
    virtual void SKKSelectorUpdate(const SKKCandidate& candidate);

    // 送り入力中に見出し語部分が増えた場合に呼び出される
    virtual void SKKOkuriListenerAppendEntry(const std::string& fixed);

public:
    SKKInputEngine(SKKInputEnvironment* env);

    // 入力モード
    void SelectInputMode(SKKInputMode mode);

    // 状態変更
    void SetStatePrimary();
    void SetStateComposing();
    void SetStateOkuri();
    void SetStateSelectCandidate();
    void SetStateEntryRemove();
    void SetStateRegistration();

    // 入力
    void HandleChar(char code, bool direct);
    void HandleBackSpace();
    void HandleDelete();
    void HandleCursorLeft();
    void HandleCursorRight();
    void HandleCursorUp();
    void HandleCursorDown();
    void HandlePaste();
    void HandlePing();
    void HandleEnter();
    void HandleCancel();

    // 確定
    void Commit();

    // リセット
    void Reset();

    // トグル変換
    void ToggleKana();
    void ToggleJisx0201Kana();

    // 同期
    void UpdateInputContext();

    // ローマ字かな変換が発生するか？
    bool CanConvert(char code) const;

    // 送りが完成したか？
    bool IsOkuriComplete() const;
};

#endif
