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

#ifndef SKKTextBuffer_h
#define SKKTextBuffer_h

#include <string>

// カーソル移動をサポートするテキストバッファ
class SKKTextBuffer {
    std::string buf_;
    int cursor_;

    int minCursorPosition() const;
    int maxCursorPosition() const;

public:
    SKKTextBuffer();

    void Insert(const std::string& str);
    void BackSpace();
    void Delete();
    void Clear();

    void CursorLeft();
    void CursorRight();
    void CursorUp();
    void CursorDown();

    int CursorPosition() const;

    bool IsEmpty() const;

    bool operator==(const std::string& str) const;

    std::string String() const;
    std::string LeftString() const;
    std::string RightString() const;
};

#endif
