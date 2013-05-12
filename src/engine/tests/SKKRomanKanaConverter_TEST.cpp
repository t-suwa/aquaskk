#include <cassert>
#include <iostream>
#include "SKKRomanKanaConverter.h"

void dump(const std::string& input, const SKKRomanKanaConversionResult& result) {
    std::cerr << "input=" << input
              << ",next=" << result.next
              << ",output=" << result.output
              << ",intermediate=" << result.intermediate
              << std::endl;
}

int main() {
    auto& conv = SKKRomanKanaConverter::theInstance();

    conv.Initialize("kana-rule.conf");

    bool result;

    SKKRomanKanaConversionResult state;

    result = conv.Convert(HirakanaInputMode, "a", state);
    assert(result && state.next == "" && state.output == "あ");

    result = conv.Convert(KatakanaInputMode, "a", state);
    assert(state.next == "" && state.output == "ア");

    result = conv.Convert(Jisx0201KanaInputMode, "a", state);
    assert(state.next == "" && state.output == "ｱ");

    result = conv.Convert(HirakanaInputMode, "gg", state);
    assert(state.next == "g" && state.output == "っ");

    result = conv.Convert(HirakanaInputMode, ",", state);
    assert(state.next == "" && state.output == "、");

    result = conv.Convert(HirakanaInputMode, "#", state);
    assert(state.next == "" && state.output == "＃");

    result = conv.Convert(HirakanaInputMode, " ", state);
    assert(state.next == "" && state.output == " ");

    result = conv.Convert(HirakanaInputMode, "kyl", state);
    assert(state.next == "" && state.output == "l");

    result = conv.Convert(HirakanaInputMode, "z,", state);
    assert(state.next == "" && state.output == "‥");

    result = conv.Convert(HirakanaInputMode, "co", state);
    assert(state.next == "" && state.output == "お");

    result = conv.Convert(HirakanaInputMode, "'", state);
    assert(state.next == "" && state.output == "'");

    result = conv.Convert(HirakanaInputMode, "k1", state);
    assert(state.next == "" && state.output == "1");

    result = conv.Convert(HirakanaInputMode, "kgya", state);
    assert(state.next == "" && state.output == "ぎゃ");

    result = conv.Convert(HirakanaInputMode, "k1234gya", state);
    assert(state.next == "" && state.output == "1234ぎゃ");

    result = conv.Convert(HirakanaInputMode, "gyagyugyo", state);
    assert(state.next == "" && state.output == "ぎゃぎゅぎょ");

    result = conv.Convert(HirakanaInputMode, "chho", state);
    assert(state.next == "" && state.output == "ほ");

    result = conv.Convert(HirakanaInputMode, "c", state);
    assert(state.next == "c" && state.output == "");

    result = conv.Convert(HirakanaInputMode, "pmp", state);
    assert(state.next == "p" && state.output == "");

    result = conv.Convert(HirakanaInputMode, "pmpo", state);
    assert(state.next == "" && state.output == "ぽ");

    result = conv.Convert(HirakanaInputMode, "kanji", state);
    assert(state.next == "" && state.output == "かんじ");

    result = conv.Convert(HirakanaInputMode, "/", state);
    assert(state.next == "" && state.output == "/");

    result = conv.Convert(HirakanaInputMode, "z ", state);
    assert(state.next == "" && state.output == "　");

    result = conv.Convert(HirakanaInputMode, "z\\", state);
    assert(state.next == "" && state.output == "￥");

    result = conv.Convert(HirakanaInputMode, ".", state);
    assert(state.output == "。");

    conv.Patch("period.rule");

    result = conv.Convert(HirakanaInputMode, ".", state);
    assert(state.output == "．");
}
