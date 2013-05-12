// -*- C++ -*-

#ifndef MockClipboard_h
#define MockClipboard_h

#include "SKKClipboard.h"

class MockClipboard : public SKKClipboard {
    std::string str_;
    
    virtual const std::string PasteString() {
        return str_;
    }

public:
    void SetString(const std::string& str) {
        str_ = str;
    }
};

#endif
