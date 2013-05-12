/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2010 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef SKKDictionaryTemplate_h
#define SKKDictionaryTemplate_h

#include "SKKBaseDictionary.h"
#include "SKKDictionaryKeeper.h"

// 標準的な SKK 辞書実装用のテンプレート

template <typename Loader, SKKDictionaryKeeper::Encoding encoding = SKKDictionaryKeeper::EUC_JP>
class SKKDictionaryTemplate : public SKKBaseDictionary {
    SKKDictionaryKeeper keeper_;
    Loader loader_;

public:
    SKKDictionaryTemplate() : keeper_(encoding) {}

    virtual void Initialize(const std::string& location) {
        loader_.Initialize(location);
        keeper_.Initialize(&loader_);
    }

    virtual void Find(const SKKEntry& entry, SKKCandidateSuite& result) {
        const std::string& key = entry.EntryString();
        SKKCandidateSuite suite;

        if(entry.IsOkuriAri()) {
            suite.Parse(keeper_.FindOkuriAri(key));

            SKKCandidateSuite strict;

            if(suite.FindOkuriStrictly(entry.OkuriString(), strict)) {
                strict.Add(suite.Hints());
                suite = strict;
            }
        } else {
            suite.Parse(keeper_.FindOkuriNasi(key));
        }

        result.Add(suite);
    }

    virtual std::string ReverseLookup(const std::string& candidate) {
        return keeper_.ReverseLookup(candidate);
    }

    virtual void Complete(SKKCompletionHelper& helper) {
        keeper_.Complete(helper);
    }
};

#endif
