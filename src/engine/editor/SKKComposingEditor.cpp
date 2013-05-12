/* -*- mode: C++; coding: utf-8 -*-

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

#include "SKKComposingEditor.h"
#include "SKKInputContext.h"

SKKComposingEditor::SKKComposingEditor(SKKInputContext* context)
    : SKKBaseEditor(context) {}

void SKKComposingEditor::ReadContext() {
    composing_.Clear();

    if(context()->entry.IsEmpty()) {
        // 直接入力モードからの遷移
        composing_.Insert(context()->undo.Entry());
    } else {
        // 変換モードからの遷移なので、見出し語を復元する
        context()->entry.SetOkuri("", "");
        composing_.Insert(context()->entry.EntryString());
    }

    context()->dynamic_completion = true;
}

void SKKComposingEditor::WriteContext() {
    context()->output.SetMark();
    context()->output.Compose("▽" + composing_.String(), composing_.CursorPosition());

    update();
}

void SKKComposingEditor::Input(const std::string& ascii) {
    composing_.Insert(ascii);

    update();
}

void SKKComposingEditor::Input(const std::string& fixed, const std::string&, char) {
    Input(fixed);
}

void SKKComposingEditor::Input(SKKBaseEditor::Event event) {
    switch(event) {
    case BackSpace:
        if(composing_.IsEmpty()) {
            context()->needs_setback = true;
        }
        composing_.BackSpace();
        break;

    case Delete:
        composing_.Delete();
        break;

    case CursorLeft:
        composing_.CursorLeft();
        break;
        
    case CursorRight:
        composing_.CursorRight();
        break;
        
    case CursorUp:
        composing_.CursorUp();
        break;

    case CursorDown:
        composing_.CursorDown();
        break;
    }

    update();
}

void SKKComposingEditor::Commit(std::string& queue) {
    queue = composing_.String();
}

// ----------------------------------------------------------------------

void SKKComposingEditor::SetEntry(const std::string& entry) {
    composing_.Clear();
    composing_.Insert(entry);

    update();
}

// ----------------------------------------------------------------------

void SKKComposingEditor::update() {
    context()->entry = SKKEntry(composing_.LeftString());
}
