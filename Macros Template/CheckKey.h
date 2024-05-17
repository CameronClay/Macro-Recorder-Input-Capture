#pragma once
#include <Windows.h>
#include <initializer_list>
#include <vector>

//Check down or released state of VKs (Virtual Keys) and SCs (Scan Codes)
namespace CheckKey
{
	bool VKDown(const RAWKEYBOARD& kbd, TCHAR vKey);
	bool SCDown(const RAWKEYBOARD& kbd, WORD scanCode);

	bool VKRelease(const RAWKEYBOARD& kbd, TCHAR vKey);
	bool SCRelease(const RAWKEYBOARD& kbd, WORD scanCode);

	bool VKComboDown(const RAWKEYBOARD& kbd, std::initializer_list<TCHAR> vKeys);
	bool SCComboDown(const RAWKEYBOARD& kbd, std::initializer_list<WORD> sKeys);

	bool VKComboDown(const RAWKEYBOARD& kbd, const std::vector<TCHAR>& vKeys);
	bool SCComboDown(const RAWKEYBOARD& kbd, const std::vector<WORD>& sKeys);
};