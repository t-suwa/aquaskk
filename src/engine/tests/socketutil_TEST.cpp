#include "socketutil.h"

int main() {
    net::socket::endpoint ep1("localhost", "http");
    assert(ep1.node() == "localhost" && ep1.service() == "http");

    net::socket::endpoint ep2("localhost", 80);
    assert(ep2.node() == "localhost" && ep2.service() == "80");

    ep1.parse("localhost:80", "8080");
    assert(ep1.node() == "localhost" && ep1.service() == "80");

    ep1.parse("localhost", "8080");
    assert(ep1.node() == "localhost" && ep1.service() == "8080");
}
    
