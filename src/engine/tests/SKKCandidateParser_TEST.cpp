#include <cassert>
#include "SKKCandidateParser.h"

int main() {
    SKKCandidateParser parser;

    parser.Parse("//");
    assert(parser.Candidates().empty() && parser.Hints().empty());

    parser.Parse("/候補1/");
    assert(parser.Candidates().size() == 1 && parser.Candidates()[0] == SKKCandidate("候補1"));

    parser.Parse("/候補1/候補2;アノテーション/候補3/");
    assert(parser.Candidates().size() == 3 && parser.Candidates()[1] == SKKCandidate("候補2"));

    parser.Parse("/候補1/[おくり/候補1/]/");
    SKKOkuriHint hint;
    hint.first = "おくり";
    hint.second.push_back(std::string("候補1"));
    assert(parser.Candidates().size() == 1 && parser.Hints().size() == 1 && parser.Hints()[0] == hint);

    parser.Parse("/候補;[]][アノテーション/");
    assert(parser.Candidates().size() == 1 && parser.Hints().empty());

    parser.Parse("//[]/[///]/[おくり/候補1/候補2/]//");
    assert(parser.Candidates().empty() && parser.Hints().size() == 1
	   && parser.Hints()[0].second[1] == SKKCandidate("候補2"));
}
