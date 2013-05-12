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

#include "SKKInlineSelector.h"
#include <cassert>

void SKKInlineSelector::Initialize(SKKCandidateContainer& container, unsigned inlineCount) {
    range_.set(container, 0, inlineCount);
    pos_ = 0;
}

bool SKKInlineSelector::Next() {
    if(IsEmpty() || pos_ == maxPosition()) {
	return false;
    }

    ++ pos_;
    return true;
}

bool SKKInlineSelector::Prev() {
    if(IsEmpty() || pos_ == minPosition()) {
	return false;
    }

    -- pos_;
    return true;
}

const SKKCandidate& SKKInlineSelector::Current() const {
    assert(!IsEmpty());

    return range_[pos_];
}

bool SKKInlineSelector::IsEmpty() const {
    return range_.empty();
}

int SKKInlineSelector::minPosition() const {
    return 0;
}

int SKKInlineSelector::maxPosition() const {
    return range_.size() - 1;
}
