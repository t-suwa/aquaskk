/* -*- ObjC -*-

  MacOS X implementation of the SKK input method.

  Copyright (C) 2007-2013 Tomotaka SUWA <tomotaka.suwa@gmail.com>

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

#ifndef SKKInputController_h
#define SKKInputController_h

#import <InputMethodKit/InputMethodKit.h>
#include "SKKInputMenu.h"
#include "SKKServerProxy.h"
#include "SKKInputMenu.h"

class SKKLayoutManager;
class SKKInputSession;
class MacInputModeWindow;

@interface SKKInputController : IMKInputController {
    id <IMKTextInput, NSObject> client_;
    NSTextInputContext* context_;
    BOOL activated_;

    SKKServerProxy* proxy_;
    SKKInputMenu* menu_;
    SKKLayoutManager* layout_;
    SKKInputSession* session_;
    MacInputModeWindow* modeIcon_;
}
@end

#endif
