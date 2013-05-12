// -*- C++ -*-

#ifndef MockRegistrationObserver_h
#define MockRegistrationObserver_h

#include "SKKRegistrationObserver.h"

class MockRegistrationObserver : public SKKRegistrationObserver {
    virtual void SKKRegistrationBegin(SKKBaseEditor* bottom) { delete bottom; }
    virtual void SKKRegistrationFinish(const std::string& word) {}
    virtual void SKKRegistrationCancel() {}
};

#endif
