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

#ifndef SKKFrontEnd_h
#define SKKFrontEnd_h

#include <string>

// クライアントインタフェース
class SKKFrontEnd {
public:
    virtual ~SKKFrontEnd() {}

    // 確定文字入力
    virtual void InsertString(const std::string& str) = 0;

    // 未確定文字入力(カーソル位置は末尾からのオフセット)
    virtual void ComposeString(const std::string& str, int cursorOffset = 0) = 0;

    // 未確定文字入力
    // candidateStart=変換候補の開始位置, candidateLength=変換候補の長さ
    virtual void ComposeString(const std::string& str, int candidateStart, int candidateLength) = 0;

    // 選択中の文字列
    virtual std::string SelectedString() = 0;
};

#endif
