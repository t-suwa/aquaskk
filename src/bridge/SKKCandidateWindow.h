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

#ifndef SKKCandidateWindow_h
#define SKKCandidateWindow_h

#include <vector>
#include "SKKWidget.h"
#include "SKKCandidate.h"

class SKKCandidateWindow : public SKKWidget {
public:
    virtual ~SKKCandidateWindow() {}

    // 各ページ毎に表示可能な候補数を求める
    virtual void Setup(SKKCandidateIterator begin, SKKCandidateIterator end, std::vector<int>& pages) = 0;

    // 更新
    //
    // begin,end=候補の範囲
    // cursor=カーソル位置
    // page_pos=現在ページ位置
    // page_max=全ページ数
    virtual void Update(SKKCandidateIterator begin, SKKCandidateIterator end,
                        int cursor, int page_pos, int page_max) = 0;

    // 候補ラベルのインデックス取得(一致しない場合には -1)
    virtual int LabelIndex(char label) = 0;
};

#endif
