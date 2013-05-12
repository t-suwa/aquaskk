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

#include "SKKDictionaryFile.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>

static std::string OKURI_ARI_MARK = ";; okuri-ari entries.";
static std::string OKURI_NASI_MARK = ";; okuri-nasi entries.";

bool SKKDictionaryFile::Load(const std::string& path) {
    okuriAri_.clear();
    okuriNasi_.clear();

    if(!exist(path)) return false;

    std::ifstream ifs(path.c_str());
    SKKDictionaryEntry entry;

    while(std::getline(ifs, entry.second)) {
	if(OKURI_ARI_MARK.find(entry.second) != std::string::npos) break;
    }

    for(okuriAri_.clear(); fetch(ifs, entry); okuriAri_.push_back(entry)) {
	if(OKURI_NASI_MARK.find(entry.second) != std::string::npos) break;
    }

    for(okuriNasi_.clear(); fetch(ifs, entry); okuriNasi_.push_back(entry)) {}

    return true;
}

bool SKKDictionaryFile::Save(const std::string& path) {
    std::ofstream ofs(path.c_str());

    ofs << OKURI_ARI_MARK << std::endl;
    if(!store(ofs, okuriAri_)) return false;

    ofs << OKURI_NASI_MARK << std::endl;
    if(!store(ofs, okuriNasi_)) return false;

    return true;
}

bool SKKDictionaryFile::IsEmpty() const {
    return okuriAri_.empty() && okuriNasi_.empty();
}

void SKKDictionaryFile::Sort() {
    sort(okuriAri_);
    sort(okuriNasi_);
}

SKKDictionaryEntryContainer& SKKDictionaryFile::OkuriAri() {
    return okuriAri_;
}

SKKDictionaryEntryContainer& SKKDictionaryFile::OkuriNasi() {
    return okuriNasi_;
}

bool SKKDictionaryFile::exist(const std::string& path) {
    struct stat st;

    if(stat(path.c_str(), &st) < 0 || !S_ISREG(st.st_mode)) {
	std::cerr << "SKKDictionaryFile::open(): can't open: " << path << std::endl;
	return false;
    }

    return true;
}

bool SKKDictionaryFile::fetch(std::istream& is, SKKDictionaryEntry& entry) {
    if(is >> entry.first && is.ignore() && std::getline(is, entry.second)) {
	return true;
    }

    return false;
}

bool SKKDictionaryFile::store(std::ostream& os, const SKKDictionaryEntryContainer& container) {
    for(unsigned i = 0; i < container.size(); ++ i) {
	os << container[i].first << ' ' << container[i].second << std::endl;
    }

    return os.good();
}

void SKKDictionaryFile::sort(SKKDictionaryEntryContainer& container) {
    std::sort(container.begin(), container.end(), SKKDictionaryEntryCompare());
}
