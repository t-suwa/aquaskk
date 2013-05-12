/* -*- ObjC -*-

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

#ifndef SKKLayoutManager_h
#define SKKLayoutManager_h

#include <Foundation/Foundation.h>
#include <InputMethodKit/InputMethodKit.h>

class SKKLayoutManager {
    id client_;

    BOOL putUpward() const;
    NSRect inputFrame(int index) const;
    NSRect screenFrame(const NSRect& input) const;
    NSPoint fit(const NSRect& screen, const NSRect& window) const;
    int margin() const; 

public:
    SKKLayoutManager(id client);

    // 入力位置の原点
    NSPoint InputOrigin(int index = 0) const;

    // 各ウィンドウ向け原点
    NSPoint CandidateWindowOrigin() const;
    NSPoint AnnotationWindowOrigin(int mark) const;

    int WindowLevel() const;
};

#endif
