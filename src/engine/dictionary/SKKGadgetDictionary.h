/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2009 Tomotaka SUWA <t.suwa@mac.com>

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

#ifndef SKKGadgetDictionary_h
#define SKKGadgetDictionary_h

#include <map>
#include "SKKBaseDictionary.h"

// ======================================================================
// プログラム実行変換辞書クラス
// ======================================================================
class SKKGadgetDictionary : public SKKBaseDictionary {
    typedef void (*DispatchHandler)(const std::string&, std::vector<std::string>&);
    typedef std::pair<std::string, DispatchHandler> DispatchPair;
    typedef std::vector<DispatchPair> DispatchTable;

    struct Match;
    struct Search;
    struct Comp;
    struct Store;

    DispatchTable table_;

    DispatchTable selectHandlers(const std::string& entry, bool complete = false) const;

    template <typename Predicate, typename Func>
    void apply(Predicate pred, Func func) {
        DispatchTable::iterator first = table_.begin();
        DispatchTable::iterator last = table_.end();

        while(first != last) {
            if(pred(*first)) {
                func(*first);
            }
            ++ first;
        }
    }

public:

    virtual void Initialize(const std::string& location);

    virtual void Find(const SKKEntry& entry, SKKCandidateSuite& result);

    virtual void Complete(SKKCompletionHelper& helper);
};

#endif
