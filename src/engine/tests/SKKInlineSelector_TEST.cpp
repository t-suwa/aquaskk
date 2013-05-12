#include <cassert>
#include "SKKInlineSelector.h"

int main() {
    SKKCandidateContainer container;
    SKKInlineSelector selector;
    
    container.push_back(SKKCandidate("候補1"));
    container.push_back(SKKCandidate("候補2"));
    container.push_back(SKKCandidate("候補3"));

    selector.Initialize(container, 3);

    assert(!selector.IsEmpty() && !selector.Prev() && selector.Current().ToString() == "候補1");
    assert(selector.Next() && selector.Current().ToString() == "候補2");
    assert(selector.Next() && !selector.Next() && selector.Current().ToString() == "候補3");
    assert(selector.Prev() && selector.Current().ToString() == "候補2");
}
