/*

  MacOS X implementation of the SKK input method.

  Copyright (C) 2002-2004 phonohawk
  Copyright (C) 2006-2008 Tomotaka SUWA <t.suwa@mac.com>

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

#include "skkserv.h"
#include <iostream>
#include <string>
#include <cctype>
#include <unistd.h>
#include "SKKBackEnd.h"
#include "jconv.h"

namespace {
    // スレッド引数
    class thread_param {
        int fd_;

    public:
        thread_param(int arg) : fd_(arg) {}

        int fd() const { return fd_; }
    };
}

skkserv::skkserv(unsigned short port, bool localonly) : thread_(0), localonly_(localonly) {
    server_.open(port);

    if(server_) {
        incoming_.add(server_.socket(), net::socket::monitor::READ);
        pthread_create(&thread_, 0, skkserv::listener, this);
    } else {
        std::cerr << "skkserv::start(): can't open port " << port << std::endl;
    }
}

skkserv::~skkserv() {
    if(thread_ && pthread_cancel(thread_) == 0) {
	server_.close();
        pthread_join(thread_, 0);
	thread_ = 0;
    }
}

void* skkserv::listener(void* param) {
    skkserv* server = reinterpret_cast<skkserv*>(param);

    while(true) {
	pthread_testcancel();
	server->accept();
    }

    return 0;
}

void skkserv::accept() {
    if(0 < incoming_.wait()) {
	int fd = server_.accept();
	if(fd == -1) return;

	net::socket::namepair remote = net::socket::nameinfo::remote(fd);
	if(localonly_ && remote.first != "127.0.0.1") {
	    std::cout << "AquaSKK(skkserv): reject[" << remote.first << "]" << std::endl;
	    close(fd);
	    return;
	}

	pthread_t tmp;
	thread_param* param = new thread_param(fd);
	pthread_create(&tmp, 0, skkserv::worker, param);
    }
}

void* skkserv::worker(void* arg) {
    thread_param* param = reinterpret_cast<thread_param*>(arg);
    net::socket::tcpstream session(param->fd());
    net::socket::namepair remote = net::socket::nameinfo::remote(session.socket());
    delete param;

    std::cout << "AquaSKK(skkserv): session start[" << remote.first << "]" << std::endl;

    unsigned char cmd;
    do {
        cmd = session.get();
        switch(cmd) {
        case '0':		// 切断
            break;

	case '1': {		// 検索
	    std::string word;
            session >> word;
            session.get();

	    std::string key = jconv::utf8_from_eucj(word);
	    SKKEntry entry;
	    SKKCandidateSuite result;

	    // 検索文字列の最後が [a-z] なら『送りあり』とする
	    if(1 < key.size() && 0x7f < (unsigned)key[0] && std::isalpha(key[key.size() - 1])) {
		entry = SKKEntry(key, "dummy");
	    } else {
		entry = SKKEntry(key);
	    }
	    
	    SKKBackEnd::theInstance().Find(entry, result);
            std::string candidates = jconv::eucj_from_utf8(result.ToString(true));
            if(!candidates.empty()) {
		session << '1' << candidates << std::endl;
	    } else {
		session << '4' << word << std::endl;
	    }
	    session.flush();
	}
            break;

        case '2':		// バージョン
	    session << "AquaSKKServer1.0 " << std::flush;
            break;

        case '3':		// ホスト情報
	    session << "127.0.0.1:0.0.0.0: " << std::flush;
            break;

	case '4':		// サーバー補完
	    // 今のところ未対応
	    session.ignore(0xff, ' ');

        default:		// 無効なコマンド
	    fprintf(stderr, "AquaSKK(skkserv): Unknown command[0x%02x]\n", cmd);
	    session << '0' << std::flush;
            break;
	}
    } while(session.good() && cmd != '0');

    std::cout << "AquaSKK(skkserv): session finish[" << remote.first << "]" << std::endl;

    return 0;
}
