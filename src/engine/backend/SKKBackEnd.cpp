/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008-2010 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#include <iostream>
#include <set>
#include "SKKBackEnd.h"
#include "SKKLocalUserDictionary.h"
#include "SKKCandidateSuite.h"
#include "SKKNumericConverter.h"
#include "SKKCandidateFilter.h"
#include "utf8util.h"

namespace {
    // 検索用ファンクタ
    class ApplyFind {
        SKKEntry entry_;
        SKKCandidateSuite* result_;

    public:
        ApplyFind(const SKKEntry& entry, SKKCandidateSuite& result)
            : entry_(entry), result_(&result) {}

        void operator()(SKKBaseDictionary* dict) const {
            dict->Find(entry_, *result_);
        }
    };

    // 補完用ファンクタ
    class ApplyComplete {
        SKKCompletionHelper* helper_;

    public:
        ApplyComplete(SKKCompletionHelper& helper) : helper_(&helper) {}

        void operator()(SKKBaseDictionary* dict) const {
            dict->Complete(*helper_);
        }
    };

    // 数値変換用ファンクタ
    class NumericConversion {
        SKKNumericConverter* converter_;

    public:
        NumericConversion(SKKNumericConverter& converter)
            : converter_(&converter) {}

        SKKCandidate& operator()(SKKCandidate& candidate) const {
            converter_->Apply(candidate);
            return candidate;
        }
    };

    // 補完ヘルパー
    class CompletionHelper : public SKKCompletionHelper {
        std::set<std::string> check_;
        std::vector<std::string> result_;
        std::string entry_;
        unsigned minimumLength_;
        unsigned completionLimit_;
        bool needsLengthCheck_;
        
    public:
        CompletionHelper(const std::string& entry, int minimumLength, int completionLimit)
            : entry_(entry), minimumLength_(minimumLength), completionLimit_(completionLimit) {
            needsLengthCheck_ = utf8::length(entry) < minimumLength_;
            check_.insert(entry_);
        }

        virtual const std::string& Entry() const {
            return entry_;
        }

        virtual void Add(const std::string& completion) {
            if(!CanContinue()) return;

            if(needsLengthCheck_ && utf8::length(completion) <= minimumLength_) {
                return;
            }

            if(check_.find(completion) == check_.end()) {
                check_.insert(completion);
                result_.push_back(completion);
            }
        }

        virtual bool CanContinue() const {
            return completionLimit_ == 0 || result_.size() < completionLimit_;
        }

        operator std::vector<std::string>&() {
            return result_;
        }
    };
}

SKKBackEnd::SKKBackEnd()
    : userdict_(0)
    , useNumericConversion_(false)
    , enableExtendedCompletion_(false)
    , minimumCompletionLength_(0)
{}

SKKBackEnd& SKKBackEnd::theInstance() {
    static SKKBackEnd obj;
    return obj;
}

void SKKBackEnd::Initialize(const std::string& userdict_path, const SKKDictionaryKeyContainer& keys) {
    if(userdict_.get() == 0) {
        userdict_.reset(new SKKLocalUserDictionary());
    }

    userdict_->Initialize(userdict_path);

    // 不要な辞書を破棄する
    for(unsigned i = 0; i < actives_.size(); ++ i) {
	if(std::find(keys.begin(), keys.end(), actives_[i]) == keys.end()) {
	    cache_.Clear(actives_[i]);
	}
    }

    // 辞書を初期化する
    dicts_.clear();
    dicts_.push_back(userdict_.get());
    for(unsigned i = 0; i < keys.size(); ++ i) {
	dicts_.push_back(cache_.Get(keys[i]));
    }

    actives_ = keys;
}

