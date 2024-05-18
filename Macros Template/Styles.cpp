#include "Styles.h"
#include <windowsx.h>

void Styles::Initalize(HDC hdc, HPEN hp, HBRUSH hb, HBRUSH clrb, HFONT hf, COLORREF bckClr, COLORREF txtColor, int charSpacing) {
	this->hp = hp;
	this->hb = hb;
	this->clrb = clrb;
	this->hf = hf;

	SelectFont(hdc, hf);
	SelectPen(hdc, hp);
	SelectBrush(hdc, hb);
	SetBkColor(hdc, bckClr);
	SetTextColor(hdc, RGB(0, 0, 0));
	SetTextCharacterExtra(hdc, 1);
}

void Styles::Cleanup() {
	if (hf)
		DeleteFont(hf);

	if (hp)
		DeletePen(hp);

	if (hb)
		DeleteBrush(hb);

	if (clrb)
		DeleteBrush(clrb);
}