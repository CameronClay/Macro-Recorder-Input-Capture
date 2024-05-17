#include "Keys.h"

TCHAR Keys::CharToVirtualKey(TCHAR c)
{
	return VkKeyScan(c) & 0xFF;
}
WORD Keys::VirtualKeyToScanCode(TCHAR vk)
{
	return MapVirtualKey(vk, MAPVK_VK_TO_VSC_EX);
}
WORD Keys::CharToScanCode(TCHAR c)
{
	return VirtualKeyToScanCode(CharToVirtualKey(c));
}
TCHAR Keys::ScanCodeToVirtualKey(WORD scan)
{
	return MapVirtualKey(scan, MAPVK_VSC_TO_VK_EX);
}


void Keys::OnPress(unsigned char vKey)
{
	keyStates[vKey] = true;
}
void Keys::OnRelease(unsigned char vKey)
{
	keyStates[vKey] = false;
}

bool Keys::IsPressed(unsigned char vKey) const
{
	return keyStates[vKey];
}
bool Keys::IsPressedCombo(std::initializer_list<TCHAR> vKeys) const
{
	if (vKeys.size() != 0)
	{
		for(const auto& k : vKeys)
			if(!IsPressed(k))
				return false;

		return true;
	}
	return false;
}
bool Keys::IsPressedCombo(const std::vector<TCHAR>& vKeys) const
{
	if (vKeys.size() != 0)
	{
		for (const auto& k : vKeys)
			if (!IsPressed(k))
				return false;

		return true;
	}
	return false;
}

bool Keys::IsPressedSC(unsigned char sc) const
{
	return IsPressed(ScanCodeToVirtualKey(sc));
}
bool Keys::IsPressedComboSC(std::initializer_list<TCHAR> scs) const
{
	if (scs.size() != 0)
	{
		for (const auto& k : scs)
			if (!IsPressedSC(k))
				return false;

		return true;
	}
	return false;
}
bool Keys::IsPressedComboSC(const std::vector<TCHAR>& scs) const
{
	if (scs.size() != 0)
	{
		for (const auto& k : scs)
			if (!IsPressedSC(k))
				return false;

		return true;
	}
	return false;
}