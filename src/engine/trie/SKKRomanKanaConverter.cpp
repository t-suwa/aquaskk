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

#include "jconv.h"
#include "SKKRomanKanaConverter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// ======================================================================
// ユーティリティ
// ======================================================================
namespace {
    void unescape_string(std::string& str) {
        static struct {
            std::string from;
            const char* to;
        } escape[] = {
            { "&comma;", "," },
            { "&space;", " " },
            { "&sharp;", "#" },
            { "",      0x00 },
        };

        for(int i = 0; escape[i].to != 0x00; ++ i) {
            std::string& target = escape[i].from;
            std::string::size_type pos;
            for(pos = str.find(target); pos != std::string::npos; pos = str.find(target, pos)) {
                str.replace(pos, target.length(), escape[i].to);
            }
        }
    }

    void escape_string(std::string& str) {
        const std::string target(" ");
        std::string::size_type pos;

        for(pos = str.find(target); pos != std::string::npos; pos = str.find(target, pos)) {
            str.replace(pos, target.length(), "&space;");
        }
    }

    // 変換ヘルパー
    class ConversionHelper : public SKKTrieHelper {
        bool converted_;
        bool short_;
        SKKInputMode mode_;
        std::string queue_;
        std::string output_;
        std::string next_;
        std::string intermediate_;

        virtual const std::string& SKKTrieRomanString() const {
            return queue_;
        }

        virtual void SKKTrieNotifyConverted(const SKKTrie* node) {
            converted_ = true;
            output_ += node->KanaString(mode_);
            next_ = node->NextState();
            intermediate_.clear();
        }

        virtual void SKKTrieNotifyIntermediate(const SKKTrie* node) {
            intermediate_ = node->KanaString(mode_);
        }

        virtual void SKKTrieNotifyNotConverted(char code) {
            converted_ = false;
            output_ += code;
        }

        virtual void SKKTrieNotifySkipLength(int length) {
            queue_ = queue_.substr(length);
        }

        virtual void SKKTrieNotifyShort() {
            short_ = true;
        }

    public:
        ConversionHelper(SKKInputMode mode, const std::string& queue)
            : converted_(false), short_(false), mode_(mode), queue_(queue) {}

        const std::string& Output() const {
            return output_;
        }

        const std::string& Next() const {
            return next_;
        }

        const std::string& Intermediate() const {
            return intermediate_;
        }

        const std::string& Queue() const {
            return queue_;
        }

        bool IsConverted() const {
            return converted_;
        }

        bool IsShort() const {
            return short_;
        }
    };
}

// ======================================================================
// SKKRomanKanaConverter インタフェース
// ======================================================================
SKKRomanKanaConverter::SKKRomanKanaConverter() {}

SKKRomanKanaConverter& SKKRomanKanaConverter::theInstance() {
    static SKKRomanKanaConverter obj;
    return obj;
}

void SKKRomanKanaConverter::Initialize(const std::string& path) {
    load(path, true);
}

void SKKRomanKanaConverter::Patch(const std::string& path) {
    load(path, false);
}

bool SKKRomanKanaConverter::Convert(SKKInputMode mode, const std::string& str, SKKRomanKanaConversionResult& result) {
    bool converted = false;
    std::string queue(str);

    result = SKKRomanKanaConversionResult();

    while(!queue.empty()) {
        ConversionHelper helper(mode, queue);

        root_.Traverse(helper);

        result.output += helper.Output();
        result.next = helper.Next();
        result.intermediate = helper.Intermediate();

        converted = helper.IsConverted();

        if(helper.IsShort()) {
            result.next = helper.Queue();
            break;
        }

        queue = helper.Queue();
    }

    return converted;
}

// ----------------------------------------------------------------------

void SKKRomanKanaConverter::load(const std::string& path, bool initialize) {
    std::ifstream ifs(path.c_str());
    std::string str;

    if(!ifs) {
	std::cerr << "SKKRomanKanaConverter::load(): can't open file [" << path << "]" << std::endl;
	return;
    }

    if(initialize) {
        root_.Clear();
    }

    while(std::getline(ifs, str)) {
	if(str.empty() || str[0] == '#') continue;

	std::string utf8;
	jconv::convert_eucj_to_utf8(str, utf8);

	// 全ての ',' を空白に置換して分解する(事前に空白をエスケープする)
        escape_string(utf8);
	std::replace(utf8.begin(), utf8.end(), ',', ' ');
	std::istringstream buf(utf8);

	// 変換ルールを読む
	std::string roman, hirakana, katakana, jisx0201kana, next;
	if(buf >> roman >> hirakana >> katakana >> jisx0201kana) {
	    // オプションの次状態も読む
	    buf >> next;

	    // エスケープ文字を元に戻す
	    unescape_string(roman);
	    unescape_string(hirakana);
	    unescape_string(katakana);
	    unescape_string(jisx0201kana);
	    unescape_string(next);

	    // ルール木に追加
	    root_.Add(roman, SKKTrie(hirakana, katakana, jisx0201kana, next));
	} else {
	    // 不正な形式
	    std::cerr << "SKKRomanKanaConverter::load(): invalid rule [" << utf8 << "]" << std::endl;
	}
    }
}
