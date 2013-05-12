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

#ifndef SKKLocalDictionaryLoader_h
#define SKKLocalDictionaryLoader_h

#include "SKKDictionaryLoader.h"
#include <ctime>
#include <sys/stat.h>

// ローカル辞書ローダー

class SKKLocalDictionaryLoader : public SKKDictionaryLoader {
    std::time_t lastupdate_;
    std::string path_;

    virtual bool NeedsUpdate() {
        struct stat st;

        if(stat(path_.c_str(), &st) == 0 && lastupdate_ < st.st_mtime) {
            lastupdate_ = st.st_mtime;
            return true;
        }

        return false;
    }

    virtual const std::string& FilePath() const {
        return path_;
    }

public:
    SKKLocalDictionaryLoader() : lastupdate_(0) {}

    virtual void Initialize(const  std::string& location) {
        path_ = location;
    }

    virtual int Interval() const {
        return 60;
    }

    virtual int Timeout() const {
        return 1;
    }
};

#endif

