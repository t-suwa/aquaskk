#include <cassert>
#include "SKKCandidate.h"

int main() {
    SKKCandidate c1;

    assert(c1.IsEmpty() && c1.ToString() == "");

    std::string src("候補;アノテーション");
    SKKCandidate c2(src);

    assert(c2.Word() == "候補" && c2.Annotation() == "アノテーション");
    assert(c2.ToString() == src);

    c1 = c2;

    assert(c1.ToString() == src && c1 == c2);

    std::string variant("数値変換");
    c1.SetVariant(variant);

    assert(c1.Variant() == variant && c1 != c2);

    assert(SKKCandidate::Encode("[/;") == "[5b][2f][3b]");
    assert(SKKCandidate::Decode("[5b][2f][3b]") == "[/;");
}
