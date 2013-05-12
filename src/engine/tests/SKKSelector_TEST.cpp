#include <cassert>
#include "SKKSelector.h"
#include "SKKCandidateWindow.h"
#include "SKKBackEnd.h"
#include "SKKCommonDictionary.h"
#include "SKKDictionaryFactory.h"

#include "MockFrontEnd.h"
#include "MockCandidateWindow.h"

class MockBuddy : public SKKSelectorBuddy {
    SKKCandidate candidate_;

    virtual const SKKEntry SKKSelectorQueryEntry() {
	return SKKEntry("かんじ");
    }

    virtual void SKKSelectorUpdate(const SKKCandidate& candidate) {
	candidate_ = candidate;
    }

public:
    SKKCandidate& Current() { return candidate_; }
};

int main() {
    MockCandidateWindow test_window;
    MockBuddy buddy;
    SKKSelector selector(&buddy, &test_window);
    SKKDictionaryKeyContainer dicts;

    SKKRegisterFactoryMethod<SKKCommonDictionary>(0);
    dicts.push_back(SKKDictionaryKey(0, "SKK-JISYO.TEST"));
    SKKBackEnd::theInstance().Initialize("skk-jisyo.utf8", dicts);

    assert(selector.Execute(3) && buddy.Current().ToString() == "漢字");
}
