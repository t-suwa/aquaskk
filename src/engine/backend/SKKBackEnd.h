/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008-2010 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef	SKKBackEnd_h
#define SKKBackEnd_h

#include "SKKCandidateSuite.h"
#include "SKKDictionaryKey.h"
#include "SKKDictionaryCache.h"
#include "SKKEntry.h"
#include <string>
#include <vector>
#include <memory>

class SKKBaseDictionary;
class SKKUserDictionary;

class SKKBackEnd {
    std::auto_ptr<SKKUserDictionary> userdict_;
    std::vector<SKKBaseDictionary*> dicts_;
    SKKDictionaryKeyContainer actives_;
    SKKDictionaryCache cache_;
    bool useNumericConversion_;
    bool enableExtendedCompletion_;
    int minimumCompletionLength_;

    SKKBackEnd();
    SKKBackEnd(const SKKBackEnd&);
    SKKBackEnd& operator=(const SKKBackEnd&);

    SKKEntry normalize(const SKKEntry& entry);

public:
    static SKKBackEnd& theInstance();

    void Initialize(const std::string& userdict_path, const SKKDictionaryKeyContainer& keys);

    // 初期化
    void Initialize(SKKUserDictionary* dictionary, const SKKDictionaryKeyContainer& keys);

    // 補完
    bool Complete(const std::string& key, std::vector<std::string>& result, unsigned limit = 0);

    // 検索
    bool Find(const SKKEntry& entry, SKKCandidateSuite& result);

    // 逆引き
    std::string ReverseLookup(const std::string& candidate);

    // 登録
    void Register(const SKKEntry& entry, const SKKCandidate& candidate);

    // 削除
    void Remove(const SKKEntry& entry, const SKKCandidate& candidate);

    // オプション：数値変換
    void UseNumericConversion(bool flag);

    // オプション：拡張補完
    void EnableExtendedCompletion(bool flag);

    // オプション：プライベートモード
    void EnablePrivateMode(bool flag);

    // オプション：補完候補の長さの下限(足切り)
    void SetMinimumCompletionLength(int length);
};

#endif
