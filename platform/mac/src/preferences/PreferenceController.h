/* -*- ObjC -*-

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

#ifndef PreferenceController_h
#define PreferenceController_h

#include <Cocoa/Cocoa.h>

@class SKKServerProxy;

@interface PreferenceController : NSObject {
    IBOutlet NSWindow* prefWindow_;
    IBOutlet NSPopUpButton* layoutPopUp_;
    IBOutlet NSButton* fontButton_;
    IBOutlet NSObjectController* objController_;
    IBOutlet NSArrayController* arrayController_;
    IBOutlet NSArrayController* blacklistArrayController_;
    IBOutlet NSTextField* version_;
    IBOutlet NSTextField* copyright_;
    IBOutlet NSArrayController* dictionaryTypes_;
    IBOutlet NSArrayController* subRuleController_;

    NSMutableDictionary* preferences_;
    NSMutableArray* blacklistApps_;
    NSMutableArray* dictionarySet_;
    NSMutableArray* layoutNames_;
    NSFont* candidateWindowFont_;
    SKKServerProxy* proxy_;
}

- (IBAction)showFontPanel:(id)sender;
- (IBAction)browseLocation:(id)sender;

@end

#endif
