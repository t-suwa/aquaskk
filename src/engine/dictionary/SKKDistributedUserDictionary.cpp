/*

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

#include "SKKDistributedUserDictionary.h"
#include "SKKCandidateSuite.h"
#include "utf8util.h"
#include "stringutil.h"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <cstdlib>

namespace {
    class basic_request {
        std::vector<std::string> request_;

    protected:
        void add_param(const std::string& param) {
            request_.push_back(param);
        }

    public:
        bool send(std::iostream& stream) {
            const std::string CRLF = "\r\n";
            std::string line;

            stream << string::join(request_, "\t") << CRLF << std::flush;

            std::getline(stream, line);

            return line.find("OK") == 0;
        }
    };

    class get_request : public basic_request {
    public:
        get_request(const SKKEntry& entry) {
            add_param("GET");
            add_param(entry.EntryString());
            add_param(entry.OkuriString());
        }
    };

    class complete_request : public basic_request {
    public:
        complete_request(const SKKEntry& entry) {
            add_param("COMPLETE");
            add_param(entry.EntryString());
        }
    };

    class update_request : public basic_request {
    protected:
        update_request(const std::string& command, const SKKEntry& entry, const SKKCandidate& candidate) {
            SKKCandidate tmp(candidate);

            tmp.Encode();

            add_param(command);
            add_param(entry.EntryString());
            add_param(tmp.ToString());
            add_param(entry.OkuriString());
        }
    };

    class put_request : public update_request {
    public:
        put_request(const SKKEntry& entry, const SKKCandidate& candidate)
            : update_request("PUT", entry, candidate) {}
    };

    class delete_request : public update_request {
    public:
        delete_request(const SKKEntry& entry, const SKKCandidate& candidate)
            : update_request("DELETE", entry, candidate) {}
    };

    class response {
        string::splitter splitter_;

    public:
        response(std::iostream& stream) {
            std::string line;

            std::getline(stream, line);

            splitter_.split(line, "/");
        }

        response& operator>>(std::string& str) {
            splitter_ >> str;
            return *this;
        }

        operator bool() {
            return splitter_;
        }
    };
}

void SKKDistributedUserDictionary::Initialize(const std::string& path) {
    server_.close();
    server_.open("localhost", atoi(path.c_str()));
}

void SKKDistributedUserDictionary::Find(const SKKEntry& entry, SKKCandidateSuite& result) {
    get_request request(entry);

    if(request.send(server_)) {
        response response(server_);
        std::string buf;

        while(response >> buf) {
            SKKCandidate candidate(buf);

            candidate.Decode();

            result.Add(candidate);
        }
    }
}

std::string SKKDistributedUserDictionary::ReverseLookup(const std::string& candidate) {
    // 今のところサポートしない
    return "";
}

void SKKDistributedUserDictionary::Complete(SKKCompletionHelper& helper) {
    complete_request request(helper.Entry());

    if(request.send(server_)) {
        response response(server_);
        std::string buf;

        while(response >> buf && helper.CanContinue()) {
            helper.Add(buf);
        }
    }
}

void SKKDistributedUserDictionary::Register(const SKKEntry& entry, const SKKCandidate& candidate) {
    put_request request(entry, candidate);

    if(!request.send(server_)) {
        std::cerr << "SKKDistributedUserDictionary: put failed" << std::endl;
    }
}

void SKKDistributedUserDictionary::Remove(const SKKEntry& entry, const SKKCandidate& candidate) {
    delete_request request(entry, candidate);

    if(!request.send(server_)) {
        std::cerr << "SKKDistributedUserDictionary: delete failed" << std::endl;
    }
}

void SKKDistributedUserDictionary::SetPrivateMode(bool flag) {
    // FIXME: to be done
}
