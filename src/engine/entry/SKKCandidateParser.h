/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2007 Tomotaka SUWA <t.suwa@mac.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef SKKCandidateParser_h
#define SKKCandidateParser_h

#include "SKKCandidate.h"
#include "SKKOkuriHint.h"
#include <algorithm>
#include <functional>

class SKKCandidateParser {
    SKKCandidateContainer candidates_;
    SKKOkuriHintContainer hints_;

    std::string tmp_candidate_;
    SKKOkuriHint tmp_hint_;

    typedef void (SKKCandidateParser::*Handler)(char ch);
    Handler handler_;

    // 送りヒントのパース
    void phase2(char ch) {
	if(tmp_candidate_.empty()) {
	    switch(ch) {
	    case '/': case '[':
		return;

	    case ']':
		if(!tmp_hint_.second.empty()) {
		    hints_.push_back(tmp_hint_);
		}
		reset();
		return;
	    }
	}

	if(ch == '/') {
	    if(tmp_hint_.first.empty()) {
		tmp_hint_.first = tmp_candidate_;
	    } else {
		tmp_hint_.second.push_back(tmp_candidate_);
	    }
	    tmp_candidate_.clear();
	    return;
	}

	tmp_candidate_ += ch;
    }

    // 通常候補のパース
    void phase1(char ch) {
	if(ch == '/') {
	    if(!tmp_candidate_.empty()) {
		candidates_.push_back(tmp_candidate_);
		tmp_candidate_.clear();
	    }
	    return;
	}

	if(ch == '[' && tmp_candidate_.empty()) {
	    handler_ = &SKKCandidateParser::phase2;
	    return;
	}

	tmp_candidate_ += ch;
    }

    void invoke(char ch) {
	(this->*handler_)(ch);
    }

    void reset() {
	tmp_candidate_.clear();
	tmp_hint_.first.clear();
	tmp_hint_.second.clear();
    }

public:
    void Parse(const std::string& str) {
	handler_ = &SKKCandidateParser::phase1;

	candidates_.clear();
	hints_.clear();

	reset();

	std::for_each(str.begin(), str.end(),
		      std::bind1st(std::mem_fun(&SKKCandidateParser::invoke), this));
    }

    const SKKCandidateContainer& Candidates() const {
	return candidates_;
    }

    const SKKOkuriHintContainer& Hints() const {
	return hints_;
    }
};

#endif
