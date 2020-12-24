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

#ifndef SKKKeymapEntry_h
#define SKKKeymapEntry_h

#include <vector>
#include <string>

// キーマップパーサー
class SKKKeymapEntry {
    int type_;
    int symbol_;
    int label_;
    int mods_;
    bool not_;

    typedef std::pair<int, int> KeyRange;
    std::vector<KeyRange> keys_;
    unsigned pos_;

    enum {
	LABEL_GROUP	= (1 << 1),
	LABEL_HEX	= (1 << 2),
	LABEL_KEYCODE	= (1 << 3)
    };

    std::string setup(const std::string& str);
    void parseGroup(const std::string& str);
    void parseRange(const std::string& str);
    void parseEntry(const std::string& str);
    int makeKey(const std::string& str);

public:
    SKKKeymapEntry();
    SKKKeymapEntry(const std::string& configKey, const std::string& configValue);

    // キーの読み出し
    bool operator>>(int& key);

    // エントリーがイベントかどうか
    bool IsEvent() const;

    // エントリーが SKK_CHAR 属性かどうか
    bool IsAttribute() const;

    // エントリーが 属性リセット用かどうか
    bool IsNot() const;

    // シンボルの取得
    int Symbol() const;
};

#endif
