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

#include "SKKPrimaryEditor.h"
#include "SKKInputContext.h"

SKKPrimaryEditor::SKKPrimaryEditor(SKKInputContext* context)
    : SKKBaseEditor(context) {}

void SKKPrimaryEditor::ReadContext() {
    context()->entry = SKKEntry();

    SKKRegistration& registration = context()->registration;

    if(registration == SKKRegistration::Finished) {
        context()->output.Fix(registration.Word());
        registration.Clear();
    }
}

void SKKPrimaryEditor::Input(const std::string& ascii) {
    context()->event_handled = false;
}

void SKKPrimaryEditor::Input(const std::string& fixed, const std::string&, char) {
    context()->output.Fix(fixed);
}

void SKKPrimaryEditor::Input(SKKBaseEditor::Event event) {
    context()->event_handled = false;
}

void SKKPrimaryEditor::Commit(std::string& queue) {
    context()->output.Fix(queue);
    queue.clear();

    context()->entry = SKKEntry();
}
