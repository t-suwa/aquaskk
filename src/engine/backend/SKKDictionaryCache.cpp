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

#include "SKKDictionaryCache.h"
#include "SKKDictionaryFactory.h"
#include "SKKBaseDictionary.h"

namespace {
    struct DeleteDictionary {
	void operator()(const std::pair<SKKDictionaryKey, SKKBaseDictionary*>& entry) const {
	    delete entry.second;
	}
    };
}

SKKDictionaryCache::~SKKDictionaryCache() {
    std::for_each(cache_.begin(), cache_.end(), DeleteDictionary());
}

SKKBaseDictionary* SKKDictionaryCache::Get(const SKKDictionaryKey& key) {
    if(cache_.find(key) == cache_.end()) {
	cache_[key] = SKKDictionaryFactory::theInstance().Create(key.first, key.second);
    }

    return cache_[key];
}

void SKKDictionaryCache::Clear(const SKKDictionaryKey& key) {
    if(cache_.find(key) != cache_.end()) {
	delete cache_[key];
	cache_.erase(key);
    }
}
