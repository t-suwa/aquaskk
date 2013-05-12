/* -*- C++ -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2009 Tomotaka SUWA <t.suwa@mac.com>

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

#include "SKKUndoContext.h"
#include "SKKFrontEnd.h"
#include "SKKBackEnd.h"

SKKUndoContext::SKKUndoContext(SKKFrontEnd* frontend)
    : frontend_(frontend) {}

SKKUndoContext::UndoResult SKKUndoContext::Undo() {
    candidate_ = frontend_->SelectedString();

    // 逆引き
    entry_ = SKKBackEnd::theInstance().ReverseLookup(candidate_);

    if(entry_.empty()) {
        candidate_.clear();
        return UndoFailed;
    }

    // 表示不可能な文字が含まれるか？
    if(std::find_if(entry_.begin(), entry_.end(),
                    std::not1(std::ptr_fun(isprint))) != entry_.end()) {
        return UndoKanaEntry;
    }

    return UndoAsciiEntry;
}

bool SKKUndoContext::IsActive() const {
    return !entry_.empty();
}

void SKKUndoContext::Clear() {
    entry_.clear();
    candidate_.clear();
}

const std::string& SKKUndoContext::Entry() const {
    return entry_;
}

const std::string& SKKUndoContext::Candidate() const {
    return candidate_;
}
