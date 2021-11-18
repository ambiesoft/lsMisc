//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.



#include <windows.h>
#include <CommCtrl.h>
#include "GetHotkeyString.h"

using namespace std;

static wstring GetVKString(BYTE vk)
{
    if (0x41 <= vk && vk <= 0x5a)
    {
        wstring ret;
        ret += (char)vk;
        return ret;
    }
    switch (vk)
    {
    case 0x01: return L"LBUTTON";
    case 0x02: return L"RBUTTON";
    case 0x03: return L"CANCEL";
    case 0x04: return L"MBUTTON";
    case 0x05: return L"XBUTTON1";
    case 0x06: return L"XBUTTON2";
    case 0x08: return L"BACK";
    case 0x09: return L"TAB";
    case 0x0C: return L"CLEAR";
    case 0x0D: return L"return ";
    case 0x10: return L"SHIFT";
    case 0x11: return L"CONTROL";
    case 0x12: return L"MENU";
    case 0x13: return L"PAUSE";
    case 0x14: return L"CAPITAL";
    //case 0x15: return L"HANGEUL";
    //case 0x15: return L"HANGUL";
    case 0x15: return L"KANA";
    case 0x17: return L"JUNJA";
    case 0x18: return L"FINAL";
    // case 0x19: return L"HANJA";
    case 0x19: return L"KANJI";
    case 0x1B: return L"ESCAPE";
    case 0x1C: return L"CONVERT";
    case 0x1D: return L"NONCONVERT";
    case 0x1E: return L"ACCEPT";
    case 0x1F: return L"MODECHANGE";
    case 0x20: return L"SPACE";
    case 0x21: return L"PRIOR";
    case 0x22: return L"NEXT";
    case 0x23: return L"END";
    case 0x24: return L"HOME";
    case 0x25: return L"LEFT";
    case 0x26: return L"UP";
    case 0x27: return L"RIGHT";
    case 0x28: return L"DOWN";
    case 0x29: return L"SELECT";
    case 0x2A: return L"PRINT";
    case 0x2B: return L"EXECUTE";
    case 0x2C: return L"SNAPSHOT";
    case 0x2D: return L"INSERT";
    case 0x2E: return L"DELETE";
    case 0x2F: return L"HELP";
    case 0x30: return L"0";
    case 0x31: return L"1";
    case 0x32: return L"2";
    case 0x33: return L"3";
    case 0x34: return L"4";
    case 0x35: return L"5";
    case 0x36: return L"6";
    case 0x37: return L"7";
    case 0x38: return L"8";
    case 0x39: return L"9";
    case 0x3a: return L"Unassgined";
    case 0x3b: return L"Unassgined";
    case 0x3c: return L"Unassgined";
    case 0x3d: return L"Unassgined";
    case 0x3e: return L"Unassgined";
    case 0x3f: return L"Unassgined";
    case 0x40: return L"Unassgined";
    case 0x5B: return L"LWIN";
    case 0x5C: return L"RWIN";
    case 0x5D: return L"APPS";
    case 0x5F: return L"SLEEP";
    case 0x60: return L"NUMPAD0";
    case 0x61: return L"NUMPAD1";
    case 0x62: return L"NUMPAD2";
    case 0x63: return L"NUMPAD3";
    case 0x64: return L"NUMPAD4";
    case 0x65: return L"NUMPAD5";
    case 0x66: return L"NUMPAD6";
    case 0x67: return L"NUMPAD7";
    case 0x68: return L"NUMPAD8";
    case 0x69: return L"NUMPAD9";
    case 0x6A: return L"MULTIPLY";
    case 0x6B: return L"ADD";
    case 0x6C: return L"SEPARATOR";
    case 0x6D: return L"SUBTRACT";
    case 0x6E: return L"DECIMAL";
    case 0x6F: return L"DIVIDE";
    case 0x70: return L"F1";
    case 0x71: return L"F2";
    case 0x72: return L"F3";
    case 0x73: return L"F4";
    case 0x74: return L"F5";
    case 0x75: return L"F6";
    case 0x76: return L"F7";
    case 0x77: return L"F8";
    case 0x78: return L"F9";
    case 0x79: return L"F10";
    case 0x7A: return L"F11";
    case 0x7B: return L"F12";
    case 0x7C: return L"F13";
    case 0x7D: return L"F14";
    case 0x7E: return L"F15";
    case 0x7F: return L"F16";
    case 0x80: return L"F17";
    case 0x81: return L"F18";
    case 0x82: return L"F19";
    case 0x83: return L"F20";
    case 0x84: return L"F21";
    case 0x85: return L"F22";
    case 0x86: return L"F23";
    case 0x87: return L"F24";
    case 0x88: return L"NAVIGATION_VIEW";
    case 0x89: return L"NAVIGATION_MENU";
    case 0x8A: return L"NAVIGATION_UP";
    case 0x8B: return L"NAVIGATION_DOWN";
    case 0x8C: return L"NAVIGATION_LEFT";
    case 0x8D: return L"NAVIGATION_RIGHT";
    case 0x8E: return L"NAVIGATION_ACCEPT";
    case 0x8F: return L"NAVIGATION_CANCEL";
    case 0x90: return L"NUMLOCK";
    case 0x91: return L"SCROLL";
    // case 0x92: return L"OEM_FJ_JISHO";
    case 0x92: return L"OEM_NEC_EQUAL";
    case 0x93: return L"OEM_FJ_MASSHOU";
    case 0x94: return L"OEM_FJ_TOUROKU";
    case 0x95: return L"OEM_FJ_LOYA";
    case 0x96: return L"OEM_FJ_ROYA";
    case 0xA0: return L"LSHIFT";
    case 0xA1: return L"RSHIFT";
    case 0xA2: return L"LCONTROL";
    case 0xA3: return L"RCONTROL";
    case 0xA4: return L"LMENU";
    case 0xA5: return L"RMENU";
    case 0xA6: return L"BROWSER_BACK";
    case 0xA7: return L"BROWSER_FORWARD";
    case 0xA8: return L"BROWSER_REFRESH";
    case 0xA9: return L"BROWSER_STOP";
    case 0xAA: return L"BROWSER_SEARCH";
    case 0xAB: return L"BROWSER_FAVORITES";
    case 0xAC: return L"BROWSER_HOME";
    case 0xAD: return L"VOLUME_MUTE";
    case 0xAE: return L"VOLUME_DOWN";
    case 0xAF: return L"VOLUME_UP";
    case 0xB0: return L"MEDIA_NEXT_TRACK";
    case 0xB1: return L"MEDIA_PREV_TRACK";
    case 0xB2: return L"MEDIA_STOP";
    case 0xB3: return L"MEDIA_PLAY_PAUSE";
    case 0xB4: return L"LAUNCH_MAIL";
    case 0xB5: return L"LAUNCH_MEDIA_SELECT";
    case 0xB6: return L"LAUNCH_APP1";
    case 0xB7: return L"LAUNCH_APP2";
    case 0xBA: return L"OEM_1";
    case 0xBB: return L"OEM_PLUS";
    case 0xBC: return L"OEM_COMMA";
    case 0xBD: return L"OEM_MINUS";
    case 0xBE: return L"OEM_PERIOD";
    case 0xBF: return L"OEM_2";
    case 0xC0: return L"OEM_3";
    case 0xC3: return L"GAMEPAD_A";
    case 0xC4: return L"GAMEPAD_B";
    case 0xC5: return L"GAMEPAD_X";
    case 0xC6: return L"GAMEPAD_Y";
    case 0xC7: return L"GAMEPAD_RIGHT_SHOULDER";
    case 0xC8: return L"GAMEPAD_LEFT_SHOULDER";
    case 0xC9: return L"GAMEPAD_LEFT_TRIGGER";
    case 0xCA: return L"GAMEPAD_RIGHT_TRIGGER";
    case 0xCB: return L"GAMEPAD_DPAD_UP";
    case 0xCC: return L"GAMEPAD_DPAD_DOWN";
    case 0xCD: return L"GAMEPAD_DPAD_LEFT";
    case 0xCE: return L"GAMEPAD_DPAD_RIGHT";
    case 0xCF: return L"GAMEPAD_MENU";
    case 0xD0: return L"GAMEPAD_VIEW";
    case 0xD1: return L"GAMEPAD_LEFT_THUMBSTICK_BUTTON";
    case 0xD2: return L"GAMEPAD_RIGHT_THUMBSTICK_BUTTON";
    case 0xD3: return L"GAMEPAD_LEFT_THUMBSTICK_UP";
    case 0xD4: return L"GAMEPAD_LEFT_THUMBSTICK_DOWN";
    case 0xD5: return L"GAMEPAD_LEFT_THUMBSTICK_RIGHT";
    case 0xD6: return L"GAMEPAD_LEFT_THUMBSTICK_LEFT";
    case 0xD7: return L"GAMEPAD_RIGHT_THUMBSTICK_UP";
    case 0xD8: return L"GAMEPAD_RIGHT_THUMBSTICK_DOWN";
    case 0xD9: return L"GAMEPAD_RIGHT_THUMBSTICK_RIGHT";
    case 0xDA: return L"GAMEPAD_RIGHT_THUMBSTICK_LEFT";
    case 0xDB: return L"OEM_4";
    case 0xDC: return L"OEM_5";
    case 0xDD: return L"OEM_6";
    case 0xDE: return L"OEM_7";
    case 0xDF: return L"OEM_8";
    case 0xE1: return L"OEM_AX";
    case 0xE2: return L"OEM_102";
    case 0xE3: return L"ICO_HELP";
    case 0xE4: return L"ICO_00";
    case 0xE5: return L"PROCESSKEY";
    case 0xE6: return L"ICO_CLEAR";
    case 0xE7: return L"PACKET";
    case 0xE9: return L"OEM_RESET";
    case 0xEA: return L"OEM_JUMP";
    case 0xEB: return L"OEM_PA1";
    case 0xEC: return L"OEM_PA2";
    case 0xED: return L"OEM_PA3";
    case 0xEE: return L"OEM_WSCTRL";
    case 0xEF: return L"OEM_CUSEL";
    case 0xF0: return L"OEM_ATTN";
    case 0xF1: return L"OEM_FINISH";
    case 0xF2: return L"OEM_COPY";
    case 0xF3: return L"OEM_AUTO";
    case 0xF4: return L"OEM_ENLW";
    case 0xF5: return L"OEM_BACKTAB";
    case 0xF6: return L"ATTN";
    case 0xF7: return L"CRSEL";
    case 0xF8: return L"EXSEL";
    case 0xF9: return L"EREOF";
    case 0xFA: return L"PLAY";
    case 0xFB: return L"ZOOM";
    case 0xFC: return L"NONAME";
    case 0xFD: return L"PA1";
    case 0xFE: return L"OEM_CLEAR";
    }
    return L"<ERROR>";
}

