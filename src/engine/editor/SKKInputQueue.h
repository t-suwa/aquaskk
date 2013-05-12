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

#ifndef SKKInputQueue_h
#define SKKInputQueue_h

#include <string>
#include "SKKInputMode.h"

class SKKInputQueueObserver {
public:
    // 入力状態
    struct State {
        std::string fixed;        // 確定した文字
        std::string intermediate; // 最小マッチした文字
        std::string queue;        // 入力バッファ
        char code;                // 入力文字
    };

    virtual ~SKKInputQueueObserver() {}

    virtual void SKKInputQueueUpdate(const State& state) = 0;
};

class SKKInputQueue {
    SKKInputQueueObserver* observer_;
    SKKInputMode mode_;
    std::string queue_;

    SKKInputQueueObserver::State convert(char code, bool direct);
    SKKInputQueueObserver::State terminate();

public:
    SKKInputQueue(SKKInputQueueObserver* observer);

    // 入力モードを変更する
    void SelectInputMode(SKKInputMode mode);

    // 文字の追加と削除
    void AddChar(char code, bool direct = false);
    void RemoveChar();

    // 中間状態を確定させる(n → ん)
    void Terminate();

    void Clear();

    bool IsEmpty() const;
    const std::string& QueueString() const;

    // 変換可能かどうか
    bool CanConvert(char code) const;
};

#endif
