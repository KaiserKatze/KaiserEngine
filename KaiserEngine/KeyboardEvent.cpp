#include "stdafx.h"
#include "KeyboardEvent.h"

// @see: https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getkeystate
static
inline
bool
IsKeyActive(int virtualKey)
{
    return (::GetKeyState(virtualKey) & 1) != 0;
}

KeyboardEvent::
KeyboardEvent(UINT message, WPARAM wParam, LPARAM lParam)
    : vk{ wParam & 0xFF }
    , rc{ lParam & 0xFFFF }
    , ek{ 0x1 & (lParam >> 24) }
    , cc{ 0x1 & (lParam >> 29) }
    , ks{ 0x1 & (lParam >> 30) }
    , ts{ 0x1 & (lParam >> 31) }
    , kslctrl{ ::IsKeyActive(VK_LCONTROL) }
    , ksrctrl{ ::IsKeyActive(VK_RCONTROL) }
    , kscapslock{ ::IsKeyActive(VK_CAPITAL) }
    , kslshift{ ::IsKeyActive(VK_LSHIFT) }
    , ksrshift{ ::IsKeyActive(VK_RSHIFT) }
    , kslalt{ ::IsKeyActive(VK_LMENU) }
    , ksralt{ ::IsKeyActive(VK_RMENU) }
{
}

KeyboardEvent::
~KeyboardEvent()
{
}


int
KeyboardEvent::
GetKeyCode() const
{
    int code{ vk };

    if (VK_NUMPAD0 <= vk && vk <= VK_NUMPAD9)
    {
        code = vk - VK_NUMPAD0 + 0x30;
    }
    else if (vk == VK_LCONTROL || vk == VK_RCONTROL)
    {
        code = VK_CONTROL;
    }
    else if (vk == VK_LSHIFT || vk == VK_RSHIFT)
    {
        code = VK_SHIFT;
    }
    else if (vk == VK_LMENU || vk == VK_RMENU)
    {
        code = VK_MENU;
    }

    return code;
}

int
KeyboardEvent::
GetKeyLocation() const
{
    return vk;
}

bool
KeyboardEvent::
IsCtrlKeyDown() const
{
    return kslctrl || ksrctrl;
}

bool
KeyboardEvent::
isShiftKeyDown() const
{
    return kslshift || ksrshift;
}

bool
KeyboardEvent::
IsAltKeyDown() const
{
    return kslalt || ksralt;
}

bool
KeyboardEvent::
isKeyDown() const
{
    return (this->ts & 1) == 0;
}

bool
KeyboardEvent::
isKeyUp() const
{
    return !(this->isKeyDown());
}
