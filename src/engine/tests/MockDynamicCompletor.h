// -*- C++ -*-

#ifndef MockDynamicCompletor_h
#define MockDynamicCompletor_h

#include "SKKDynamicCompletor.h"

class MockDynamicCompletor : public SKKDynamicCompletor {
    virtual void SKKWidgetShow() {}
    virtual void SKKWidgetHide() {}
    virtual void Update(const std::string& completion, int commonPrefixSize, int cursorOffset) {}
};

#endif
