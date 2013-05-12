// -*- C++ -*-

#ifndef TestData_h
#define TestData_h

#include "SKKKeyState.h"
#include "TestEvent.h"
#include "TestResult.h"
#include "stringutil.h"
#include <fstream>
#include <vector>

struct TestEntry {
    int line;
    TestEvent input;
    TestResult expected;
};

class TestData {
    std::vector<TestEntry> tests_;
    unsigned current_;

    std::string& decode(std::string& str) {
        string::translate(str, "%20", " ");
        return str;
    }

    TestEvent makeEvent(const std::string& event) {
        TestEvent result;
        string::splitter splitter;
        std::string key;
        std::string str;

        splitter.split(event);
        splitter >> key;

        while(splitter >> str) {
            string::splitter sub;
            sub.split(str, "=");

            std::string opt;
            std::string value;
            
            if(sub >> opt >> value) {
                if(opt == "sel") {
                    result.selection = value;
                }
                if(opt == "yank") {
                    result.yank = value;
                }
            }
        }

        splitter.split(key, "::");

        while(splitter >> str) {
            if(str == "shift") result.mods |= SKKKeyState::SHIFT;
            if(str == "ctrl") result.mods |= SKKKeyState::CTRL;
            if(str == "alt") result.mods |= SKKKeyState::ALT;
            if(str == "meta") result.mods |= SKKKeyState::META;
        }

        result.code = charcode(str);

        return result;
    }

    SKKInputMode inputMode(const std::string& mode) {
        if(mode == "J") return HirakanaInputMode;
        if(mode == "K") return KatakanaInputMode;
        if(mode == "Q") return Jisx0201KanaInputMode;
        if(mode == "A") return AsciiInputMode;
        if(mode == "L") return Jisx0208LatinInputMode;

        return InvalidInputMode;
    }

    char charcode(const std::string& str) {
        char result;
        std::istringstream buf(str);

        if(str.find("0x") == 0) {
            int tmp;
            buf >> std::hex >> tmp;
            result = tmp;
        } else {
            buf >> result;
        }

        return result;
    }

    int integer(const std::string& str) {
        int result;
        std::istringstream buf(str);

        buf >> result;

        return result;
    }

    TestResult makeTestResult(const std::string& expected) {
        string::splitter splitter;
        TestResult result;
        std::string str;

        splitter.split(expected, ",");

        while(splitter >> str) {
            string::splitter entry;
            std::string key;
            std::string value;

            entry.split(str, "=");
            if(entry >> key >> value) {
                if(key == "fixed") result.fixed = value;
                if(key == "marked") result.marked = decode(value);
                if(key == "mode") result.mode = inputMode(value);
                if(key == "pos") result.pos = integer(value);
                if(key == "ret") result.ret = (integer(value) == 1);
            }
        }

        return result;
    }

    TestEntry makeTestEntry(int line, const std::string& event, const std::string& expected) {
        TestEntry result;

        result.line = line;
        result.input = makeEvent(event);
        result.expected = makeTestResult(expected);

        return result;
    }

public:
    void Load(const std::string& path) {
        std::ifstream ifs(path.c_str());
        std::string line;
        string::splitter splitter;
        int num = 0;

        while(std::getline(ifs, line)) {
            ++ num;

            std::string event;
            std::string result;

            splitter.split(line, " ");
            splitter >> event >> result;

            if(event.empty() || event[0] == '#') continue;
            
            tests_.push_back(makeTestEntry(num, event, result));
        }

        current_ = 0;
    }

    bool operator>>(TestEntry& entry) {
        if(current_ < tests_.size()) {
            entry = tests_[current_ ++];
            return true;
        }

        return false;
    }
};

#endif
