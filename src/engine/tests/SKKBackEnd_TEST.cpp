#include <cassert>
#include <iostream>
#include "SKKBackEnd.h"
#include "SKKDictionaryFactory.h"
#include "SKKCommonDictionary.h"

int main() {
    SKKRegisterFactoryMethod<SKKCommonDictionary>(0);
    
    SKKDictionaryKeyContainer dicts;

    dicts.push_back(SKKDictionaryKey(0, "SKK-JISYO.TEST"));
    dicts.push_back(SKKDictionaryKey(0, "SKK-JISYO.TEST"));

    auto& backend = SKKBackEnd::theInstance();

    backend.Initialize("skk-jisyo.utf8", dicts);

    std::vector<std::string> result;
    SKKCandidateSuite suite;

    // 補完
    assert(backend.Complete("か", result) && result[0] == "かんじ");

    // 検索
    assert(!backend.Find(SKKEntry("NOT-EXIST", "え"), suite));
    assert(backend.Find(SKKEntry("よi", "い"), suite) && suite.ToString() == "/良/好/酔/善/");
    assert(!backend.Find(SKKEntry("NOT-EXIST"), suite));
    assert(backend.Find(SKKEntry("たんごとうろく"), suite) && suite.ToString() == "/単語登録/");

    // 登録
    backend.Register(SKKEntry("あr", "り"), SKKCandidate("有"));
    backend.Register(SKKEntry("かなめ"), SKKCandidate("要"));

    // 補完
    assert(backend.Complete("か", result) && result[0] == "かなめ" && result[1] == "かんじ");

    // 検索
    assert(backend.Find(SKKEntry("あr", "り"), suite) && suite.ToString() == "/有/[り/有/]/");
    assert(backend.Find(SKKEntry("かなめ"), suite) && suite.ToString() == "/要/");

    // 逆引き
    assert(backend.ReverseLookup("漢字") == "かんじ");

    // 削除
    backend.Remove(SKKEntry("あr", "り"), SKKCandidate("有"));
    backend.Remove(SKKEntry("かなめ"), SKKCandidate("要"));

    // 補完
    assert(!backend.Complete("かなめ", result));

    // 検索
    assert(!backend.Find(SKKEntry("あr", "り"), suite));
    assert(!backend.Find(SKKEntry("かなめ"), suite));

    // skk-ignore-dic-word 対応
    assert(backend.Find(SKKEntry("おおk", "き"), suite) && suite.ToString() == "/大/");
    assert(backend.Find(SKKEntry("むし"), suite) && suite.ToString() == "/蒸し/虫/");
}
