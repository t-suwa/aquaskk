#include "SKKInputSession.h"
#include "SKKKeymap.h"
#include "SKKRomanKanaConverter.h"
#include "SKKBackEnd.h"

#include "MockInputSessionParameter.h"
#include "TestData.h"

#include <ios>
#include <iostream>
#include <fstream>
#include <cassert>

class TestRunner {
    MockInputSessionParameter* param;
    SKKInputSession session;
    SKKKeymap map;
    TestData test;

    SKKEvent getEvent(TestEntry& entry) {
        TestEvent& input = entry.input;
        
        return map.Fetch(input.code, 0, input.mods);
    }

    void initialize() {
        SKKDictionaryKeyContainer keys;
        std::string userdict("skk.jisyo");
        std::ofstream ofs(userdict.c_str(), std::ios_base::trunc);
        ofs.close();

        SKKBackEnd::theInstance().Initialize(userdict, keys);

        SKKRomanKanaConverter::theInstance().Initialize("kana-rule.conf");

        map.Initialize("keymap.conf");

        session.AddInputModeListener(param->Listener());
    }

    void execute() {
        int total = 0;
        int success = 0;
        TestEntry entry;

        while(test >> entry) {
            ++ total;
            TestResult& actual = param->Result();

            actual.Clear();

            param->SetSelectedString(entry.input.selection);
            param->SetYankString(entry.input.yank);

            SKKEvent event = getEvent(entry);
            
            actual.ret = session.HandleEvent(event);

            if(actual != entry.expected) {
                std::cerr << std::endl;
                std::cerr << "*** test failed *** line=" << entry.line << std::endl;
                std::cerr << "\t" << event.dump() << std::endl;
                entry.expected.Dump("\texpected: ");
                        actual.Dump("\t  actual: ");
                std::cerr << std::endl;
            } else {
                ++ success;
            }
        }

        std::cerr << "success=" << success << " / "
                  << "total=" << total
                  << ", (" << (double)success / total * 100 << "%)"
                  << std::endl;
    }

public:
    TestRunner(const std::string& path)
        : param(new MockInputSessionParameter()), session(param) {
        initialize();
        test.Load(path);
    }

    void Run() {
        execute();
    }
};

int main() {
    TestRunner test("test.dat");

    test.Run();
}
