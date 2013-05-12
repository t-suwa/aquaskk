#include <cassert>
#include "SKKDictionaryFile.h"

int main() {
    SKKDictionaryEntryContainer okuriAri;
    SKKDictionaryEntryContainer okuriNasi;

    okuriAri.push_back(SKKDictionaryEntry("うけとt", "/受け取/受取/"));
    okuriAri.push_back(SKKDictionaryEntry("いあw", "/居合/"));

    okuriNasi.push_back(SKKDictionaryEntry("かんじ", "/漢字/官寺/寛治/"));
    okuriNasi.push_back(SKKDictionaryEntry("かいはつ", "/開発/"));

    SKKDictionaryFile file;

    file.OkuriAri() = okuriAri;
    file.OkuriNasi() = okuriNasi;

    file.Save("dict.file");

    file.OkuriAri().clear();
    file.OkuriNasi().clear();

    file.Load("dict.file");

    assert(file.OkuriAri() == okuriAri && file.OkuriNasi() == okuriNasi);
}
