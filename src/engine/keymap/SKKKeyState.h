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

#ifndef SKKKeyState_h
#define SKKKeyState_h

// 以下のようなキー状態を作り出すユーティリティクラス
//
//  31       23       15       7      0 bit
// +--------+--------+--------+--------+
// + 未使用 |modifier|key code| ascii  |
// +--------+--------+--------+--------+
//
// modifier は shift, ctrl, alt(=opt), meta(=cmd)
//
class SKKKeyState {
    int state_;

    SKKKeyState();
    SKKKeyState(int charcode, int keycode, short mods) {
	state_ = (mods << 16) | ((0xff & keycode) << 8) | (0xff & charcode);
    }

public:
    enum Modifier {
	SHIFT	= (1 << 1),
	CTRL	= (1 << 2),
	ALT	= (1 << 3),
	META	= (1 << 4)
    };

    static SKKKeyState KeyCode(int code, int mods) {
	return SKKKeyState(0, code, mods);
    }

    static SKKKeyState CharCode(int code, int mods) {
	return SKKKeyState(code, 0, mods);
    }

    operator int() const {
	return state_;
    }
};

#endif
