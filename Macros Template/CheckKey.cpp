#include "CheckKey.h"
#include "Keys.h"

bool CheckKey::VKDown(const RAWKEYBOARD& kbd, TCHAR vKey)
{
	return (kbd.VKey == vKey) && (kbd.Message == WM_KEYDOWN);
}
bool CheckKey::SCDown(const RAWKEYBOARD& kbd, WORD scanCode)
{
	return (kbd.MakeCode == scanCode) && (kbd.Flags == RI_KEY_MAKE);
}

bool CheckKey::VKRelease(const RAWKEYBOARD& kbd, TCHAR vKey)
{
	return (kbd.VKey == vKey) && (kbd.Message == WM_KEYUP);
}
bool CheckKey::SCRelease(const RAWKEYBOARD& kbd, WORD scanCode)
{
	return (kbd.MakeCode == scanCode) && (kbd.Flags == RI_KEY_BREAK);
}

bool CheckKey::VKComboDown(const RAWKEYBOARD& kbd, std::initializer_list<TCHAR> vKeys)
{
	if ((kbd.VKey == *(vKeys.end() - 1)) && (kbd.Message == WM_KEYDOWN))
	{
		for (auto it = vKeys.begin(), end = vKeys.end() - 1; it != end; ++it)
			if (!(GetAsyncKeyState(*it) & 0x8000))
				return false;
		return true;
	}
	return false;
}
bool CheckKey::SCComboDown(const RAWKEYBOARD& kbd, std::initializer_list<WORD> vKeys)
{
	if ((kbd.MakeCode == *(vKeys.end() - 1)) && (kbd.Flags == RI_KEY_BREAK))
	{
		for (auto it = vKeys.begin(), end = vKeys.end() - 1; it != end; ++it)
			if (!(GetAsyncKeyState(keys.ScanCodeToVirtualKey(*it)) & 0x8000))
				return false;
		return true;
	}
	return false;
}

bool CheckKey::VKComboDown(const RAWKEYBOARD& kbd, const std::vector<TCHAR>& vKeys)
{
	if ((kbd.VKey == *(vKeys.end() - 1)) && (kbd.Message == WM_KEYDOWN))
	{
		for (auto it = vKeys.begin(), end = vKeys.end() - 1; it != end; ++it)
			if (!(GetAsyncKeyState(*it) & 0x8000))
				return false;
		return true;
	}
	return false;
}
bool CheckKey::SCComboDown(const RAWKEYBOARD& kbd, const std::vector<WORD>& vKeys)
{
	if ((kbd.MakeCode == *(vKeys.end() - 1)) && (kbd.Flags == RI_KEY_BREAK))
	{
		for (auto it = vKeys.begin(), end = vKeys.end() - 1; it != end; ++it)
			if (!(GetAsyncKeyState(keys.ScanCodeToVirtualKey(*it)) & 0x8000))
				return false;
		return true;
	}
	return false;
}