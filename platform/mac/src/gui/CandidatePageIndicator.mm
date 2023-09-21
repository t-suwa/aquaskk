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

#include "CandidatePageIndicator.h"

@implementation CandidatePageIndicator

- (void)prepareAttributes {
    attributes_ = [[NSMutableDictionary alloc] initWithCapacity:32];

    [attributes_ setObject:[NSFont boldSystemFontOfSize:[NSFont smallSystemFontSize]] forKey:NSFontAttributeName];
    [attributes_ setObject:[NSColor whiteColor] forKey:NSForegroundColorAttributeName];
}

- (void)preparePlate {
    indicator_ = [[NSAttributedString alloc]
		     initWithString:[NSString stringWithFormat:@"888 / 888"]
		     attributes:attributes_];

    float radius = 0.5 * [indicator_ size].height;

    NSRect rc;
    rc.origin = NSMakePoint(radius, 0);
    rc.size = [indicator_ size];

    NSPoint topleft = NSMakePoint(NSMinX(rc), NSMaxY(rc));
    NSPoint bottomright = NSMakePoint(NSMaxX(rc), NSMinY(rc));

    plate_ = [[NSBezierPath bezierPathWithRect:rc] retain];

    [plate_ moveToPoint:bottomright];
    [plate_ appendBezierPathWithArcWithCenter:NSMakePoint(bottomright.x, bottomright.y + radius)
	    radius:radius startAngle:270 endAngle:90];

    [plate_ moveToPoint:topleft];
    [plate_ appendBezierPathWithArcWithCenter:NSMakePoint(topleft.x, topleft.y - radius)
	    radius:radius startAngle:90 endAngle:270];
    
}

- (id)init {
    if(self = [super init]) {
	[self prepareAttributes];
	[self preparePlate];
    }

    return self;
}

- (void)dealloc {
    if(indicator_) {
	[indicator_ release];
    }
    [plate_ release];
    [attributes_ release];

    [super dealloc];
}

- (void)setPage:(NSRange)page {
    if(indicator_) [indicator_ release];

    indicator_ = [[NSAttributedString alloc]
		     initWithString:[NSString stringWithFormat:@"%3ld / %-3ld", page.location, page.length]
		     attributes:attributes_];
}

- (void)drawAtPoint:(NSPoint)pt {
    NSAffineTransform* xform = [NSAffineTransform transform];

    [xform translateXBy:pt.x yBy:pt.y];
    [xform concat];

    [[NSColor systemGrayColor] setFill];

    [plate_ fill];
    [indicator_ drawAtPoint:NSMakePoint(([plate_ bounds].size.width - [indicator_ size].width) / 2.0,
                                        ([plate_ bounds].size.height - [indicator_ size].height) / 2.0)];

    [xform invert];
    [xform concat];
}

- (NSSize)size {
    return [plate_ bounds].size;
}

@end
