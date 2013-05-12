/*

  MacOS X implementation of the SKK input method.

  Copyright (C) 2009 Tomotaka SUWA <t.suwa@mac.com>

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

#include <stdexcept>
#include <algorithm>
#include <ctime>
#include "calculator.h"
#include "SKKCandidate.h"
#include "SKKCandidateSuite.h"
#include "SKKGadgetDictionary.h"

namespace {
    // ======================================================================
    // 実行変換ハンドラー
    // ======================================================================

    // 日付時刻生成
    tm current_datetime() {
        time_t current;
        tm result;

        time(&current);
        localtime_r(&current, &result);

        return result;
    }

    // 日付文字列整形
    std::string format_date(const char* format, const tm& tm) {
        char buf[64];

        strftime(buf, sizeof(buf), format, &tm);

        return buf;
    }

    // 現在日付
    void today(const std::string& entry, std::vector<std::string>& result) {
        tm now = current_datetime();
        const char* weekday[] = { "日", "月", "火", "水", "木", "金", "土" };

        result.push_back(format_date("%Y/%m/%d(%a)", now));
        result.push_back(format_date("%Y 年 %m 月 %d 日(", now) + weekday[now.tm_wday] + ")");
    }

    // 現在時刻
    void now(const std::string& entry, std::vector<std::string>& result) {
        tm now = current_datetime();

        result.push_back(format_date("%H:%M:%S", now));
        result.push_back(format_date("%H 時 %M 分 %S 秒", now));
    }

    // 元号変換
    //
    // jdate:yyyy
    //
    void jdate(const std::string& entry, std::vector<std::string>& result) {
    }

    // 簡易計算
    //
    // =(32768+64)*1024
    //
    void calculate(const std::string& entry, std::vector<std::string>& result) {
        calculator::engine calc;
        std::ostringstream buf;

        try {
            buf << calc.run(entry.substr(1));
            result.push_back(buf.str());
        } catch(const std::logic_error& ex) {
            result.push_back(ex.what());
        } catch(...) {
        }
    }
}

// ハンドラー選択ファンクタ
struct SKKGadgetDictionary::Match {
    std::string entry_;

    Match(const std::string& entry) : entry_(entry) {}

    bool operator()(const DispatchPair& pair) const {
        return entry_.find(pair.first) == 0;
    }
};

// 検索ファンクタ
struct SKKGadgetDictionary::Search {
    std::string entry_;
    std::vector<std::string>* result_;

    Search(const std::string& entry, std::vector<std::string>& result)
        : entry_(entry), result_(&result) {}

    void operator()(const DispatchPair& pair) const {
        pair.second(entry_, *result_);
    }
};

// ハンドラー補完ファンクタ
struct SKKGadgetDictionary::Comp {
    std::string entry_;

    Comp(const std::string& entry) : entry_(entry) {}

    bool operator()(const DispatchPair& pair) const {
        return entry_.compare(0, entry_.size(), pair.first,
                              0, entry_.size()) == 0;
    }
};

// 保存ファンクタ
struct SKKGadgetDictionary::Store {
    SKKCompletionHelper* helper_;

    Store(SKKCompletionHelper& helper) : helper_(&helper) {}

    void operator()(const DispatchPair& pair) const {
        helper_->Add(pair.first);
    }
};

void SKKGadgetDictionary::Initialize(const std::string& location) {
    table_.push_back(std::make_pair("today", today));
    table_.push_back(std::make_pair("now", now));
    table_.push_back(std::make_pair("jdate:", jdate));
    table_.push_back(std::make_pair("=", calculate));
}

void SKKGadgetDictionary::Find(const SKKEntry& entry, SKKCandidateSuite& result) {
    // 今のところ「送りあり」のサポートはなし
    if(entry.IsOkuriAri()) return;
    
    std::vector<std::string> tmp;
    const std::string& key = entry.EntryString();

    apply(Match(key), Search(key, tmp));

    if(tmp.empty()) return;

    for(unsigned index = 0; index < tmp.size(); ++ index) {
        SKKCandidate cand(tmp[index]);
        cand.SetAvoidStudy();
        result.Add(cand);
    }
}

void SKKGadgetDictionary::Complete(SKKCompletionHelper& helper) {
    apply(Comp(helper.Entry()), Store(helper));
}
