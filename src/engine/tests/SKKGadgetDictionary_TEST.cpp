#include <iostream>
#include "SKKGadgetDictionary.h"

int main() {
    SKKGadgetDictionary dict;

    dict.Initialize("hoge");

    SKKCandidateSuite suite;
    dict.Find(SKKEntry("today"), suite);
    dict.Find(SKKEntry("now"), suite);
    dict.Find(SKKEntry("=(32768+64)*1024"), suite);

    std::cerr << suite.ToString() << std::endl;
}
