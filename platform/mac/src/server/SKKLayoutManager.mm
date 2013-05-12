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

#include <InputMethodKit/InputMethodKit.h>
#include "SKKConstVars.h"
#include "SKKLayoutManager.h"
#include "CandidateWindow.h"
#include "AnnotationWindow.h"

SKKLayoutManager::SKKLayoutManager(id client)
    : client_(client) {}

// 入力位置の原点
NSPoint SKKLayoutManager::InputOrigin(int index) const {
    NSRect frame = inputFrame(index);

    return frame.origin;
}

// 候補ウィンドウ原点
NSPoint SKKLayoutManager::CandidateWindowOrigin() const {
    NSRect input = inputFrame(0);
    NSRect candidate = [[[CandidateWindow sharedWindow] window] frame];
    NSRect screen = screenFrame(input);
    NSPoint pt = candidate.origin = input.origin;
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    bool candidateIsUpward = [defaults boolForKey:SKKUserDefaultKeys::put_candidate_window_upward] == YES;

    // 右端
    pt = fit(screen, candidate);

    if(candidateIsUpward) {
        pt.y = NSMaxY(input) + margin();

        // 上端
        if(NSHeight(screen) < pt.y + NSHeight(candidate)) {
            pt.y = NSMinY(input) - NSHeight(candidate) - margin();
        }
    } else {
        pt.y -= NSHeight(candidate) + margin();

        // 下端
        if(pt.y < NSMinY(screen)) {
            pt.y = NSMaxY(input) + margin();
        }
    }

    return pt;
}

// アノテーションウィンドウ原点
NSPoint SKKLayoutManager::AnnotationWindowOrigin(int mark) const {
    NSRect input = inputFrame(mark);
    NSRect annotation = [[[AnnotationWindow sharedWindow] window] frame];
    NSRect screen = screenFrame(input);
    NSPoint pt = annotation.origin = input.origin;
    NSWindow* candwindow = [[CandidateWindow sharedWindow] window];
    NSRect candidate = [candwindow frame];
    bool candidateIsVisible = [candwindow isVisible];
    bool candidateIsUpward = (input.origin.y < candidate.origin.y);

    // 右端
    pt = fit(screen, annotation);

    // アノテーションは常に下に表示する
    pt.y -= NSHeight(annotation) + margin();
    if(candidateIsVisible && !candidateIsUpward) {
        pt.y -= NSHeight(candidate) + margin();
    }

    // 下端
    if(!NSPointInRect(pt, screen)) {
        pt.y = NSMaxY(input) + margin();
        if(candidateIsVisible && candidateIsUpward) {
            pt.y += NSHeight(candidate) + margin();
        }
    }

    return pt;
}

int SKKLayoutManager::WindowLevel() const {
    id <IMKTextInput> obj = client_;
    
    return [obj windowLevel] + 1;
}

// ----------------------------------------------------------------------

// 入力エリア矩形
NSRect SKKLayoutManager::inputFrame(int index) const {
    NSRect frame = NSZeroRect;

    // attributesForCharacterIndex に未対応のクライアント(Twitter など)を考慮し、
    // 例外を補足する(例外発生時は、左下原点が使用される)
    @try {
        NSRect candidate = [[[CandidateWindow sharedWindow] window] frame];
        NSDictionary* dict = [[client_ attributesForCharacterIndex:index
                                               lineHeightRectangle:&frame] retain];
        if(dict) {
            NSFont* font = [dict objectForKey:NSFontAttributeName];

            frame.size.height = font ? NSHeight([font boundingRectForFont]) : NSHeight(candidate);

            [dict release];
        }
    }

    @catch(NSException* exception) {
        NSLog(@"%@", exception);
    }

    return frame;
}

// メニューバーを除いたスクリーン矩形
NSRect SKKLayoutManager::screenFrame(const NSRect& input) const {
    // カーソルを含むスクリーンを探す
    NSEnumerator* enumerator = [[NSScreen screens] objectEnumerator];
    while(NSScreen* screen = [enumerator nextObject]) {
        NSRect whole = [screen frame];
        NSRect frame = [screen visibleFrame];

        frame.origin = whole.origin;
        frame.size.width = NSWidth(whole);

        if(NSContainsRect(frame, input)) {
            return frame;
        }
    }

    // 念の為
    return [[NSScreen mainScreen] frame];
}

// 右端調整
NSPoint SKKLayoutManager::fit(const NSRect& screen, const NSRect& window) const {
    NSPoint pt = window.origin;

    if(NSMaxX(screen) < NSMaxX(window)) {
        pt.x = NSMaxX(screen) - NSWidth(window);
    }

    return pt;
}

// ウィンドウ間のマージン
int SKKLayoutManager::margin() const {
    return 1;
}
