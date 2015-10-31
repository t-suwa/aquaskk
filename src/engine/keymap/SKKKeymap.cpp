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

#include <fstream>
#include "SKKKeyState.h"
#include "SKKKeymap.h"
#include "SKKKeymapEntry.h"

void SKKKeymap::Initialize(const std::string& path) {
    load(path, true);
}

void SKKKeymap::Patch(const std::string& path) {
    load(path, false);
}

SKKEvent SKKKeymap::Fetch(int charcode, int keycode, int mods) {
    SKKEvent event;
    Keymap::iterator iter;

    // 文字コード
    event.code = charcode;

    iter = find(charcode, keycode, mods, events_);
    if(iter != events_.end()) {
	event.id = iter->second;
    } else {
        event.id = SKK_CHAR;
    }

    // SKK_CHAR イベントなら属性も調べる
    if(event.id == SKK_CHAR) {
	iter = find(charcode, keycode, mods, attributes_);
	if(iter != attributes_.end()) {
            event.attribute = iter->second;
        }
    }

    // 処理オプションを検索する
    iter = find(charcode, keycode, mods, option_);
    if(iter != option_.end()) {
        event.option = iter->second;
    }

    return event;
}

void SKKKeymap::load(const std::string& path, bool initialize) {
    std::ifstream config(path.c_str());

    if(!config) return;

    // 初期化
    if(initialize) {
        events_.clear();
        attributes_.clear();
        option_.clear();
    }

    std::string configKey;
    std::string configValue;
    while(config >> configKey >> configValue) {
        // コメントは無視
	if(!configKey.empty() && configKey[0] != '#') {
	    SKKKeymapEntry entry(configKey, configValue);

	    // キー情報を読み取る
	    int key;
	    while(entry >> key) {
                // 明示的なイベント
                if(entry.IsEvent()) {
                    events_[key] = entry.Symbol();
                    continue;
                }

                // SKK_CHAR 属性
                if(entry.IsAttribute()) {
                    // NotInputChars q とあった場合、qからInputChars属性をはずす
                    if(entry.IsNot()) {
                        attributes_[key] &= ~entry.Symbol();
                    } else {
                        events_[key] = SKK_CHAR;
                        attributes_[key] |= entry.Symbol();
                    }
                    continue;
                }

                // 処理オプション
                option_[key] = entry.Symbol();
            }
        }
        // 行末まで読み飛ばす
        config.ignore(0xff, '\n');
    }
}

SKKKeymap::Keymap::iterator SKKKeymap::find(int charcode, int keycode, int mods, Keymap& keymap) {
    Keymap::iterator iter;

    // まずキーコードで調べる(優先度高)
    iter = keymap.find(SKKKeyState::KeyCode(keycode, mods));
    if(iter != keymap.end()) {
	return iter;
    }

    // キャラクターコードを調べる
    iter = keymap.find(SKKKeyState::CharCode(charcode, mods));
    if(iter != keymap.end()) {
	return iter;
    }

    return keymap.end();
 }
