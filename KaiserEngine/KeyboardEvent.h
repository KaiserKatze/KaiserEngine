#pragma once

#include <Windows.h>
#include "Event.h"

class KeyboardEvent
    : public Event
{
public:
    KeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam);
    virtual ~KeyboardEvent();
private:
    struct
    {
#pragma pack(push, 1)
        // Virtual-key code
        // @see: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
        const int vk: 8;
        // Repeat code
        // The number of times the keystroke is autorepeated
        // as a result of the user holding down the key.
        // The repeat count is always 1 for a WM_KEYUP message
        const int rc : 16;
        // Extended key
        // Indicates whether the key is an extended key, such as the right-hand ALT
        // and CTRL keys that appear on an enhanced 101- or 102-key keyboard
        // The value is 1 if it is an extended key; otherwise, it is 0
        const int ek : 1;
        // Context code
        // The value is always 0 for a WM_KEYUP message
        // The value is always 0 for a WM_KEYDOWN message
        const int cc : 1;
        // Previous key state
        // The value is always 1 for a WM_KEYUP message
        // The value is 1 if the key is down before the message is sent, or it is zero if the key is up
        const int ks : 1;
        // Transition state
        // The value is always 1 for a WM_KEYUP message
        // The value is always 0 for a WM_KEYDOWN message
        const int ts : 1;
#pragma pack(pop)
        // KeyStates:
        // <Control>
        const bool kslctrl;     // Left
        const bool ksrctrl;     // Right
        // <CapsLock>
        const bool kscapslock;
        // <Shift>
        const bool kslshift;    // Left
        const bool ksrshift;    // Right
        // <Alternative>
        const bool kslalt;      // Left
        const bool ksralt;      // Right
    };
};
