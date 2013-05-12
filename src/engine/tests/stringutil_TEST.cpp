#include "stringutil.h"
#include <cassert>
#include <iostream>
#include <vector>

int main() {
    string::splitter splitter;
    std::string str;

    splitter.split("a,b,c,d");

    assert(splitter >> str && str == "a");
    assert(splitter >> str && str == "b");
    assert(splitter >> str && str == "c");
    assert(splitter >> str && str == "d");
    assert(!(splitter >> str));

    splitter.split("abc::def::ghi", "::");

    assert(splitter >> str && str == "abc");
    assert(splitter >> str && str == "def");
    assert(splitter >> str && str == "ghi");
    assert(!(splitter >> str));

    splitter.split("::abc:,def:,ghi::", "::");
    assert(splitter >> str && str == "abc:,def:,ghi");
    assert(!(splitter >> str));

    splitter.split("abc def", " ");
    assert(splitter >> str && str == "abc");
    assert(splitter >> str && str == "def");
    assert(!(splitter >> str));

    str = "begin%20%20first%2second%0third20%end";
    string::translate(str, "%20", " ");
    assert(str == "begin  first%2second%0third20%end");

    std::vector<std::string> a;

    a.push_back("abc");
    a.push_back("def");
    a.push_back("ghi");
    a.push_back("jkl");

    assert(string::join(a) == "abc def ghi jkl");
    assert(string::join(a, "/") == "abc/def/ghi/jkl");

    splitter.split("abc 123 3.14");
    int ivalue;
    double dvalue;
    assert(splitter >> str && str == "abc");
    assert(splitter >> ivalue && ivalue == 123);
    assert(splitter >> dvalue && dvalue == 3.14);
}
