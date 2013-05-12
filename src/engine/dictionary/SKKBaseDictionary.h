/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2007-2010 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef SKKBaseDictionary_h
#define SKKBaseDictionary_h

#include <iostream>
#include <string>
#include <vector>
#include "SKKEntry.h"
#include "SKKCandidateSuite.h"
#include "SKKCompletionHelper.h"

// ======================================================================
// 抽象辞書クラス(文字列は UTF-8)
// ======================================================================
class SKKBaseDictionary {
public:
    virtual ~SKKBaseDictionary() {};

    // 辞書の初期化
    //
    // location の書式は各辞書の実装で定義する
    //
    virtual void Initialize(const std::string& location) = 0;

    // 候補検索
    virtual void Find(const SKKEntry& entry, SKKCandidateSuite& result) = 0;

    // 見出し語検索
    virtual std::string ReverseLookup(const std::string& candidate) {
        return "";
    }

    // 見出し語補完
    virtual void Complete(SKKCompletionHelper& helper) {}
};

#endif
