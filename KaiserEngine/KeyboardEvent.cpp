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
    , ksnumlock{ ::IsKeyActive(VK_NUMLOCK) }
{
}

KeyboardEvent::
~KeyboardEvent()
{
}

// @see: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
char
KeyboardEvent::
GetKeyChar() const
{
    char ch{ 0 };

    const bool isShiftDown = this->isShiftKeyDown();
    if (0x30 <= vk && vk <= 0x39)
    {
        ch = vk - 0x30 + '0';
        if (isShiftDown)
        {
            switch (ch)
            {
            case '1':
                ch = '!';
                break;
            case '2':
                ch = '@';
                break;
            case '3':
                ch = '#';
                break;
            case '4':
                ch = '$';
                break;
            case '5':
                ch = '%';
                break;
            case '6':
                ch = '^';
                break;
            case '7':
                ch = '&';
                break;
            case '8':
                ch = '*';
                break;
            case '9':
                ch = '(';
                break;
            case '0':
                ch = ')';
                break;
            }
        }
    }
    else if (ksnumlock && VK_NUMPAD0 <= vk && vk <= VK_NUMPAD9)
    {
        ch = vk - VK_NUMPAD0 + '0';
    }
    else if (0x41 <= vk && vk <= 0x5A)
    {
        const bool isCapital = isShiftDown ^ kscapslock;
        ch = vk - 0x41 + (isCapital ? 'A' : 'a');
    }
    else
    {
        // other keyboard layouts, such as those of french and german
        // should be implemented here

        // US standard keyboard layout

        switch (vk)
        {
        case (VK_MULTIPLY):
            ch = '*';
            break;
        case (VK_ADD):
            ch = '+';
            break;
        case (VK_SUBTRACT):
            ch = '-';
            break;
        case (VK_DECIMAL):
            ch = '.';
            break;
        case (VK_DIVIDE):
            ch = '/';
            break;
        default:
            if (isShiftDown)
            {
                switch (vk)
                {
                case (VK_OEM_1):
                    ch = ';';
                    break;
                case (VK_OEM_PLUS):
                    ch = '=';
                    break;
                case (VK_OEM_COMMA):
                    ch = ',';
                    break;
                case (VK_OEM_MINUS):
                    ch = '-';
                    break;
                case (VK_OEM_PERIOD):
                    ch = '.';
                    break;
                case (VK_OEM_2):
                    ch = '/';
                    break;
                case (VK_OEM_3):
                    ch = '`';
                    break;
                case (VK_OEM_4):
                    ch = '[';
                    break;
                case (VK_OEM_5):
                    ch = '\\';
                    break;
                case (VK_OEM_6):
                    ch = ']';
                    break;
                case (VK_OEM_7):
                    ch = '\'';
                    break;
                }
            }
            else
            {
                switch (vk)
                {
                case (VK_OEM_1):
                    ch = ':';
                    break;
                case (VK_OEM_PLUS):
                    ch = '+';
                    break;
                case (VK_OEM_COMMA):
                    ch = '<';
                    break;
                case (VK_OEM_MINUS):
                    ch = '_';
                    break;
                case (VK_OEM_PERIOD):
                    ch = '>';
                    break;
                case (VK_OEM_2):
                    ch = '?';
                    break;
                case (VK_OEM_3):
                    ch = '~';
                    break;
                case (VK_OEM_4):
                    ch = '{';
                    break;
                case (VK_OEM_5):
                    ch = '|';
                    break;
                case (VK_OEM_6):
                    ch = '}';
                    break;
                case (VK_OEM_7):
                    ch = '"';
                    break;
                }
            }
        }
    }

    return ch;
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
