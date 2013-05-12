/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008 Tomotaka SUWA <t.suwa@mac.com>

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

#ifndef	SKKEntry_h
#define SKKEntry_h

#include "SKKInputMode.h"
#include <string>

// 見出し語
class SKKEntry {
    std::string normal_entry_;
    std::string okuri_entry_;
    std::string prefix_;
    std::string kana_;
    std::string prompt_;

    void updateEntry();

public:
    SKKEntry();
    SKKEntry(const std::string& entry, const std::string& okuri = "");

    void SetEntry(const std::string& entry);
    void AppendEntry(const std::string& str);
    void SetOkuri(const std::string& prefix, const std::string& kana);

    const std::string& EntryString() const;
    const std::string& OkuriString() const;
    const std::string& PromptString() const;

    std::string ToggleKana(SKKInputMode mode) const;
    std::string ToggleJisx0201Kana(SKKInputMode mode) const;

    // 正規化
    SKKEntry Normalize(SKKInputMode mode) const;

    bool IsEmpty() const;
    bool IsOkuriAri() const;

    friend bool operator==(const SKKEntry& left, const SKKEntry& right);
};

#endif
