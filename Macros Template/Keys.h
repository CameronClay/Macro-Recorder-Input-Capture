#pragma once
#include <Windows.h>
#include <tchar.h>
#include <bitset>
#include <vector>

class Keys
{
public:
	friend class MainWindow;

	Keys() = default;

	static TCHAR CharToVirtualKey(TCHAR c);
	static WORD CharToScanCode(TCHAR c);

	static WORD VirtualKeyToScanCode(TCHAR vk);
	static TCHAR ScanCodeToVirtualKey(WORD scan);

	bool IsPressed(unsigned char vKey) const;
	bool IsPressedCombo(std::initializer_list<TCHAR> vKeys) const;
	bool IsPressedCombo(const std::vector<TCHAR>& vKeys) const;

	bool IsPressedSC(unsigned char sc) const;
	bool IsPressedComboSC(std::initializer_list<TCHAR> scs) const;
	bool IsPressedComboSC(const std::vector<TCHAR>& scs) const;
private:
	void OnPress(unsigned char vKey);
	void OnRelease(unsigned char vKey);

	static constexpr unsigned int nKeys = 256u;
	std::bitset<nKeys> keyStates;
};