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

#ifndef SKKUndoContext_h
#define SKKUndoContext_h

#include <string>
 
class SKKFrontEnd;

class SKKUndoContext {
    SKKFrontEnd* frontend_;
    std::string entry_;
    std::string candidate_;

public:
    SKKUndoContext(SKKFrontEnd* frontend);

    enum UndoResult { UndoFailed, UndoKanaEntry, UndoAsciiEntry };
    UndoResult Undo();
    bool IsActive() const;
    void Clear();
    const std::string& Entry() const;
    const std::string& Candidate() const;
};

#endif
