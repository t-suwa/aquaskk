#ifndef MockFrontEnd_h
#define MockFrontEnd_h

#include "SKKFrontEnd.h"
#include "SKKInputModeListener.h"
#include "TestResult.h"
#include <sstream>

class MockFrontEnd : public SKKFrontEnd, public SKKInputModeListener {
    TestResult result_;
    std::string selected_string_;

    virtual void InsertString(const std::string& str) {
	result_.fixed += str;
    }

    virtual void ComposeString(const std::string& str, int cursorOffset) {
	result_.marked = str;
        result_.pos = cursorOffset;
    }

    virtual void ComposeString(const std::string& str, int convertFrom, int convertTo) {
        result_.marked = str;
        result_.pos = 0;
    }

    virtual std::string SelectedString() {
        return selected_string_;
    }

    virtual void SelectInputMode(SKKInputMode mode) {
        result_.mode = mode;
    }

    virtual void SKKWidgetShow() {}
    virtual void SKKWidgetHide() {}

public:
    void SetSelectedString(const std::string& str) { selected_string_ = str; }
    operator TestResult&() { return result_; }
};

#endif
