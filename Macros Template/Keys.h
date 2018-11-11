#pragma once
#include <Windows.h>
#include <tchar.h>

class Keys
{
public:
	Keys();

	static TCHAR CharToVirtualKey(TCHAR c);
	static WORD CharToScanCode(TCHAR c);

	static WORD VirtualKeyToScanCode(TCHAR vk);
	static TCHAR ScanCodeToVirtualKey(WORD scan);

	const WORD Q, W, E, R;
	const WORD A, S, D, F;
	const WORD Z, X, C, V;
	const WORD ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE;
	const WORD F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12;
	const WORD SPACE, ENTER;
};

extern const Keys keys;