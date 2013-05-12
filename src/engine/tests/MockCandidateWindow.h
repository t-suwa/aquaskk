#ifndef MockCandidateWindow_h
#define MockCandidateWindow_h

#include "SKKCandidateWindow.h"

class MockCandidateWindow : public SKKCandidateWindow {
    virtual void SKKWidgetShow() {}
    virtual void SKKWidgetHide() {}

public:
    virtual void Setup(SKKCandidateIterator begin,
                       SKKCandidateIterator end,
                       std::vector<int>& pages) {
	pages.push_back(end - begin);
    }

    virtual void Update(SKKCandidateIterator begin,
                        SKKCandidateIterator end,
                        int cursor, int page_pos, int page_max) {}

    virtual int LabelIndex(char label) { return 0; }
};

#endif
