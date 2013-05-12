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

#include <iostream>
#include "SKKTrie.h"

SKKTrie::SKKTrie() : leaf_(false) {}

SKKTrie::SKKTrie(const std::string& hirakana,
		 const std::string& katakana,
		 const std::string& jisx0201kana,
		 const std::string& next)
    : leaf_(true), hirakana_(hirakana), katakana_(katakana), jisx0201kana_(jisx0201kana), next_(next) {}

void SKKTrie::Clear() {
    children_.clear();
}

void SKKTrie::Add(const std::string& str, const SKKTrie& node, unsigned depth) {
    // 末端か？
    if(depth == str.size() - 1) {
	children_[str[depth]] = node;
    } else {
	children_[str[depth]].Add(str, node, depth + 1); // 再帰追加
    }
}

void SKKTrie::Traverse(SKKTrieHelper& helper, unsigned depth) {
    const std::string& str = helper.SKKTrieRomanString();

    // [1] データ不足(ex. "k" や "ch" など)
    if(depth == str.size()) {
        if(IsLeaf()) {
            helper.SKKTrieNotifyIntermediate(this);
        }

	return helper.SKKTrieNotifyShort();
    }

    // 一致？
    if(children_.find(str[depth]) != children_.end()) {
	SKKTrie* node = &children_[str[depth]];

	// 末端でないなら再帰検索
	if(!node->children_.empty()) {
	    return node->Traverse(helper, depth + 1);
	}

	// [2] 完全一致
        helper.SKKTrieNotifyConverted(node);
        helper.SKKTrieNotifySkipLength(depth + 1);

        return;
    }

    // [3] 部分一致(ex. "kb" や "chm" など)
    if(0 < depth) {
        if(IsLeaf()) {
            helper.SKKTrieNotifyConverted(this);
        }
        helper.SKKTrieNotifySkipLength(depth);

        return;
    }

    // [4] 最初の一文字が木に存在しない
    helper.SKKTrieNotifyNotConverted(str[0]);
    helper.SKKTrieNotifySkipLength(1);
}

const std::string& SKKTrie::KanaString(SKKInputMode mode) const {
    static std::string nothing;

    switch(mode) {
    case HirakanaInputMode:
	return hirakana_;

    case KatakanaInputMode:
	return katakana_;

    case Jisx0201KanaInputMode:
	return jisx0201kana_;

    default:
	std::cerr << "SKKTrie::KanaString(): invalid mode [" << mode << "]" << std::endl;
	return nothing;
    }
}

const std::string& SKKTrie::NextState() const {
    return next_;
}

bool SKKTrie::IsLeaf() const {
    return leaf_;
}
