#include <cassert>
#include <iostream>
#include "SKKEvent.h"
#include "SKKKeyState.h"
#include "SKKKeymapEntry.h"

int main() {
    SKKKeymapEntry entry;
    int key;

    entry = SKKKeymapEntry("Unknown", "a");
    assert(!(entry >> key));

    entry = SKKKeymapEntry("SKK_JMODE", "a");
    entry >> key;
    assert(key == SKKKeyState::CharCode('a', false));
    assert(entry.Symbol() == SKK_JMODE);
    assert(!(entry >> key));

    entry = SKKKeymapEntry("SKK_JMODE", "keycode::0x0a");
    entry >> key;
    assert(key == SKKKeyState::KeyCode(0x0a, false));

    entry = SKKKeymapEntry("SKK_ENTER", "hex::0x03");
    entry >> key;
    assert(key == SKKKeyState::CharCode(0x03, false));

    entry = SKKKeymapEntry("SKK_ENTER", "ctrl::m");
    entry >> key;
    assert(key == SKKKeyState::CharCode('m', SKKKeyState::CTRL));

    entry = SKKKeymapEntry("Direct", "group::a,c,d-f");
    assert(!entry.IsNot());
    assert(!entry.IsEvent());
    assert(entry.Symbol() == Direct);
    entry >> key;
    assert(key == SKKKeyState::CharCode('a', false));
    entry >> key;
    assert(key == SKKKeyState::CharCode('c', false));
    entry >> key;
    assert(key == SKKKeyState::CharCode('d', false));
    entry >> key;
    assert(key == SKKKeyState::CharCode('e', false));
    entry >> key;
    assert(key == SKKKeyState::CharCode('f', false));

    entry = SKKKeymapEntry("NotDirect", "group::a,c,d-f");
    assert(entry.IsNot());
    assert(!entry.IsEvent());
    assert(entry.Symbol() == Direct);
    entry >> key;
    assert(key == SKKKeyState::CharCode('a', false));
    entry >> key;
    assert(key == SKKKeyState::CharCode('c', false));
    entry >> key;
    assert(key == SKKKeyState::CharCode('d', false));
    entry >> key;
    assert(key == SKKKeyState::CharCode('e', false));
    entry >> key;
    assert(key == SKKKeyState::CharCode('f', false));
}
