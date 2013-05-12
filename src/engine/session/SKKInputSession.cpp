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

#include "SKKInputSession.h"
#include "SKKRecursiveEditor.h"
#include "SKKFrontEnd.h"
#include "SKKPrimaryEditor.h"
#include "SKKRegisterEditor.h"
#include "SKKBackEnd.h"

namespace {
    class scoped_flag {
        bool& flag_;

    public:
        scoped_flag(bool& flag) : flag_(flag) {
            flag_ = true;
        }

        ~scoped_flag() {
            flag_ = false;
        }
    };
}

SKKInputSession::SKKInputSession(SKKInputSessionParameter* param)
    : param_(param)
    , context_(param->FrontEnd())
    , inEvent_(false) {
    stack_.push_back(createEditor(new SKKPrimaryEditor(&context_)));
}

SKKInputSession::~SKKInputSession() {
    while(!stack_.empty()) {
        popEditor();
    }

    while(!listeners_.empty()) {
        delete listeners_.back();
        listeners_.pop_back();
    }
}

void SKKInputSession::AddInputModeListener(SKKInputModeListener* listener) {
    listeners_.push_back(listener);
}

bool SKKInputSession::HandleEvent(const SKKEvent& event) {
    if(inEvent_) return false;

    scoped_flag on(inEvent_);

    beginEvent();

    top()->Input(event);

    endEvent();

    top()->Output();

    return result(event);
}

void SKKInputSession::Commit() {
    HandleEvent(SKKEvent(SKK_ENTER, 0));
    
    if(context_.output.IsComposing()) {
        Clear();
    }
}

void SKKInputSession::Clear() {
    if(inEvent_) return;

    scoped_flag on(inEvent_);

    while(stack_.size()) {
        popEditor();
    }

    stack_.push_back(createEditor(new SKKPrimaryEditor(&context_)));

    top()->Output();
}

void SKKInputSession::Activate() {
    top()->Activate();
}

void SKKInputSession::Deactivate() {
    top()->Deactivate();
}

bool SKKInputSession::IsChildOf(SKKStateMachine::Handler handler) {
    return top()->IsChildOf(handler);
}

// ----------------------------------------------------------------------

void SKKInputSession::beginEvent() {
    context_.event_handled = true;
    context_.needs_setback = false;
}

void SKKInputSession::endEvent() {
    switch(context_.registration) {
    case SKKRegistration::Started:
        context_.registration.Clear();
        stack_.push_back(createEditor(new SKKRegisterEditor(&context_)));
        break;

    case SKKRegistration::Finished:
    case SKKRegistration::Aborted:
        if(stack_.size() != 1) {
            popEditor();

            top()->Input(SKKEvent(context_.registration == SKKRegistration::Finished
                                  ? SKK_ENTER : SKK_CANCEL, 0));
        }
        break;

    default:
        break;
    }
}

bool SKKInputSession::result(const SKKEvent& event) {
    // 単語登録中か、未確定状態なら常に処理済み
    if(stack_.size() != 1 || context_.output.IsComposing()) {
        return true;
    }

    switch(event.option) {
    case AlwaysHandled:     // 常に処理済み
        return true;

    case PseudoHandled:     // 未処理
        return false;

    default:
        return context_.event_handled;
    }
}

SKKRecursiveEditor* SKKInputSession::top() {
    return stack_.back();
}

SKKRecursiveEditor* SKKInputSession::createEditor(SKKBaseEditor* bottom) {
    return new SKKRecursiveEditor(
        new SKKInputEnvironment(&context_, param_.get(), &listeners_, bottom));
}

void SKKInputSession::popEditor() {
    delete top();
    stack_.pop_back();
}
