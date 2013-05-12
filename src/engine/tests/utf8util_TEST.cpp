#include <cassert>
#include "utf8util.h"

int main() {
    std::string str = "ABCいろは日本語ＡＢＣ";

    assert(utf8::length(str) == 12);
    assert(utf8::left(str, -6) == "ABCいろは");
    assert(utf8::right(str, -6) == "日本語ＡＢＣ");
    assert(utf8::common_prefix("1漢字2", "1漢字3") == "1漢字");
    assert(utf8::common_prefix("いろは", "あいう") == "");
}
