/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2006-2010 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef SKKProxyDictionary_h
#define SKKProxyDictionary_h

#include "SKKBaseDictionary.h"
#include "socketutil.h"

// 外部 skkserv 辞書
class SKKProxyDictionary: public SKKBaseDictionary {
    net::socket::endpoint remote_;
    net::socket::tcpstream session_;
    bool active_;

    bool connect();
    bool send(const SKKEntry& entry);
    bool ready();
    void recv(SKKCandidateSuite& result);

public:
    SKKProxyDictionary();
    virtual ~SKKProxyDictionary();

    virtual void Initialize(const std::string& path);

    virtual void Find(const SKKEntry& entry, SKKCandidateSuite& result);
};

#endif