void SKKBackEnd::Initialize(SKKUserDictionary* dictionary, const SKKDictionaryKeyContainer& keys) {
    userdict_.reset(dictionary);

    // 不要な辞書を破棄する
    for(unsigned i = 0; i < actives_.size(); ++ i) {
	if(std::find(keys.begin(), keys.end(), actives_[i]) == keys.end()) {
	    cache_.Clear(actives_[i]);
	}
    }

    // 辞書を初期化する
    dicts_.clear();
    dicts_.push_back(userdict_.get());
    for(unsigned i = 0; i < keys.size(); ++ i) {
	dicts_.push_back(cache_.Get(keys[i]));
    }

    actives_ = keys;
}

bool SKKBackEnd::Complete(const std::string& key, std::vector<std::string>& result, unsigned limit) {
    CompletionHelper helper(key, minimumCompletionLength_, limit);

    if(key.empty() || !enableExtendedCompletion_) {
        userdict_->Complete(helper);
    } else {
        std::for_each(dicts_.begin(), dicts_.end(), ApplyComplete(helper));
    }

    result = helper;

    return !result.empty();
}

bool SKKBackEnd::Find(const SKKEntry& entry, SKKCandidateSuite& result) {
    result.Clear();

    std::for_each(dicts_.begin(), dicts_.end(), ApplyFind(entry, result));

    if(!entry.IsOkuriAri()) {
        SKKNumericConverter converter;

        if(useNumericConversion_ && converter.Setup(entry.EntryString())) {
            SKKCandidateSuite suite;

            std::for_each(dicts_.begin(), dicts_.end(),
                          ApplyFind(converter.NormalizedKey(), suite));

            SKKCandidateContainer& cands = suite.Candidates();

            std::transform(cands.begin(), cands.end(),
                           std::back_inserter(result.Candidates()),
                           NumericConversion(converter));
        }

        result.Remove(SKKCandidate(converter.OriginalKey()));
    }

    result.RemoveIf(SKKIgnoreDicWord());

    return !result.IsEmpty();
}

std::string SKKBackEnd::ReverseLookup(const std::string& candidate) {
    if(candidate.empty()) return "";

    for(unsigned i = 0; i < dicts_.size(); ++ i) {
        std::string entry(dicts_[i]->ReverseLookup(candidate));

        if(!entry.empty()) {
            return entry;
        }
    }

    return "";
}

void SKKBackEnd::Register(const SKKEntry& entry, const SKKCandidate& candidate) {
    if(entry.EntryString().empty() ||
       (entry.IsOkuriAri() && (entry.OkuriString().empty() || candidate.IsEmpty()))) {
	std::cerr << "SKKBackEnd: Invalid registration received" << std::endl;
	return;
    }

    if(candidate.AvoidStudy()) {
        return;
    }

    userdict_->Register(normalize(entry), candidate);
}

void SKKBackEnd::Remove(const SKKEntry& entry, const SKKCandidate& candidate) {
    if(entry.EntryString().empty()) {
	std::cerr << "SKKBackEnd: Invalid removal received" << std::endl;
	return;
    }

    userdict_->Remove(normalize(entry), candidate);
}

void SKKBackEnd::UseNumericConversion(bool flag) {
    useNumericConversion_ = flag;
}

void SKKBackEnd::EnableExtendedCompletion(bool flag) {
    enableExtendedCompletion_ = flag;
}

void SKKBackEnd::EnablePrivateMode(bool flag) {
    userdict_->SetPrivateMode(flag);
}

void SKKBackEnd::SetMinimumCompletionLength(int length) {
    minimumCompletionLength_ = length;
}

// ----------------------------------------------------------------------

SKKEntry SKKBackEnd::normalize(const SKKEntry& entry) {
    // 送りありなら何もしない
    if(entry.IsOkuriAri()) {
        return entry;
    }

    SKKNumericConverter converter;
    SKKEntry result(entry);

    // 単語登録と削除時には、数値だけの見出し語を正規化しない
    if(useNumericConversion_ && converter.Setup(entry.EntryString())) {
        if(converter.NormalizedKey() != "#") {
            result.SetEntry(converter.NormalizedKey());
        }
    }

    return result;
}
