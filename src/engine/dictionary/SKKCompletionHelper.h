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

#ifndef SKKCompletionHelper_h
#define SKKCompletionHelper_h

#include <string>

// 見出し語補完ヘルパー

class SKKCompletionHelper {
public:
    virtual ~SKKCompletionHelper() {}

    // 見出し語
    virtual const std::string& Entry() const = 0;

    // 補完継続可否
    virtual bool CanContinue() const = 0;

    // 補完候補の追加
    virtual void Add(const std::string& completion) = 0;
};

#endif
