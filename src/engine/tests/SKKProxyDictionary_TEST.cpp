#include <cassert>
#include <errno.h>
#include "SKKProxyDictionary.h"
#include "SKKCommonDictionary.h"
#include "jconv.h"

void session(int fd, SKKCommonDictionary& dict) {
    net::socket::tcpstream sock(fd);
    unsigned char cmd;

    do {
        cmd = sock.get();
        switch(cmd) {
        case '0':		// 切断
            break;

	case '1': {		// 検索
	    std::string word;
	    std::string key;
            sock >> word;
            sock.get();

	    jconv::convert_eucj_to_utf8(word, key);

	    SKKCandidateSuite result;
            SKKEntry entry(key);

	    // 検索文字列の最後が [a-z] なら『送りあり』
	    if(1 < key.size() && 0x7f < (unsigned)key[0] && std::isalpha(key[key.size() - 1])) {
                entry = SKKEntry(key, "dummy");
	    }

            dict.Find(entry, result);

	    // 見つかった？
	    if(!result.IsEmpty()) {
		std::string candidates;
		jconv::convert_utf8_to_eucj(result.ToString(), candidates);
		sock << '1' << candidates << std::endl;
	    } else {
		sock << '4' << word << std::endl;
	    }
	    sock << std::flush;
	}
            break;

        default:		// 無効なコマンド
	    sock << '0' << std::flush;
            break;
	}
    } while(sock.good() && cmd != '0');
    sock.close();
}

void notify_ok(void* param) {
    auto* condition = (pthread::condition*)param;
    pthread::lock lock(*condition);

    condition->signal();
}

// 正常サーバー
void* normal_server(void* param) {
    SKKCommonDictionary dict;

    dict.Initialize("SKK-JISYO.TEST");

    ushort port = 23000;
    net::socket::tcpserver skkserv(port);

    notify_ok(param);

    while(true) {
	session(skkserv.accept(), dict);
    }

    return 0;
}

// だんまりサーバー
void* dumb_server(void* param) {
    ushort port = 33000;
    net::socket::tcpserver skkserv(port);

    notify_ok(param);

    while(true) {
        skkserv.accept();
    }

    return 0;
}

// おかしなサーバー
void* mad_server(void* param) {
    ushort port = 43000;
    net::socket::tcpserver skkserv(port);

    notify_ok(param);

    while(true) {
        auto fd = skkserv.accept();
        net::socket::tcpstream session(fd);

        session << "やれやれ" << std::endl << std::flush;
    }

    return 0;
}

// 自殺サーバー
void* suicide_server(void* param) {
    ushort port = 53000;
    net::socket::tcpserver skkserv(port);

    notify_ok(param);

    while(true) {
        close(skkserv.accept());
    }

    return 0;
}

// サーバー起動
void spawn_server(void*(*server)(void* param)) {
    pthread_t thread;
    pthread::condition ready; 
    pthread::lock lock(ready);

    pthread_create(&thread, 0, server, &ready);
    pthread_detach(thread);

    ready.wait();
}

int main() {
    spawn_server(normal_server);
    spawn_server(dumb_server);
    spawn_server(mad_server);
    spawn_server(suicide_server);

    SKKProxyDictionary proxy;
    SKKCandidateSuite suite;

    // 存在しないサーバーテスト
    proxy.Initialize("127.0.0.1:33333");

    proxy.Find(SKKEntry("よi", "い"), suite);
    assert(suite.IsEmpty());

    // 正常系テスト
    proxy.Initialize("127.0.0.1:23000");

    proxy.Find(SKKEntry("よi", "い"), suite);
    assert(suite.ToString() == "/良/好/酔/善/");

    suite.Clear();
    proxy.Find(SKKEntry("NOT-EXIST", "i"), suite);
    assert(suite.IsEmpty());

    proxy.Find(SKKEntry("かんじ"), suite);
    assert(suite.ToString() == "/漢字/寛治/官寺/");

    suite.Clear();
    
    proxy.Find(SKKEntry("NOT-EXIST"), suite);
    assert(suite.IsEmpty());

    // だんまりサーバーテスト
    proxy.Initialize("127.0.0.1:33000");

    proxy.Find(SKKEntry("かんじ"), suite);
    assert(suite.IsEmpty());

    // おかしなサーバーテスト
    proxy.Initialize("127.0.0.1:43000");

    proxy.Find(SKKEntry("かんじ"), suite);
    assert(suite.IsEmpty());

    // 自殺サーバーテスト
    proxy.Initialize("127.0.0.1:53000");

    proxy.Find(SKKEntry("かんじ"), suite);
    assert(suite.IsEmpty());
}

