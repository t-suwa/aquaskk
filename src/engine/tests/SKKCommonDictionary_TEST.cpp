#include <cassert>
#include "SKKCommonDictionary.h"

void test(SKKBaseDictionary& dict) {
    SKKCandidateSuite suite;

    dict.Find(SKKEntry("NOT_EXIST", "d"), suite);
    assert(suite.IsEmpty());

    dict.Find(SKKEntry("よi", "い"), suite);
    assert(suite.ToString() == "/良/好/酔/善/");

    suite.Clear();
    dict.Find(SKKEntry("NOT_EXIST"), suite);
    assert(suite.IsEmpty());

    dict.Find(SKKEntry("かんじ"), suite);
    assert(suite.ToString() == "/漢字/寛治/官寺/");

    assert(dict.ReverseLookup("漢字") == "かんじ");
}

int main() {
    SKKCommonDictionary dict;

    dict.Initialize("SKK-JISYO.TEST");

    test(dict);
 
    SKKCommonDictionaryUTF8 utf8;

    utf8.Initialize("SKK-JISYO.TEST.UTF8");

    test(utf8);

    return 0;
}
