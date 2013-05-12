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

#include "SKKEntryRemoveEditor.h"
#include "SKKInputContext.h"
#include "SKKBackEnd.h"

SKKEntryRemoveEditor::SKKEntryRemoveEditor(SKKInputContext* context)
    : SKKBaseEditor(context) {}


void SKKEntryRemoveEditor::ReadContext() {
    entry_ = context()->entry;
    candidate_ = context()->candidate;

    input_.clear();

    prompt_ = entry_.EntryString() + " /"
        + candidate_.ToString() + "/ を削除しますか？(yes/no) ";
}

void SKKEntryRemoveEditor::WriteContext() {
    context()->output.Clear();
    context()->output.Compose(prompt_ + input_);

    context()->entry = entry_;
}

void SKKEntryRemoveEditor::Input(const std::string& ascii) {
    input_ += ascii;
}

void SKKEntryRemoveEditor::Input(const std::string& fixed, const std::string&, char) {
    Input(fixed);
}

void SKKEntryRemoveEditor::Input(Event event) {
    if(event == SKKBaseEditor::BackSpace && !input_.empty()) {
        input_.erase(input_.end() - 1);
    }
}

void SKKEntryRemoveEditor::Commit(std::string& queue) {
    if(input_ != "yes") {
        context()->needs_setback = true;
    } else {
        SKKBackEnd::theInstance().Remove(entry_, candidate_);
    }

    queue.clear();
}
