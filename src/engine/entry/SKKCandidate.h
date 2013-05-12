/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2007-2008 Tomotaka SUWA <t.suwa@mac.com>

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

#ifndef SKKCandidate_h
#define SKKCandidate_h

#include <string>
#include <deque>

// 単一の変換候補
class SKKCandidate {
    std::string word_;
    std::string annotation_;
    std::string variant_;	// 数値変換用
    bool avoid_study_;          // 動的変換は学習しない

    void parse(const std::string& str) {
	std::string::size_type pos = str.find_first_of(';');

	if(pos != std::string::npos) {
	    annotation_ = str.substr(pos + 1);
	}

	word_ = str.substr(0, pos);
    }

public:
    SKKCandidate() : avoid_study_(false) {}

    SKKCandidate(const std::string& candidate, bool auto_parse = true) : avoid_study_(false) {
	if(auto_parse) {
            parse(candidate);
        } else {
            word_ = candidate;
        }
    }

    bool IsEmpty() const {
	return word_.empty();
    }

    const std::string& Word() const {
	return word_;
    }

    const std::string& Annotation() const {
	return annotation_;
    }

    const std::string& Variant() const {
	return (variant_.empty() ? Word() : variant_);
    }

    bool AvoidStudy() const {
        return avoid_study_;
    }

    void SetVariant(const std::string& str) {
	variant_ = str;
    }

    void SetAvoidStudy() {
        avoid_study_ = true;
    }

    std::string ToString() const {
	return word_ + (annotation_.empty() ? "" : (";" + annotation_));
    }

    bool operator==(const SKKCandidate& rhs) const {
	return Variant() == rhs.Variant(); // 注釈は比較しない
    }

    bool operator!=(const SKKCandidate& rhs) const {
	return !this->operator==(rhs);
    }

    // 候補のエンコードとデコード
    static std::string Encode(const std::string& src);
    static std::string Decode(const std::string& src);

    void Encode() {
        word_ = Encode(word_);
    }

    void Decode() {
        word_ = Decode(word_);
    }
};

typedef std::deque<SKKCandidate> SKKCandidateContainer;
typedef SKKCandidateContainer::iterator SKKCandidateIterator;

#endif
