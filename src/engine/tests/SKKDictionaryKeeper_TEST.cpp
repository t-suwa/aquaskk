#include <cassert>
#include "SKKDictionaryKeeper.h"
#include "MockCompletionHelper.h"

class DebugLoader : public SKKDictionaryLoader {
    std::string path_;

    virtual bool NeedsUpdate() {
        return true;
    }

    virtual const std::string& FilePath() const {
        return path_;
    }

public:
    virtual void Initialize(const std::string& path) {
        path_ = path;
    }

    virtual int Interval() const { return 5; }

    virtual int Timeout() const { return 5; }
};

int main() {
    DebugLoader loader;
    SKKDictionaryKeeper keeper;
    MockCompletionHelper helper;

    loader.Initialize("SKK-JISYO.TEST");
    keeper.Initialize(&loader);

    assert(keeper.ReverseLookup("官寺") == "かんじ");

    helper.Initialize("か");
    keeper.Complete(helper);

    assert(helper.Result()[0] == "かんじ");
}
