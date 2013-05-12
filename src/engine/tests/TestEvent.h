// -*- C++ -*-

#ifndef TestEvent_h
#define TestEvent_h

// SKKInputSession テスト用の入力イベント
struct TestEvent {
    int code;
    int mods;
    std::string selection;
    std::string yank;

    TestEvent() : code(0), mods(0) {}
};

#endif
