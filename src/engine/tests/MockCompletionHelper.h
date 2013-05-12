// -*- C++ -*-

#ifndef MockCompletionHelper_h
#define MockCompletionHelper_h

#include "SKKCompletionHelper.h"

class MockCompletionHelper : public SKKCompletionHelper {
    std::vector<std::string> result_;
    std::string entry_;

public:
    void Initialize(const std::string& entry) {
        entry_ = entry;
        result_.clear();
    }

    std::vector<std::string>& Result() {
        return result_;
    }

    virtual const std::string& Entry() const {
        return entry_;
    }

    virtual void Add(const std::string& completion) {
        result_.push_back(completion);
    }

    virtual bool CanContinue() const {
        return true;
    }
};

#endif
