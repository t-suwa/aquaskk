/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2002 phonohawk
  Copyright (C) 2005-2010 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef SKKCommonDictionary_h
#define SKKCommonDictionary_h

#include "SKKDictionaryTemplate.h"
#include "SKKLocalDictionaryLoader.h"

// SKK 共有辞書

// EUC-JP 版
typedef SKKDictionaryTemplate<SKKLocalDictionaryLoader> SKKCommonDictionary;

// UTF-8 版
typedef SKKDictionaryTemplate<SKKLocalDictionaryLoader, SKKDictionaryKeeper::UTF_8> SKKCommonDictionaryUTF8;

#endif
