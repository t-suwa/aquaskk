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

#include "SKKOkuriEditor.h"
#include "SKKInputContext.h"
#include "utf8util.h"
#include <iostream>
#include <cctype>
#include <exception>

SKKOkuriEditor::SKKOkuriEditor(SKKInputContext* context, SKKOkuriListener* listener)
    : SKKBaseEditor(context), listener_(listener) {}

void SKKOkuriEditor::ReadContext() {
    first_ = true;

    prefix_.clear();
    okuri_.clear();
}

void SKKOkuriEditor::WriteContext() {
    context()->output.Compose("*" + okuri_);

    update();
}

void SKKOkuriEditor::Input(const std::string& fixed, const std::string& input, char code) {
    input_ = input;

    if(first_) {
        first_ = false;
        prefix_ += std::tolower(code);

        // KesSi 対応
        if(!fixed.empty() && !input.empty()) {
            listener_->SKKOkuriListenerAppendEntry(fixed);
            update();
            return;
        }
    }

    // fixed が ascii の場合には送りとはみなさない
    // 文字種で判断したいところだが、とりあえず長さで判断
    if(utf8::length(fixed) != fixed.size()) {
        okuri_ += fixed;
    }

    // OWsa 対応
    if(okuri_.empty()) {
        prefix_.clear();
        if(input.empty()) {
            if(code != 0) {
                prefix_ += std::tolower(code);
            }
        } else {
            prefix_ += std::tolower(input[0]);
        }
    } else {
        if(prefix_.empty() && code != 0) {
            prefix_ += std::tolower(code);
        }
    }

    update();
}

void SKKOkuriEditor::Input(SKKBaseEditor::Event event) {
    if(event == BackSpace) {
        if(okuri_.empty()) {
            context()->needs_setback = true;
        } else {
            utf8::pop(okuri_);
        }
    }

    update();
}

void SKKOkuriEditor::Commit(std::string&) {
    prefix_.clear();
    okuri_.clear();
}

bool SKKOkuriEditor::IsOkuriComplete() const {
    return !okuri_.empty() && input_.empty();
}

// ----------------------------------------------------------------------

void SKKOkuriEditor::update() {
    context()->entry.SetOkuri(prefix_, okuri_);
}
