#include "SKKDictionaryLoader.h"
#include <iostream>

class DebugLoader : public SKKDictionaryLoader {
    bool state_;
    std::string path_;

    virtual bool NeedsUpdate() {
        return state_;
    }

    virtual const std::string& FilePath() const {
        return path_;
    }

public:
    void SetState(bool flag, const std::string& path) {
        state_ = flag;
        path_ = path;
    }

    virtual void Initialize(const std::string& location) {
        path_ = location;
    }

    virtual int Interval() const { return 1; }
    virtual int Timeout() const { return 1; }
};

class DebugObserver : public SKKDictionaryLoaderObserver {
    bool notified_;
    SKKDictionaryFile file_;

    virtual void SKKDictionaryLoaderUpdate(const SKKDictionaryFile& file) {
        file_ = file;
        notified_ = true;
    }

public:
    void Clear() {
        notified_ = false;
    }

    SKKDictionaryFile& File() {
        return file_;
    }

    bool IsNotified() const { return notified_; }
};

struct TestData {
    const char* comment;        // コメント
    bool state;                 // 更新が必要か
    std::string path;           // ファイルパス
    bool notified;              // 通知されたかどうか
    bool empty;                 // ファイルが空かどうか
};

void test(const char* msg, TestData* suite) {
    DebugLoader loader;
    DebugObserver observer;
    pthread::task& task = loader;

    loader.Connect(&observer);

    for(int index = 0; suite[index].comment != 0; ++ index) {
        TestData& data = suite[index];

        loader.SetState(data.state, data.path);
        observer.Clear();
        task.run();

        if(observer.IsNotified() != data.notified) {
            std::cerr << "notify test failed: " << msg << data.comment << std::endl;
        }

        if(observer.File().IsEmpty() != data.empty) {
            std::cerr << "empty test failed: " << msg << data.comment << std::endl;
        }
    }
}

int main() {
    TestData normal[] = {
        { "初期ロード",
          false, "SKK-JISYO.TEST",
          true, false },
        { "未更新",
          false, "SKK-JISYO.TEST",
          false, false },
        { "更新通知",
          true, "SKK-JISYO.TEST",
          true, false },

        { 0, false, "", false, false }
    };
    
    TestData download[] = {
        { "初期ロード",
          false, "/dev/null",
          true, true },
        { "空ファイル通知",
          false, "SKK-JISYO.TEST",
          true, true }, 
       { "更新通知",
          true, "SKK-JISYO.TEST",
          true, false },
        { "未更新",
          false, "SKK-JISYO.TEST",
          false, false },

        { 0, false, "", false, false }
    };

    test("正常パターン", normal);
    test("ダウンロードパターン", download);
}
