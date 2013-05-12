#include <cassert>
#include <iostream>
#include "SKKLocalUserDictionary.h"
#include "MockCompletionHelper.h"

int main() {
    SKKLocalUserDictionary dict;
    SKKCandidateSuite suite;

    dict.Initialize("skk-jisyo.utf8");

    dict.Find(SKKEntry("#"), suite);
    assert(suite.IsEmpty());

    dict.Find(SKKEntry("おくりあr", "り"), suite);
    assert(suite.ToString() == "/送り有/");

    suite.Clear();
    dict.Find(SKKEntry("かんじ"), suite);
    assert(suite.ToString() == "/漢字/");

    dict.Register(SKKEntry("おくりあr", "り"), SKKCandidate("送りあ"));
    dict.Register(SKKEntry("かんり"), SKKCandidate("管理"));

    suite.Clear();

    dict.Find(SKKEntry("おくりあr", "り"), suite);
    assert(suite.ToString() == "/送りあ/[り/送りあ/]/");

    suite.Clear();
    dict.Find(SKKEntry("かんり"), suite);
    assert(suite.ToString() == "/管理/");

    MockCompletionHelper helper;
    helper.Initialize("かん");
    dict.Complete(helper);
    assert(helper.Result()[0] == "かんり" && helper.Result()[1] == "かんじ");

    dict.Remove(SKKEntry("おくりあr", "り"), SKKCandidate("送りあ"));
    dict.Remove(SKKEntry("かんり"), SKKCandidate("管理"));

    suite.Clear();
    dict.Find(SKKEntry("おくりあr", "り"), suite);
    assert(suite.ToString() == "/送り有/");

    suite.Clear();
    dict.Find(SKKEntry("かんり"), suite);
    assert(suite.IsEmpty());

    dict.SetPrivateMode(true);

    suite.Clear();
    dict.Find(SKKEntry("おくりあr", "り"), suite);
    assert(suite.ToString() == "/送り有/");

    suite.Clear();
    dict.Find(SKKEntry("かんじ"), suite);
    assert(suite.ToString() == "/漢字/");

    dict.Register(SKKEntry("おくりあr", "り"), SKKCandidate("送りあ"));
    dict.Register(SKKEntry("かんり"), SKKCandidate("管理"));

    suite.Clear();
    dict.Find(SKKEntry("おくりあr", "り"), suite);
    assert(suite.ToString() == "/送りあ/[り/送りあ/]/");

    suite.Clear();
    dict.Find(SKKEntry("かんり"), suite);
    assert(suite.ToString() == "/管理/");

    dict.SetPrivateMode(false);

    suite.Clear();
    dict.Find(SKKEntry("おくりあr", "り"), suite);
    assert(suite.ToString() == "/送り有/");

    suite.Clear();
    dict.Find(SKKEntry("かんじ"), suite);
    assert(suite.ToString() == "/漢字/");

    assert(dict.ReverseLookup("漢字") == "かんじ");

    dict.Remove(SKKEntry("ほかん1"), SKKCandidate(""));
    suite.Clear();
    dict.Find(SKKEntry("ほかん1"), suite);
    assert(suite.ToString() == "/補完1/");

    dict.Register(SKKEntry("とぐるほかん"), SKKCandidate(""));
    suite.Clear();
    dict.Find(SKKEntry("とぐるほかん"), suite);
    assert(suite.IsEmpty());

    dict.Remove(SKKEntry("とぐるほかん"), SKKCandidate(""));
    suite.Clear();
    dict.Find(SKKEntry("とぐるほかん"), suite);
    assert(suite.IsEmpty());

    helper.Initialize("かんり");
    dict.Complete(helper);
    assert(helper.Result().empty());

    suite.Clear();
    dict.Register(SKKEntry("encode"), SKKCandidate("abc;def"));
    dict.Find(SKKEntry("encode"), suite);
    assert(suite.ToString() == "/abc;def/");

    suite.Clear();
    dict.Remove(SKKEntry("encode"), SKKCandidate("abc;def"));
    dict.Find(SKKEntry("encode"), suite);
    assert(suite.IsEmpty());

    dict.Register(SKKEntry("#"), SKKCandidate("456"));
}
