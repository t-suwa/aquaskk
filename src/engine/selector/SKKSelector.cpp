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

#include "SKKSelector.h"
#include "SKKBackEnd.h"

SKKSelector::SKKSelector(SKKSelectorBuddy* buddy, SKKCandidateWindow* window)
    : buddy_(buddy), selector_(0), windowSelector_(window) {}

bool SKKSelector::IsInline() const {
    return selector_ == &inlineSelector_;
}

bool SKKSelector::Execute(int inlineCount) {
    SKKEntry entry(buddy_->SKKSelectorQueryEntry());

    suite_.Clear();

    SKKBackEnd::theInstance().Find(entry, suite_);

    inlineSelector_.Initialize(suite_.Candidates(), inlineCount);
    windowSelector_.Initialize(suite_.Candidates(), inlineCount);

    if(!suite_.IsEmpty()) {
	if(!inlineSelector_.IsEmpty()) {
	    selector_ = &inlineSelector_;
	} else {
	    selector_ = &windowSelector_;
	}

	notify();
    }

    return !suite_.IsEmpty();
}

bool SKKSelector::Next() {
    if(!selector_->Next()) {
	if(!IsInline() || windowSelector_.IsEmpty()) return false;

	selector_ = &windowSelector_;
	windowSelector_.Show();
    }

    notify();

    return true;
}

bool SKKSelector::Prev() {
    if(!selector_->Prev()) {
	if(IsInline() || inlineSelector_.IsEmpty()) return false;

	selector_ = &inlineSelector_;
	windowSelector_.Hide();
    }

    notify();

    return true;
}

void SKKSelector::CursorLeft() {
    selector_->CursorLeft();
    notify();
}

void SKKSelector::CursorRight() {
    selector_->CursorRight();
    notify();
}

void SKKSelector::CursorUp() {
    selector_->CursorUp();
    notify();
}

void SKKSelector::CursorDown() {
    selector_->CursorDown();
    notify();
}

bool SKKSelector::Select(char label) {
    if(IsInline()) return false;

    if(windowSelector_.Select(label)) {
	notify();
	return true;
    }

    return false;
}

void SKKSelector::Show() {
    if(IsInline()) return;

    windowSelector_.Show();
}

void SKKSelector::Hide() {
    if(IsInline()) return;

    windowSelector_.Hide();
}

void SKKSelector::notify() {
    if(suite_.IsEmpty()) return;

    buddy_->SKKSelectorUpdate(selector_->Current());
}
