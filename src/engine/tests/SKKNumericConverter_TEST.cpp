#include "SKKNumericConverter.h"
#include "SKKCandidate.h"
#include <iostream>
#include <cassert>

int main() {
    SKKNumericConverter converter;

    // 数値変換の対象外
    assert(!converter.Setup("abc"));
    assert(converter.OriginalKey() == "abc");
    assert(converter.NormalizedKey() == "abc");

    // 数値変換対象
    assert(converter.Setup("0-1-2-1234-4-1234-34"));
    assert(converter.NormalizedKey() == "#-#-#-#-#-#-#");

    SKKCandidate candidate("#0-#1-#2-#3-#4-#5-#9");
    converter.Apply(candidate);

    //std::cerr << candidate.Variant() << std::endl;
    assert(candidate.Variant() == "0-１-二-千二百三十四-4-壱阡弐百参拾四-３四");
}
