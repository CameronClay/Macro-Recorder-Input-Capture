#pragma once
#include <Windows.h>

struct Styles
{
	Styles() = default;

	void Initalize(HDC hdc, HPEN hp, HBRUSH hb, HBRUSH clrb, HFONT hf, COLORREF bckClr = RGB(255, 255, 255), COLORREF txtColor = RGB(0, 0, 0), int charSpacing = 1);
	void Cleanup();

	BYTE alpha = 255;
	HPEN hp = NULL;
	HBRUSH hb = NULL;
	HBRUSH clrb = NULL;
	HFONT hf = NULL;
	bool initialized = false;
};