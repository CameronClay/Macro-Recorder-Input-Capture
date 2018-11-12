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
	if ((vKeys.size() != 0) && (kbd.VKey == *(vKeys.end() - 1)) && (kbd.Message == WM_KEYDOWN))
	{
		for (auto it = vKeys.begin(), end = vKeys.end() - 1; it != end; ++it)
			if (!(GetAsyncKeyState(*it) & 0x8000))
				return false;
		return true;
	}
	return false;
}
bool CheckKey::SCComboDown(const RAWKEYBOARD& kbd, std::initializer_list<WORD> sKeys)
{
	if ((sKeys.size() != 0) && (kbd.MakeCode == *(sKeys.end() - 1)) && (kbd.Flags == RI_KEY_BREAK))
	{
		for (auto it = sKeys.begin(), end = sKeys.end() - 1; it != end; ++it)
			if (!(GetAsyncKeyState(keys.ScanCodeToVirtualKey(*it)) & 0x8000))
				return false;
		return true;
	}
	return false;
}

bool CheckKey::VKComboDown(const RAWKEYBOARD& kbd, const std::vector<TCHAR>& vKeys)
{
	if ((vKeys.size() != 0) && (kbd.VKey == *(vKeys.end() - 1)) && (kbd.Message == WM_KEYDOWN))
	{
		for (auto it = vKeys.begin(), end = vKeys.end() - 1; it != end; ++it)
			if (!(GetAsyncKeyState(*it) & 0x8000))
				return false;
		return true;
	}
	return false;
}
bool CheckKey::SCComboDown(const RAWKEYBOARD& kbd, const std::vector<WORD>& sKeys)
{
	if ((sKeys.size() != 0) && (kbd.MakeCode == *(sKeys.end() - 1)) && (kbd.Flags == RI_KEY_BREAK))
	{
		for (auto it = sKeys.begin(), end = sKeys.end() - 1; it != end; ++it)
			if (!(GetAsyncKeyState(keys.ScanCodeToVirtualKey(*it)) & 0x8000))
				return false;
		return true;
	}
	return false;
}