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

#include "MacDynamicCompletor.h"
#include "CompletionWindow.h"

#include <InputMethodKit/InputMethodKit.h>

MacDynamicCompletor::MacDynamicCompletor(SKKLayoutManager* layout) : layout_(layout) {
    window_ = [CompletionWindow sharedWindow];
}

void MacDynamicCompletor::Update(const std::string& completion,
                                 int commonPrefixLength, int cursorOffset) {
    completion_ = completion;
    commonPrefixLength_ = commonPrefixLength;
    cursorOffset_ = cursorOffset;
}

// ------------------------------------------------------------

void MacDynamicCompletor::SKKWidgetShow() {
    if(completion_.empty()) {
        SKKWidgetHide();
        return;
    }

    [window_ showCompletion:makeAttributedString()
             at:layout_->InputOrigin(cursorOffset_ + 1)
             level:layout_->WindowLevel()];
}

void MacDynamicCompletor::SKKWidgetHide() {
    [window_ hide];
}

NSAttributedString* MacDynamicCompletor::makeAttributedString() {
    NSDictionary* bold = [[NSDictionary dictionaryWithObject:[NSFont boldSystemFontOfSize:0.0]
                                                     forKey:NSFontAttributeName] retain];
    NSMutableAttributedString* result = [[NSMutableAttributedString alloc]
                                            initWithString:[NSString stringWithUTF8String:completion_.c_str()]];

    [result addAttribute:NSFontAttributeName
                   value:[NSFont systemFontOfSize:0.0] range:NSMakeRange(0, [result length])];

    NSRange diff = NSMakeRange(-1, 1);
    do {
        diff.location += commonPrefixLength_ + 1;

        if(diff.location < [result length]) {
            [result setAttributes:bold range:diff];
        }

        NSString* str = [result string];

        diff = [str rangeOfString:@"\n" options:0
                            range:NSMakeRange(diff.location, [str length] - diff.location)];
    } while(diff.location != NSNotFound);

    [bold release];

    return [result autorelease];
}
