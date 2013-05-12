/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008 Tomotaka SUWA <t.suwa@mac.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef SKKWindowSelector_h
#define SKKWindowSelector_h

#include <vector>
#include "SKKBaseSelector.h"

class SKKWindowSelector : public SKKBaseSelector {
    class SKKCandidateWindow* window_;

    typedef subrange<SKKBaseSelector::Range> PageRange;

    SKKBaseSelector::Range range_;
    PageRange view_;

    std::vector<int> pages_;
    int page_pos_;
    int cursor_pos_;
    int offset_;

    void refresh();
    int minPage() const;
    int maxPage() const;
    int minPosition() const;
    int maxPosition() const;

public:
    SKKWindowSelector(SKKCandidateWindow* window);

    void Initialize(SKKCandidateContainer& container, unsigned inlineCount);

    virtual bool Next();
    virtual bool Prev();

    virtual const SKKCandidate& Current() const;

    virtual bool IsEmpty() const;

    virtual void CursorLeft();
    virtual void CursorRight();
    virtual void CursorUp();
    virtual void CursorDown();

    bool Select(char label);

    void Show();
    void Hide();
};

#endif
