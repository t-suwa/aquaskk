/* -*- C++ -*-

   MacOS X implementation of the SKK input method.

   Copyright (C) 2007 Tomotaka SUWA <t.suwa@mac.com>

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

#ifndef SKKState_h
#define SKKState_h

#include "GenericStateMachine.h"
#include "SKKInputEnvironment.h"
#include "SKKInputMode.h"
#include "SKKEvent.h"
#include "SKKCompleter.h"
#include "SKKSelector.h"

using namespace statemachinecxx_sourceforge_jp;

class SKKMessenger;
class SKKCandidateWindow;
class SKKConfig;
class SKKInputEngine;

// 状態コンテナ
class SKKState : public BaseStateContainer<SKKState, SKKEvent> {
    SKKInputContext* context_;
    SKKMessenger* messenger_;
    SKKCandidateWindow* window_;
    SKKConfig* configuration_;
    SKKInputEngine* editor_;
    SKKCompleter completer_;
    SKKSelector selector_;

public:
    SKKState(SKKInputEnvironment* env, SKKInputEngine* editor);
    SKKState(const SKKState& src);

    virtual const Handler InitialState() const { return &SKKState::Primary; }

    // level 1
    State Primary(const Event& event);

        // level 2 (initial state)
        State KanaInput(const Event& event);

	    // level 3 (sub of KanaInput)
    	    State Hirakana(const Event& event);
    	    State Katakana(const Event& event);
    	    State Jisx0201Kana(const Event& event);

        // level 2 (sub of Direct)
        State LatinInput(const Event& event);

    	    // level 3 (sub of LatinInput)
    	    State Ascii(const Event& event);
    	    State Jisx0208Latin(const Event& event);

    // level 1
    State Composing(const Event& event);

	// level 2 (sub of Composing)
	State Edit(const Event& event);

	    // level 3 (sub of Edit)
	    State EntryInput(const Event& event);

		// lelvel 4 (sub of EntryInput)
		State KanaEntry(const Event& event);
		State AsciiEntry(const Event& event);

	    // level 3 (sub of Edit)
	    State EntryCompletion(const Event& event);

	// level 2 (sub of Composing)
	State SelectCandidate(const Event& event);

    // level 1
    State OkuriInput(const Event& event);

    // level 1
    State EntryRemove(const Event& event);

    // level 1
    State RecursiveRegister(const Event& event);

    static void ToString(const Handler handler, const Event& event, std::string& result);
};

#endif
