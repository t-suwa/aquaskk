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
#include "SKKDictionaryFactory.h"
#include "SKKBaseDictionary.h"

// Null 辞書
class SKKNullDictionary : public SKKBaseDictionary {
public:
    virtual void Initialize(const std::string&) {}

    virtual void Find(const SKKEntry&, SKKCandidateSuite&) {}
};

SKKDictionaryFactory::SKKDictionaryFactory() {}

SKKDictionaryFactory& SKKDictionaryFactory::theInstance() {
    static SKKDictionaryFactory obj;
    return obj;
}

void SKKDictionaryFactory::Register(int type, SKKDictionaryCreator creator) {
    if(creators_.find(type) != creators_.end()) {
	std::cerr << "type=" << type << " already has a creator" << std::endl;
	return;
    }

    creators_[type] = creator;
}

SKKBaseDictionary* SKKDictionaryFactory::Create(int type, const std::string& location) {
    if(creators_.find(type) == creators_.end()) {
	std::cerr << "type=" << type << " does not have a creator" << std::endl;
	return new SKKNullDictionary();
    }

    return creators_[type](location); 
}
