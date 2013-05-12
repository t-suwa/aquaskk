/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2009 Tomotaka SUWA <t.suwa@mac.com>

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

#include "SKKOutputBuffer.h"
#include "SKKFrontEnd.h"
#include "utf8util.h"

SKKOutputBuffer::SKKOutputBuffer(SKKFrontEnd* frontend)
    : frontend_(frontend) {}
    
void SKKOutputBuffer::Fix(const std::string& str) {
    frontend_->InsertString(str);
}

void SKKOutputBuffer::Compose(const std::string& str, int cursor) {
    utf8::push(composing_, str, cursor_);
    cursor_ += cursor;
}

void SKKOutputBuffer::Convert(const std::string& str) {
    start_ = utf8::length(utf8::left(composing_, cursor_));
    length_ = utf8::length(str);

    Compose(str);
}

void SKKOutputBuffer::SetMark() {
    mark_ = utf8::length(composing_) + cursor_;
}

int SKKOutputBuffer::GetMark() const {
    return mark_;
}

void SKKOutputBuffer::Clear() {
    composing_.clear();
    cursor_ = mark_ = start_ = length_ = 0;
}

void SKKOutputBuffer::Output() {
    if(composing_ == last_ && composing_.empty()) {
        return;
    }

    if(length_) {
        frontend_->ComposeString(composing_, start_, length_);
    } else {
        frontend_->ComposeString(composing_, cursor_);
    }
    
    last_ = composing_;
}

bool SKKOutputBuffer::IsComposing() const {
    return !composing_.empty();
}
