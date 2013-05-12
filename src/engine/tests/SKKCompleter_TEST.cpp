#include <cassert>
#include "SKKCompleter.h"
#include "SKKBackEnd.h"

class TestBuddy : public SKKCompleterBuddy {
    std::string query_;

    virtual const std::string SKKCompleterQueryString() {
	return query_;
    }

    virtual void SKKCompleterUpdate(const std::string& entry) {
	query_ = entry;
    }

public:
    void SetQuery(const std::string& str) {
	query_ = str;
    }

    const std::string& Entry() const {
	return query_;
    }
};

int main() {
    TestBuddy buddy;
    SKKCompleter completer(&buddy);
    SKKDictionaryKeyContainer dicts;

    auto& backend = SKKBackEnd::theInstance();

    backend.Initialize("skk-jisyo.utf8", dicts);

    buddy.SetQuery("ほかん");

    assert(completer.Execute() && buddy.Entry() == "ほかん1");

    completer.Next();
    completer.Next();

    assert(buddy.Entry() == "ほかん3");

    backend.Register(SKKEntry("とぐるほかん"), SKKCandidate());

    buddy.SetQuery("とぐる");
    assert(completer.Execute() && buddy.Entry() == "とぐるほかん");

    backend.Remove(SKKEntry("とぐるほかん"), SKKCandidate());
    assert(!completer.Execute());
}
