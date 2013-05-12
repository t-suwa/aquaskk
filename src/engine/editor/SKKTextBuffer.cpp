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

#include "SKKTextBuffer.h"
#include "utf8util.h"

SKKTextBuffer::SKKTextBuffer() : cursor_(0) {}

void SKKTextBuffer::Insert(const std::string& str) {
    utf8::push(buf_, str, cursor_);
}

void SKKTextBuffer::BackSpace() {
    if(cursor_ != minCursorPosition()) {
	utf8::pop(buf_, cursor_);
    }
}

void SKKTextBuffer::Delete() {
    if(cursor_ != maxCursorPosition()) {
	CursorRight();
	BackSpace();
    }
}

void SKKTextBuffer::Clear() {
    buf_.clear();
    cursor_ = 0;
}

void SKKTextBuffer::CursorLeft() {
    if(cursor_ != minCursorPosition()) {
	-- cursor_;
    }
}

void SKKTextBuffer::CursorRight() {
    if(cursor_ != maxCursorPosition()) {
	++ cursor_;
    }
}

void SKKTextBuffer::CursorUp() {
    cursor_ = minCursorPosition();
}

void SKKTextBuffer::CursorDown() {
    cursor_ = maxCursorPosition();
}

int SKKTextBuffer::CursorPosition() const {
    return cursor_;
}

bool SKKTextBuffer::IsEmpty() const {
    return buf_.empty();
}

bool SKKTextBuffer::operator==(const std::string& str) const {
    return buf_ == str;
}

std::string SKKTextBuffer::String() const {
    return buf_;
}

std::string SKKTextBuffer::LeftString() const {
    return utf8::left(buf_, cursor_);
}

std::string SKKTextBuffer::RightString() const {
    return utf8::right(buf_, cursor_);
}

int SKKTextBuffer::minCursorPosition() const {
    // 今のところ毎回計算する(最適化しない)
    return - utf8::length(buf_);
}

int SKKTextBuffer::maxCursorPosition() const {
    return 0;
}
