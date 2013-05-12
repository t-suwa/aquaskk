#include <cassert>
#include <iostream>
#include <sstream>
#include "SKKTrie.h"

class DebugHelper : public SKKTrieHelper {
    SKKInputMode mode;
    std::string original;
    std::string string;
    std::string output;
    std::string intermediate;

    virtual const std::string& SKKTrieRomanString() const {
        return string;
    }

    virtual void SKKTrieNotifyConverted(const SKKTrie* node) {
        output += node->KanaString(mode);
    }

    virtual void SKKTrieNotifyNotConverted(char code) {
        output += code;
    }

    virtual void SKKTrieNotifyIntermediate(const SKKTrie* node) {
        intermediate += node->KanaString(mode);
    }

    virtual void SKKTrieNotifySkipLength(int length) {
        string = string.substr(length);
    }

    virtual void SKKTrieNotifyShort() {
    }

public:
    void Initialize(const std::string& str, SKKInputMode inputMode = HirakanaInputMode) {
        mode = inputMode;
        string = original = str;
        output.clear();
        intermediate.clear();
    }

    std::string Dump() {
        std::ostringstream buf;
        buf << "output=" << output
            << ",intermediate=" << intermediate
            << ",queue=" << string
            << ",original=" << original;
        return buf.str();
    }
};

int main() {
    SKKTrie root;
    DebugHelper helper;

    helper.Initialize("a");
    root.Traverse(helper);
    assert(helper.Dump() == "output=a,intermediate=,queue=,original=a");

    root.Add("a", SKKTrie("あ", "ア", "ｱ", ""));
    root.Add("kya", SKKTrie("きゃ", "キャ", "ｷｬ", ""));
    root.Add("ka",  SKKTrie("か", "カ", "ｶ", ""));
    root.Add("n", SKKTrie("ん", "ン", "ﾝ", ""));
    root.Add("nn", SKKTrie("ん", "ン", "ﾝ", ""));
    root.Add("xx", SKKTrie("っ", "ッ", "ｯ", "x"));

    helper.Initialize("m");
    root.Traverse(helper);
    assert(helper.Dump() == "output=m,intermediate=,queue=,original=m");

    helper.Initialize("a");
    root.Traverse(helper);
    assert(helper.Dump() == "output=あ,intermediate=,queue=,original=a");

    helper.Initialize("kya");
    root.Traverse(helper);
    assert(helper.Dump() == "output=きゃ,intermediate=,queue=,original=kya");

    helper.Initialize("ki");
    root.Traverse(helper);
    assert(helper.Dump() == "output=,intermediate=,queue=i,original=ki");

    helper.Initialize("ky");
    root.Traverse(helper);
    assert(helper.Dump() == "output=,intermediate=,queue=ky,original=ky");

    helper.Initialize("n");
    root.Traverse(helper);
    assert(helper.Dump() == "output=,intermediate=ん,queue=n,original=n");

    helper.Initialize("ng");
    root.Traverse(helper);
    assert(helper.Dump() == "output=ん,intermediate=,queue=g,original=ng");

    helper.Initialize("nn");
    root.Traverse(helper);
    assert(helper.Dump() == "output=ん,intermediate=,queue=,original=nn");

    helper.Initialize("xx");
    root.Traverse(helper);
    assert(helper.Dump() == "output=っ,intermediate=,queue=,original=xx");
}
