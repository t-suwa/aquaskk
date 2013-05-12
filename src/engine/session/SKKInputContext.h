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

#ifndef SKKInputContext_h
#define SKKInputContext_h

#include "SKKOutputBuffer.h"
#include "SKKEntry.h"
#include "SKKCandidate.h"
#include "SKKUndoContext.h"
#include "SKKRegistration.h"

// 入力コンテキスト
class SKKInputContext {
public:
    SKKEntry entry;
    SKKCandidate candidate;
    SKKOutputBuffer output;
    SKKUndoContext undo;
    SKKRegistration registration;

    bool event_handled;
    bool needs_setback;
    bool dynamic_completion;
    bool annotation;

    SKKInputContext(SKKFrontEnd* frontend)
        : output(frontend), undo(frontend), dynamic_completion(false) {}
};

#endif
