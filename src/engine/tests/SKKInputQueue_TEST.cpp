#include <cassert>
#include <iostream>
#include "SKKInputQueue.h"
#include "SKKRomanKanaConverter.h"

class TestInputQueueObserver : public SKKInputQueueObserver {
    State state_;

public:
    virtual void SKKInputQueueUpdate(const State& state) {
        state_.fixed += state.fixed;
        state_.queue = state.queue;
    }

    void Clear() {
        state_ = State();
    }

    bool Test(const std::string& fixed, const std::string& queue) {
        return state_.fixed == fixed && state_.queue == queue;
    }

    void Dump() {
        std::cerr << "fixed=" << state_.fixed << ", queue=" << state_.queue << std::endl;
    }
};

int main() {
    auto& converter = SKKRomanKanaConverter::theInstance();
    converter.Initialize("kana-rule.conf");

    TestInputQueueObserver observer;
    SKKInputQueue queue(&observer);

    queue.AddChar('a');
    assert(observer.Test("あ", ""));

    observer.Clear();
    queue.AddChar('k');
    assert(observer.Test("", "k"));
    queue.AddChar('y');
    assert(observer.Test("", "ky"));
    queue.RemoveChar();
    assert(observer.Test("", "k"));
    queue.AddChar('i');
    assert(observer.Test("き", ""));

    observer.Clear();
    queue.AddChar('n');
    assert(observer.Test("", "n"));
    queue.Terminate();
    assert(observer.Test("ん", ""));

    queue.AddChar('n');
    assert(queue.CanConvert('i'));

    queue.Terminate();
    observer.Clear();
    queue.AddChar('o');
    queue.AddChar('w');
    queue.AddChar('s');
    queue.AddChar('a');
    assert(observer.Test("おさ", ""));
}
