#include <cassert>
#include <iostream>
#include "SKKKeymap.h"
#include "SKKKeymapEntry.h"
#include "SKKKeyState.h"

int main() {
    SKKKeymap keymap;
    SKKEvent param;

    keymap.Initialize("keymap.conf");

    assert(keymap.Fetch(0, 0, 0) == SKKEvent(SKK_CHAR, 0, 0));
    assert(keymap.Fetch('j', 0, SKKKeyState::CTRL) == SKKEvent(SKK_JMODE, 'j', 0));
    assert(keymap.Fetch(0x03, 0, 0) == SKKEvent(SKK_ENTER, 0x03, 0));
    assert(keymap.Fetch(0x09, 0, 0) == SKKEvent(SKK_TAB, 0x09, 0));
    assert(keymap.Fetch('i', 0, SKKKeyState::CTRL) == SKKEvent(SKK_TAB, 'i', 0));
    assert(keymap.Fetch('g', 0, SKKKeyState::CTRL) == SKKEvent(SKK_CANCEL, 'g', 0));
    assert(keymap.Fetch(0x1c, 0, 0) == SKKEvent(SKK_LEFT, 0x1c, 0));

    param = keymap.Fetch('b', 0, 0);
    assert(param == SKKEvent(SKK_CHAR, 'b', InputChars));

    param = keymap.Fetch('q', 0, 0);
    assert(param == SKKEvent(SKK_CHAR, 'q', ToggleKana | InputChars));

    param = keymap.Fetch('q', 0, SKKKeyState::CTRL);
    assert(param == SKKEvent(SKK_CHAR, 'q', ToggleJisx0201Kana));

    param = keymap.Fetch('A', 0, 0);
    assert(param == SKKEvent(SKK_CHAR, 'A', UpperCases | InputChars));

    param = keymap.Fetch('1', 0x51, 0);
    assert(param == SKKEvent(SKK_CHAR, '1', Direct));

    param = keymap.Fetch('f', 0, SKKKeyState::CTRL);
    assert(param == SKKEvent(SKK_RIGHT, 'f', 0));

    param = keymap.Fetch(0x20, 0, SKKKeyState::SHIFT);
    assert(param == SKKEvent(SKK_CHAR, 0x20, PrevCandidate|CompConversion));

    param = keymap.Fetch('v', 0, SKKKeyState::META);
    assert(param == SKKEvent(SKK_PASTE, 'v', 0));

    keymap.Patch("keymap_patch.conf");

    // not changed
    param = keymap.Fetch('b', 0, 0);
    assert(param == SKKEvent(SKK_CHAR, 'b', InputChars));

    // remove attributes
    param = keymap.Fetch('q', 0, 0);
    assert(param == SKKEvent(SKK_CHAR, 'q', InputChars));

    param = keymap.Fetch('"', 0, 0);
    assert(param == SKKEvent(SKK_CHAR, '"', UpperCases | InputChars));


}
