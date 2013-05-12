#include "SKKDistributedUserDictionary.h"
#include "pthreadutil.h"
#include "socketutil.h"
#include "stringutil.h"
#include "MockCompletionHelper.h"

class param {
    string::splitter splitter_;

public:
    param(const std::string& line) {
        splitter_.split(line, " ");

        splitter_ >> command >> entry;

        if(command == "PUT" || command == "DELETE") {
            splitter_ >> candidate;
        }

        splitter_ >> okuri;
    }

    std::string command;
    std::string entry;
    std::string candidate;
    std::string okuri;
};

class server : public pthread::task {
    net::socket::tcpserver server_;

    void session(std::iostream& stream) {
        std::string line;

        while(std::getline(stream, line)) {
            param param(line);

            stream << "OK" << "\r\n" << std::flush;

            if(param.command == "GET" || param.command == "COMPLETE") {
                stream << "\r\n" << std::flush;
            }
        }
    }

public:
    server() {
        server_.open(10789);
    }

    virtual bool run() {
        while(int fd = server_.accept()) {
            net::socket::tcpstream stream(fd);

            session(stream);
        }

        return false;
    }
};

int main() {
    server server;
    pthread::timer timer(&server, 0);
    SKKDistributedUserDictionary dict;
    SKKCandidateSuite suite;
    MockCompletionHelper helper;

    dict.Initialize("127.0.0.1:10789");

    dict.Find(SKKEntry("かんじ"), suite);
    assert(suite.IsEmpty());

    assert(dict.ReverseLookup("not found") == "");

    helper.Initialize("かんじ");
    
    dict.Complete(helper);

    assert(helper.Result().empty());

    dict.Register(SKKEntry("かんじ"), SKKCandidate("漢字"));

    dict.Remove(SKKEntry("かんじ"), SKKCandidate("漢字"));

    dict.SetPrivateMode(true);
}
