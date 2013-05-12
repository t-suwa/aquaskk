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

#include "SKKState.h"
#include "SKKMessenger.h"
#include "SKKConfig.h"
#include "SKKInputEngine.h"
#include <cctype>

namespace {
    typedef SKKState::Event Event;
    typedef SKKState::State State;
}

SKKState::SKKState(SKKInputEnvironment* env, SKKInputEngine* editor)
    : context_(env->InputContext())
    , messenger_(env->InputSessionParameter()->Messenger())
    , window_(env->InputSessionParameter()->CandidateWindow())
    , configuration_(env->InputSessionParameter()->Config())
    , editor_(editor)
    , completer_(editor_)
    , selector_(editor_, window_)
{}

SKKState::SKKState(const SKKState& src)
    : context_(src.context_)
    , messenger_(src.messenger_)
    , window_(src.window_)
    , configuration_(src.configuration_)
    , editor_(src.editor_)
    , completer_(editor_)
    , selector_(editor_, window_)
{}

void SKKState::ToString(const Handler handler, const Event& event, std::string& result) {
    static const char* systemEvent[] = { "PROBE", "<<ENTRY>>", "<<INIT>>", "<<EXIT>>" };

    static struct {
	Handler handler;
	const char* name;
    } states[] = {
#define DEFINE_State(arg)	{ &SKKState::arg, "SKKState::" #arg }
	DEFINE_State(Primary),
	DEFINE_State(KanaInput),
	DEFINE_State(Hirakana),
	DEFINE_State(Katakana),
	DEFINE_State(Jisx0201Kana),
	DEFINE_State(LatinInput),
	DEFINE_State(Ascii),
	DEFINE_State(Jisx0208Latin),
	DEFINE_State(Composing),
	DEFINE_State(Edit),
	DEFINE_State(EntryInput),
	DEFINE_State(KanaEntry),
	DEFINE_State(OkuriInput),
	DEFINE_State(AsciiEntry),
	DEFINE_State(EntryCompletion),
	DEFINE_State(SelectCandidate),
	DEFINE_State(EntryRemove),
	DEFINE_State(RecursiveRegister),
#undef DEFINE_State
	{ 0, 0x00 }
    };

    result.clear();

    for(int i = 0; states[i].handler != 0; ++ i) {
	if(handler == states[i].handler) {
	    result += states[i].name;
	    result += ", ";
	    if(event.IsSystem()) {
		result += systemEvent[abs(event)];
	    } else {
		result += event.Param().dump();
	    }
	    return;
	}
    }
}

#include "SKKStatePrimary-inl.h"
#include "SKKStateComposing-inl.h"
#include "SKKStateRecursiveRegister-inl.h"
#include "SKKStateEntryRemove-inl.h"
