/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2008-2009 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef SKKBaseEditor_h
#define SKKBaseEditor_h

#include <string>

class SKKInputContext;

// 基底エディタクラス
class SKKBaseEditor {
    SKKInputContext* context_;

protected:
    SKKBaseEditor(SKKInputContext* context) : context_(context) {}

    SKKInputContext* context() const { return context_; }

public:
    virtual ~SKKBaseEditor() {}

    // SKKInputContext の情報で初期化
    //
    // 具象エディタは SKKInputContext の情報で初期化されることを期待される
    // SKKInputContext への書き込みも許可
    virtual void ReadContext() {}

    // SKKInputContext に書き出し
    //
    // 出力文字列や、状態設定等を行う
    virtual void WriteContext() {}

    // 入力処理(ASCII もしくはペースト用)
    virtual void Input(const std::string& ascii) {}

    // 入力処理(fixed=確定文字列, input=入力文字列, code=入力文字)
    virtual void Input(const std::string& fixed, const std::string& input, char code) {}

    enum Event {
        BackSpace, Delete, CursorLeft, CursorRight, CursorUp, CursorDown
    };

    // 入力処理(event=イベント)
    virtual void Input(Event event) {}

    // 確定処理
    //
    // queue に確定した文字列をセットする
    virtual void Commit(std::string& queue) = 0;
};

#endif
