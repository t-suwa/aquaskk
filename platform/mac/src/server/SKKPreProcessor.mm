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

#include <cassert>
#include <iostream>
#include <cctype>
#include <fstream>
#include "SKKPreProcessor.h"
#include "SKKKeyState.h"

SKKPreProcessor::SKKPreProcessor() {}

SKKPreProcessor& SKKPreProcessor::theInstance() {
    static SKKPreProcessor obj;
    return obj;
}

void SKKPreProcessor::Initialize(const std::string& path) {
    keymap_.Initialize(path);
}

void SKKPreProcessor::Patch(const std::string& path) {
    keymap_.Patch(path);
}

SKKEvent SKKPreProcessor::Execute(const NSEvent* event) {
    NSString* diststr = [event characters];
    int dispchar = diststr ? *[diststr UTF8String] : 0;
    NSString* charstr = [event charactersIgnoringModifiers];
    int charcode = charstr ? *[charstr UTF8String] : 0;
    int keycode = [event keyCode];
    int mods = 0;

    // シフト属性が有効なのはデッドキーのみ
    if([event modifierFlags] & NSShiftKeyMask) {
	if(std::isgraph(dispchar)) { // 空白類を除いた英数字記号
	    charcode = dispchar;
	} else {
	    mods += SKKKeyState::SHIFT;
	}
    }

    if([event modifierFlags] & NSControlKeyMask) {
	mods += SKKKeyState::CTRL;
    }

    if([event modifierFlags] & NSAlternateKeyMask) {
	mods += SKKKeyState::ALT;
    }

    if([event modifierFlags] & NSCommandKeyMask) {
	mods += SKKKeyState::META;
    }

    // 英数キー、かなキーの文字コードがスペースのため、0 にする
    if(keycode == 0x66 || keycode == 0x68) {
        charcode = 0x00;
    }

    SKKEvent result = keymap_.Fetch(charcode, keycode, mods);

    if([event modifierFlags] & NSAlphaShiftKeyMask) {
        result.option |= CapsLock;
    }

#ifdef SKK_DEBUG
    NSLog(@"%@", [event description]);
    NSLog(@"%s", result.dump().c_str());
#endif

    return result;
}
