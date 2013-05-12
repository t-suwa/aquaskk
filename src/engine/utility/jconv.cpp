/*
 * jconv.cpp - japanese character code conversion library
 *
 *   Copyright (c) 2006 Tomotaka SUWA, All rights reserved.
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 * 
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "jconv.h"
#include <algorithm>

namespace jconv {
#include "jconv_ucs2eucj-inl.h"
#include "jconv_eucj2ucs-inl.h"
#include "jconv_body-inl.h"

    void convert_utf8_to_eucj(const std::string& from, std::string& to) {
	std::for_each(from.begin(), from.end(), utf8_to_eucj(to));
    }

    void convert_eucj_to_utf8(const std::string& from, std::string& to) {
	std::for_each(from.begin(), from.end(), eucj_to_utf8(to));
    }

    std::string utf8_from_eucj(const std::string& eucj) {
        std::string utf8;

        convert_eucj_to_utf8(eucj, utf8);

        return utf8;
    }

    std::string eucj_from_utf8(const std::string& utf8) {
        std::string eucj;

        convert_utf8_to_eucj(utf8, eucj);

        return eucj;
    }

    struct kana {
	const char* hirakana;
	const char* katakana;
	const char* jisx0201_kana;
    };

    // data member pointer
    typedef const char* kana::*kana_member;

    static kana primary_kana_table[] = {
	{ "が", "ガ", "ｶﾞ" }, { "ぎ", "ギ", "ｷﾞ" }, { "ぐ", "グ", "ｸﾞ" }, { "げ", "ゲ", "ｹﾞ" }, { "ご", "ゴ", "ｺﾞ" },
	{ "ざ", "ザ", "ｻﾞ" }, { "じ", "ジ", "ｼﾞ" }, { "ず", "ズ", "ｽﾞ" }, { "ぜ", "ゼ", "ｾﾞ" }, { "ぞ", "ゾ", "ｿﾞ" },
	{ "だ", "ダ", "ﾀﾞ" }, { "ぢ", "ヂ", "ﾁﾞ" }, { "づ", "ヅ", "ﾂﾞ" }, { "で", "デ", "ﾃﾞ" }, { "ど", "ド", "ﾄﾞ" },
	{ "ば", "バ", "ﾊﾞ" }, { "び", "ビ", "ﾋﾞ" }, { "ぶ", "ブ", "ﾌﾞ" }, { "べ", "ベ", "ﾍﾞ" }, { "ぼ", "ボ", "ﾎﾞ" },
	{ "ぱ", "パ", "ﾊﾟ" }, { "ぴ", "ピ", "ﾋﾟ" }, { "ぷ", "プ", "ﾌﾟ" }, { "ぺ", "ペ", "ﾍﾟ" }, { "ぽ", "ポ", "ﾎﾟ" },
	{ "う゛", "ヴ", "ｳﾞ" }, { 0, 0, 0 }
    };

    static kana secondary_kana_table[] = {
	{ "あ", "ア", "ｱ" }, { "い", "イ", "ｲ" }, { "う", "ウ", "ｳ" }, { "え", "エ", "ｴ" }, { "お", "オ", "ｵ" },
	{ "か", "カ", "ｶ" }, { "き", "キ", "ｷ" }, { "く", "ク", "ｸ" }, { "け", "ケ", "ｹ" }, { "こ", "コ", "ｺ" },
	{ "さ", "サ", "ｻ" }, { "し", "シ", "ｼ" }, { "す", "ス", "ｽ" }, { "せ", "セ", "ｾ" }, { "そ", "ソ", "ｿ" },
	{ "た", "タ", "ﾀ" }, { "ち", "チ", "ﾁ" }, { "つ", "ツ", "ﾂ" }, { "て", "テ", "ﾃ" }, { "と", "ト", "ﾄ" },
	{ "な", "ナ", "ﾅ" }, { "に", "ニ", "ﾆ" }, { "ぬ", "ヌ", "ﾇ" }, { "ね", "ネ", "ﾈ" }, { "の", "ノ", "ﾉ" },
	{ "は", "ハ", "ﾊ" }, { "ひ", "ヒ", "ﾋ" }, { "ふ", "フ", "ﾌ" }, { "へ", "ヘ", "ﾍ" }, { "ほ", "ホ", "ﾎ" },
	{ "ま", "マ", "ﾏ" }, { "み", "ミ", "ﾐ" }, { "む", "ム", "ﾑ" }, { "め", "メ", "ﾒ" }, { "も", "モ", "ﾓ" },
	{ "や", "ヤ", "ﾔ" }, { "ゐ", "ヰ", "ｲ" }, { "ゆ", "ユ", "ﾕ" }, { "ゑ", "ヱ", "ｴ" }, { "よ", "ヨ", "ﾖ" },
	{ "ら", "ラ", "ﾗ" }, { "り", "リ", "ﾘ" }, { "る", "ル", "ﾙ" }, { "れ", "レ", "ﾚ" }, { "ろ", "ロ", "ﾛ" },
	{ "わ", "ワ", "ﾜ" }, { "を", "ヲ", "ｦ" }, { "ん", "ン", "ﾝ" },
	{ "ぁ", "ァ", "ｧ" }, { "ぃ", "ィ", "ｨ" }, { "ぅ", "ゥ", "ｩ" }, { "ぇ", "ェ", "ｪ" }, { "ぉ", "ォ", "ｫ" },
	{ "っ", "ッ", "ｯ" }, { "ゃ", "ャ", "ｬ" }, { "ゅ", "ュ", "ｭ" }, { "ょ", "ョ", "ｮ" }, { "　", "　", " " },
	{ "。", "。", "｡" }, { "、", "、", "､" }, { "ー", "ー", "ｰ" }, { "「", "「", "｢" }, { "」", "」", "｣" },
	{ "゛", "゛", "ﾞ" }, { "゜", "゜", "ﾟ" }, { "ゎ", "ヮ", "ﾜ" }, { 0, 0, 0 }
    };

    static kana* kana_tables[] = { primary_kana_table, secondary_kana_table, 0 };

    class translate {
	std::string from_;
	std::string to_;

    public:
	translate(const char* from, const char* to) : from_(from), to_(to) {}

	unsigned operator()(std::string& str, unsigned remain, unsigned offset = 0) {
	    if(!remain) return 0;

	    std::string::size_type pos = str.find(from_, offset);
	    if(pos == std::string::npos) return remain;

	    str.replace(pos, from_.size(), to_);

	    return this->operator()(str, remain - from_.size(), pos + to_.size());
	}
    };

    static void kana_convert(kana_member target, kana_member replacement, std::string& str) {
	unsigned remain = str.size();

	for(kana** tbl = kana_tables; *tbl != 0; ++ tbl) {
	    for(kana* ptr = *tbl; remain && ptr->hirakana; ++ ptr) {
		remain = translate(ptr->*target, ptr->*replacement)(str, remain);
	    }
	}
    }

    void hirakana_to_katakana(const std::string& from, std::string& to) {
	to = from;
	kana_convert(&kana::hirakana, &kana::katakana, to);
    }

    void hirakana_to_jisx0201_kana(const std::string& from, std::string& to) {
	to = from;
	kana_convert(&kana::hirakana, &kana::jisx0201_kana, to);
    }

    void katakana_to_hirakana(const std::string& from, std::string& to) {
	to = from;
	kana_convert(&kana::katakana, &kana::hirakana, to);
    }

    void katakana_to_jisx0201_kana(const std::string& from, std::string& to) {
	to = from;
	kana_convert(&kana::katakana, &kana::jisx0201_kana, to);
    }
    
    void jisx0201_kana_to_hirakana(const std::string& from, std::string& to) {
	to = from;
	kana_convert(&kana::jisx0201_kana, &kana::hirakana, to);
    }
    
    void jisx0201_kana_to_katakana(const std::string& from, std::string& to) {
	to = from;
	kana_convert(&kana::jisx0201_kana, &kana::katakana, to);
    }

    // latin record
    struct latin {
	const char* ascii;
	const char* jisx0208_latin;
    };

    static latin latin_table[] = {
	{ " ", "　" }, { "!", "！" }, { "\"", "”" }, { "#", "＃" },
	{ "$", "＄" }, { "%", "％" }, { "&", "＆" }, { "'", "’" },
	{ "(", "（" }, { ")", "）" }, { "*", "＊" }, { "+", "＋" },
	{ ",", "，" }, { "-", "−" }, { ".", "．" }, { "/", "／" },
	{ "0", "０" }, { "1", "１" }, { "2", "２" }, { "3", "３" },
	{ "4", "４" }, { "5", "５" }, { "6", "６" }, { "7", "７" },
	{ "8", "８" }, { "9", "９" }, { ":", "：" }, { ";", "；" },
	{ "<", "＜" }, { "=", "＝" }, { ">", "＞" }, { "?", "？" },
	{ "@", "＠" }, { "A", "Ａ" }, { "B", "Ｂ" }, { "C", "Ｃ" },
	{ "D", "Ｄ" }, { "E", "Ｅ" }, { "F", "Ｆ" }, { "G", "Ｇ" },
	{ "H", "Ｈ" }, { "I", "Ｉ" }, { "J", "Ｊ" }, { "K", "Ｋ" },
	{ "L", "Ｌ" }, { "M", "Ｍ" }, { "N", "Ｎ" }, { "O", "Ｏ" },
	{ "P", "Ｐ" }, { "Q", "Ｑ" }, { "R", "Ｒ" }, { "S", "Ｓ" },
	{ "T", "Ｔ" }, { "U", "Ｕ" }, { "V", "Ｖ" }, { "W", "Ｗ" },
	{ "X", "Ｘ" }, { "Y", "Ｙ" }, { "Z", "Ｚ" }, { "[", "［" },
	{ "\\", "＼" }, { "]", "］" }, { "^", "＾" }, { "_", "＿" },
	{ "`", "‘" }, { "a", "ａ" }, { "b", "ｂ" }, { "c", "ｃ" },
	{ "d", "ｄ" }, { "e", "ｅ" }, { "f", "ｆ" }, { "g", "ｇ" },
	{ "h", "ｈ" }, { "i", "ｉ" }, { "j", "ｊ" }, { "k", "ｋ" },
	{ "l", "ｌ" }, { "m", "ｍ" }, { "n", "ｎ" }, { "o", "ｏ" },
	{ "p", "ｐ" }, { "q", "ｑ" }, { "r", "ｒ" }, { "s", "ｓ" },
	{ "t", "ｔ" }, { "u", "ｕ" }, { "v", "ｖ" }, { "w", "ｗ" },
	{ "x", "ｘ" }, { "y", "ｙ" }, { "z", "ｚ" }, { "{", "｛" },
	{ "|", "｜" }, { "}", "｝" }, { "~", "〜" }, { 0, 0 }
    };

    void ascii_to_jisx0208_latin(const std::string& from, std::string& to) {
	to.clear();
	for(unsigned i = 0; i < from.size(); ++ i) {
	    if(from[i] < 0x20 || 0x7e < from[i]) {
		to += from[i];
	    } else {
		to += latin_table[from[i] - 0x20].jisx0208_latin;
	    }
	}
    }

    void jisx0208_latin_to_ascii(const std::string& from, std::string& to) {
	to = from;
	unsigned remain = to.size();
	for(latin* ptr = latin_table; remain && ptr->ascii; ++ ptr) {
	    remain = translate(ptr->jisx0208_latin, ptr->ascii)(to, remain);
	}
    }
}
