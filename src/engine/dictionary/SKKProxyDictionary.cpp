/*

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

#include <sstream>
#include "SKKProxyDictionary.h"
#include "jconv.h"

SKKProxyDictionary::SKKProxyDictionary() : active_(false) {}

SKKProxyDictionary::~SKKProxyDictionary() {
    session_.close();
}

void SKKProxyDictionary::Initialize(const std::string& location) {
    remote_.parse(location, "1178");

    session_.close();
}

void SKKProxyDictionary::Find(const SKKEntry& entry, SKKCandidateSuite& result) {
    // 再入でループするのを防ぐ
    if(!active_) {
        active_ = true;

        if(connect() && send(entry) && ready()) {
            recv(result);
        }

        active_ = false;
    }
}

// ----------------------------------------------------------------------

bool SKKProxyDictionary::connect() {
    if(!session_) {
        session_.open(remote_);
    }

    return (bool)session_;
}

bool SKKProxyDictionary::send(const SKKEntry& entry) {
    session_ << '1'
             << jconv::eucj_from_utf8(entry.EntryString())
             << ' '
             << std::flush;

    return (bool)session_;
}

bool SKKProxyDictionary::ready() {
    net::socket::monitor monitor;
    auto fd = session_.socket();
    auto type = net::socket::monitor::READ;

    monitor.add(fd, type);

    return monitor.wait(1) == 1 && monitor.test(fd, type);
}

void SKKProxyDictionary::recv(SKKCandidateSuite& result) {
    std::string response;

    if(std::getline(session_, response).eof() ||
       response.size() < 2 || response[0] != '1') {
        return;
    }

    SKKCandidateSuite suite(jconv::utf8_from_eucj(response.substr(1)));

    result.Add(suite);
}
