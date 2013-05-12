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

#include "CompletionView.h"

@interface CompletionView (Local)

- (NSRect)completionRect;
- (NSAttributedString*)newGuideWithString:(NSString*)string;

@end

@implementation CompletionView

- (id)init {
    self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)];
    if(self) {
        completion_ = nil;

        strokeColor_ = [[NSColor controlShadowColor] retain];
        backgroundColor_ = [[NSColor colorWithDeviceRed:1.0 green:1.0 blue:0.94 alpha:1.0] retain];

        guide_ = [self newGuideWithString:@"  TAB で補完  "];
        guideSize_ = [guide_ size];
        guideSize_.height += 2;
    }
    return self;
}

- (void)dealloc {
    if(completion_) {
        [completion_ release];
    }

    [strokeColor_ release];
    [backgroundColor_ release];
    [guide_ release];

    [super dealloc];
}

- (void)setCompletion:(NSAttributedString*)completion {
    if(completion_) {
        [completion_ release];
    }

    completion_ = [completion retain];

    [self setFrame:[self completionRect]];
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)rect {
    NSRect frame = [self frame];

    frame.origin.y += guideSize_.height;
    frame.size.height -= guideSize_.height;

    [backgroundColor_ setFill];
    NSRectFill(frame);

    [strokeColor_ set];
    NSFrameRect(frame);

    frame.origin.y = 0;
    frame.size.height = guideSize_.height;
    NSBezierPath* guidePlate = [NSBezierPath bezierPathWithRoundedRect:frame
                                             xRadius:NSHeight(frame) / 2
                                             yRadius:NSHeight(frame) / 2];
    frame.origin.y = frame.size.height = guideSize_.height / 2;
    [guidePlate appendBezierPathWithRect:frame];
    [guidePlate fill];

    NSPoint pt = NSMakePoint((NSWidth(frame) - guideSize_.width) / 2, 1);

    [guide_ drawAtPoint:pt];
    [completion_ drawAtPoint:NSMakePoint(3, guideSize_.height + 4)];
}

@end

@implementation CompletionView (Local)

- (NSRect)completionRect {
    NSRect rect;

    rect.origin = NSMakePoint(0, 0);
    rect.size = [completion_ size];
    rect.size.width = MAX(rect.size.width, guideSize_.width);
    rect.size.width += 8;
    rect.size.height += guideSize_.height + 8;

    return rect;
}

- (NSAttributedString*)newGuideWithString:(NSString*)string {
    NSMutableAttributedString* tmp = [[NSMutableAttributedString alloc] initWithString:string];
    NSRange range = NSMakeRange(0, [tmp length]);

    [tmp addAttribute:NSFontAttributeName
         value:[NSFont boldSystemFontOfSize:[NSFont labelFontSize]] range:range];

    [tmp addAttribute:NSForegroundColorAttributeName
         value:[NSColor whiteColor] range:range];

    [tmp addAttribute:NSBackgroundColorAttributeName
         value:strokeColor_ range:range];

    return tmp;
}

@end