UINT GetHotkeyModForRegisterHotKey(WORD wKey)
{
    UINT ret = 0;
    if (HIBYTE(wKey) & HOTKEYF_CONTROL)
        ret |= MOD_CONTROL;
    if (HIBYTE(wKey) & HOTKEYF_SHIFT)
        ret |= MOD_SHIFT;
    if (HIBYTE(wKey) & HOTKEYF_ALT)
        ret |= MOD_ALT;
    return ret;
}
wstring GetHotkeyControlStringW(WORD wKey)
{
	wstring ret;
	if(wKey==0)
		return ret;

	if(HIBYTE(wKey) & HOTKEYF_CONTROL)
		ret += L"Ctrl+";
	if(HIBYTE(wKey) & HOTKEYF_SHIFT)
		ret += L"Shift+";
	if(HIBYTE(wKey) & HOTKEYF_ALT)
		ret += L"Alt+";

    //if (HIBYTE(wKey) & MOD_CONTROL)
    //    ret += L"Ctrl+";
    //if (HIBYTE(wKey) & MOD_SHIFT)
    //    ret += L"Shift+";
    //if (HIBYTE(wKey) & MOD_ALT)
    //    ret += L"Alt+";
    //if (HIBYTE(wKey) & MOD_WIN)
    //    ret += L"Win+";

    return ret + GetVKString(LOBYTE(wKey));

 //   if (VK_F1 <= LOBYTE(wKey) && LOBYTE(wKey) <= VK_F24)
 //   {
 //       // GetKeyNameText does not work for function keys
 //       int n = LOBYTE(wKey) - VK_F1 + 1;
 //       return ret + L"F" + to_wstring(n);
 //   }
 //   else if(VK_LEFT <= LOBYTE(wKey) && LOBYTE(wKey) <= VK_F24)

 //   unsigned int scanCode = MapVirtualKey(LOBYTE(wKey), 0); //MAPVK_VK_TO_VSC);
 //   // because MapVirtualKey strips the extended bit for some keys
 //   switch (LOBYTE(wKey))
 //   {
 //       case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN: // arrow keys
 //       case VK_PRIOR: case VK_NEXT: // page up and page down
 //       case VK_END: case VK_HOME:
 //       case VK_INSERT: case VK_DELETE:
 //       case VK_DIVIDE: // numpad slash
 //       case VK_NUMLOCK:
 //       {
 //           scanCode |= 0x100; // set extended bit
 //           break;
 //       }
 //   }


 //   else
 //   {
 //       wchar_t keyName[64] = { 0 };
 //       if (GetKeyNameText(scanCode << 16, keyName, sizeof(keyName) / sizeof(keyName[0]) != 0))
 //       {
 //           ret += keyName;
 //       }
 //       else
 //       {
 //           ret += L"[Error]";
 //       }
 //   }

	//return ret;
}
