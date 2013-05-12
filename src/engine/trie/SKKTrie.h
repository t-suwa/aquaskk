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

#include <string>
#include <map>
#include "SKKInputMode.h"

#ifndef SKKTrie_h
#define SKKTrie_h

class SKKTrie;

// 変換ヘルパー
class SKKTrieHelper {
public:
    virtual ~SKKTrieHelper() {}

    // 変換用文字
    virtual const std::string& SKKTrieRomanString() const = 0;

    // 通知：変換結果
    virtual void SKKTrieNotifyConverted(const SKKTrie* node) = 0;

    // 通知：未変換結果
    virtual void SKKTrieNotifyNotConverted(char code) = 0;

    // 通知：途中結果
    virtual void SKKTrieNotifyIntermediate(const SKKTrie* node) = 0;

    // 通知：スキップ指示
    virtual void SKKTrieNotifySkipLength(int length) = 0;

    // 通知：データ不足
    virtual void SKKTrieNotifyShort() = 0;
};

// kana-rule.conf の木表現
class SKKTrie {
    bool leaf_;
    std::string hirakana_;
    std::string katakana_;
    std::string jisx0201kana_;
    std::string next_;

    std::map<char, SKKTrie> children_;

public:
    SKKTrie();
    SKKTrie(const std::string& hirakana, const std::string& katakana,
	    const std::string& jisx0201kana, const std::string& next);

    // 初期化
    void Clear();

    // 再帰的ノード追加
    //
    // 引数：
    //	str=変換文字列
    //	node=追加ノード
    //	depth=探索の深度(再帰専用)
    //
    // 例：
    // 	node.Add("gya", SKKTrie("ぎゃ", "ギャ", "ｷﾞｬ", ""));
    //
    void Add(const std::string& str, const SKKTrie& node, unsigned depth = 0);

    // 再帰的ノード検索
    //
    // 引数：
    //	helper=検索ヘルパー
    //	depth=探索の深度(再帰専用)
    //
    void Traverse(SKKTrieHelper& helper, unsigned depth = 0);

    // かな文字列取得
    const std::string& KanaString(SKKInputMode mode) const;

    // 次状態文字列取得
    const std::string& NextState() const;

    // 葉の要素か？(結果が true でも、節のケースもある)
    bool IsLeaf() const;
};

#endif
