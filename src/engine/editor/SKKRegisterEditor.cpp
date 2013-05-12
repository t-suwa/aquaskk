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

#include "SKKRegisterEditor.h"
#include "SKKInputContext.h"

SKKRegisterEditor::SKKRegisterEditor(SKKInputContext* context)
    : SKKBaseEditor(context), entry_(context->entry) {
    prompt_ = "[登録：" + entry_.PromptString() + "]";
}

void SKKRegisterEditor::ReadContext() {
    context()->entry = SKKEntry();

    Input(context()->registration.Word());
    context()->registration.Clear();
}

void SKKRegisterEditor::WriteContext() {
    context()->output.Compose(prompt_ + word_.String(), word_.CursorPosition());
}

void SKKRegisterEditor::Input(const std::string& ascii) {
    word_.Insert(ascii);
}

void SKKRegisterEditor::Input(const std::string& fixed, const std::string&, char) {
    Input(fixed);
}

void SKKRegisterEditor::Input(SKKBaseEditor::Event event) {
    switch(event) {
    case BackSpace:
        word_.BackSpace();
        break;

    case Delete:
        word_.Delete();
        break;
        
    case CursorLeft:
        word_.CursorLeft();
        break;

    case CursorRight:
        word_.CursorRight();
        break;

    case CursorUp:
        word_.CursorUp();
        break;

    case CursorDown:
        word_.CursorDown();
        break;

    default:
        return;
    }
}

void SKKRegisterEditor::Commit(std::string& queue) {
    word_.Insert(queue);
    queue = word_.String();

    context()->entry = entry_;
}
