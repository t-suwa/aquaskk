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

#ifndef SKKSelector_h
#define SKKSelector_h

#include "SKKInlineSelector.h"
#include "SKKWindowSelector.h"
#include "SKKEntry.h"
#include "SKKCandidateSuite.h"
#include "subrange.h"

// SKKSelector の相棒クラス
struct SKKSelectorBuddy {
    virtual ~SKKSelectorBuddy() {}

    // SKKSelector::Execute() 時に呼び出される
    virtual const SKKEntry SKKSelectorQueryEntry() = 0;

    // SKKSelector で現在選択中の候補が変更された場合に呼び出される
    virtual void SKKSelectorUpdate(const SKKCandidate& candidate) = 0;
};

class SKKCandidateWindow;

// 変換候補選択クラス
class SKKSelector {
    SKKSelectorBuddy* buddy_;
    SKKBaseSelector* selector_;
    SKKInlineSelector inlineSelector_;
    SKKWindowSelector windowSelector_;
    SKKCandidateSuite suite_;

    void notify();

public:
    SKKSelector(SKKSelectorBuddy* buddy, SKKCandidateWindow* window);

    // インラインかどうか
    bool IsInline() const;

    // 変換候補の検索
    bool Execute(int inlineCount);

    // 移動に成功すれば true
    bool Next();
    bool Prev();

    // カーソル移動による候補選択(インライン状態では no effect)
    void CursorLeft();
    void CursorRight();
    void CursorUp();
    void CursorDown();

    // 候補ラベルによる選択(インライン状態では常に false)
    bool Select(char label);

    // 候補ウィンドウの表示制御
    void Show();
    void Hide();
};

#endif
