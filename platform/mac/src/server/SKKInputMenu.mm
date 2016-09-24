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

#include "SKKInputMenu.h"
#include "SKKEvent.h"

@implementation SKKInputMenu

namespace {
    const struct InputModeTable {
        int event_id;
        SKKInputMode mode;
        NSString* identifier;
    } table[] = {
        // InputMode
        { SKK_HIRAKANA_MODE, 		HirakanaInputMode,
          @"com.apple.inputmethod.Japanese.Hiragana" },
        { SKK_KATAKANA_MODE, 		KatakanaInputMode,
          @"com.apple.inputmethod.Japanese.Katakana" },
        { SKK_JISX0201KANA_MODE,	Jisx0201KanaInputMode,
          @"com.apple.inputmethod.Japanese.HalfWidthKana" },
        { SKK_JISX0208LATIN_MODE,	Jisx0208LatinInputMode,
          @"com.apple.inputmethod.Japanese.FullWidthRoman" },
        { SKK_ASCII_MODE,		AsciiInputMode,
          @"com.apple.inputmethod.Roman" },
        // InputSource
        { SKK_HIRAKANA_MODE, 		HirakanaInputMode,
          @"jp.sourceforge.inputmethod.aquaskk.Hiragana" },
        { SKK_KATAKANA_MODE, 		KatakanaInputMode,
          @"jp.sourceforge.inputmethod.aquaskk.Katakana" },
        { SKK_JISX0201KANA_MODE,	Jisx0201KanaInputMode,
          @"jp.sourceforge.inputmethod.aquaskk.HalfWidthKana" },
        { SKK_JISX0208LATIN_MODE,	Jisx0208LatinInputMode,
          @"jp.sourceforge.inputmethod.aquaskk.FullWidthRoman" },
        { SKK_ASCII_MODE,		AsciiInputMode,
          @"jp.sourceforge.inputmethod.aquaskk.Ascii" },
        // Error
        { SKK_NULL,			InvalidInputMode,
          0 }
    };

    const InputModeTable& findInputModeTable(NSString* identifier) {
        int i;

        for(i = 0; table[i].identifier != 0; ++ i) {
            if([identifier caseInsensitiveCompare:table[i].identifier] == 0) {
                return table[i];
            }
        }

        return table[i];
    }

    static SKKInputMode unifiedInputMode__ = HirakanaInputMode;
};

- (id)initWithClient:(id)client {
    if(self = [super init]) {
        client_ = client;
        activation_ = YES;
        currentInputMode_ = HirakanaInputMode;
    }

    return self;
}

- (void)updateMenu:(SKKInputMode)mode {
    NSString* identifer = [self convertInputModeToId:mode];

    if(identifer) {
        currentInputMode_ = mode;
        unifiedInputMode__ = mode;

        if(activation_) {
            [client_ selectInputMode:identifer];
        }
    }
}

- (NSString*)convertInputModeToId:(SKKInputMode)mode {
    for(int i = 0; table[i].identifier != 0; ++ i) {
        if(table[i].mode == mode) {
            return table[i].identifier;
        }
    }

    return 0;
}

- (int)convertIdToEventId:(NSString*)identifier {
    return findInputModeTable(identifier).event_id;
}

- (void)activation {
    activation_ = YES;
}
- (void)deactivation {
    activation_ = NO;
}

- (SKKInputMode)convertIdToInputMode:(NSString*)identifier {
    return findInputModeTable(identifier).mode;
}

- (SKKInputMode)currentInputMode {
    return currentInputMode_;
}

- (SKKInputMode)unifiedInputMode {
    return unifiedInputMode__;
}

@end
