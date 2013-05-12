// -*- C++ -*-

#ifndef MockConfig_h
#define MockConfig_h

#include "SKKConfig.h"

class MockConfig : public SKKConfig {
    virtual bool FixIntermediateConversion() { return true; }
    virtual bool EnableDynamicCompletion() { return false; }
    virtual int DynamicCompletionRange() { return 0; }
    virtual bool EnableAnnotation() { return false; }
    virtual bool DisplayShortestMatchOfKanaConversions() { return false; }
    virtual bool SuppressNewlineOnCommit() { return true; }
    virtual int MaxCountOfInlineCandidates() { return 5; }
    virtual bool HandleRecursiveEntryAsOkuri() { return false; }
    virtual bool InlineBackSpaceImpliesCommit() { return false; }
    virtual bool DeleteOkuriWhenQuit() { return true; }
};

#endif
