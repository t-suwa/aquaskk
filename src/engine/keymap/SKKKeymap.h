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

#ifndef SKKKeymap_h
#define SKKKeymap_h

#include <map>
#include <string>
#include "SKKEvent.h"

// キーマップ
class SKKKeymap {
    typedef std::map<int, int> Keymap;

    Keymap events_;
    Keymap attributes_;
    Keymap option_;

    Keymap::iterator find(int charcode, int keycode, int mods, Keymap& keymap);
    void load(const std::string& path_to_config, bool initialize);

public:
    // 初期化
    void Initialize(const std::string& path_to_config);

    // 追加の読み込み
    void Patch(const std::string& path_to_config);

    // 検索
    SKKEvent Fetch(int charcode, int keycode, int mods);
};

#endif
