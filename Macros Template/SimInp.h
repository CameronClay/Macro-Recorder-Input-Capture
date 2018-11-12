#pragma once
#include <Windows.h>
#include <initializer_list>
#include <vector>

namespace SimInp
{
	//-Kbd Functions
	//--VirtualKeyCode
	bool SendKbdDown(WORD key);
	bool SendKbdUp(WORD key);
	bool SendKbd(WORD key, DWORD delayMilli = 0);
	void KeyCombo(std::initializer_list<TCHAR> keys);
	void KeyCombo(const std::vector<TCHAR>& keys);
	void SendKbd(const TCHAR* str, int len);

	//--Scan Codes
	bool SendKbdDownSC(WORD key, bool E0 = false);
	bool SendKbdUpSC(WORD key, bool E0 = false);
	bool SendKbdSC(WORD key, bool E0 = false, DWORD delayMilli = 0);
	void KeyComboSC(std::initializer_list<std::pair<WORD, bool>> keys);
	void KeyComboSC(const std::vector<std::pair<WORD, bool>>& keys);
	void SendKbdSC(const TCHAR* str, int len);

	//-Mouse Functions
	bool SendClickDown(bool left, bool right, bool middle);
	bool SendClickUp(bool left, bool right, bool middle);
	bool SendClick(bool left, bool right, bool middle, DWORD delayMilli = 0);

	bool SendXClickDown(bool xButton1, bool xButton2);
	bool SendXClickUp(bool xButton1, bool xButton2);
	bool SendXClick(bool xButton1, bool xButton2, DWORD delayMilli = 0);

	//If absolute x and y are normalized coordinates from 0 to 65,535
	bool SendMousePosition(int x, int y, bool absolute = false);
	void MouseMove(int x, int y, DWORD duration, DWORD freq);

	bool SendMouseScroll(int nClicks);
}