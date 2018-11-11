#include "Keys.h"

Keys::Keys()
	:
	Q(CharToScanCode(_T('Q'))),
	W(CharToScanCode(_T('W'))),
	E(CharToScanCode(_T('E'))),
	R(CharToScanCode(_T('R'))),
	A(CharToScanCode(_T('A'))),
	S(CharToScanCode(_T('S'))),
	D(CharToScanCode(_T('D'))),
	F(CharToScanCode(_T('F'))),
	Z(CharToScanCode(_T('Z'))),
	X(CharToScanCode(_T('X'))),
	C(CharToScanCode(_T('C'))),
	V(CharToScanCode(_T('V'))),
	ZERO(CharToScanCode(_T('0'))),
	ONE(CharToScanCode(_T('1'))),
	TWO(CharToScanCode(_T('2'))),
	THREE(CharToScanCode(_T('3'))),
	FOUR(CharToScanCode(_T('4'))),
	FIVE(CharToScanCode(_T('5'))),
	SIX(CharToScanCode(_T('6'))),
	SEVEN(CharToScanCode(_T('7'))),
	EIGHT(CharToScanCode(_T('8'))),
	NINE(CharToScanCode(_T('9'))),
	F1(VirtualKeyToScanCode(VK_F1)),
	F2(VirtualKeyToScanCode(VK_F2)),
	F3(VirtualKeyToScanCode(VK_F3)),
	F4(VirtualKeyToScanCode(VK_F4)),
	F5(VirtualKeyToScanCode(VK_F5)),
	F6(VirtualKeyToScanCode(VK_F6)),
	F7(VirtualKeyToScanCode(VK_F7)),
	F8(VirtualKeyToScanCode(VK_F8)),
	F9(VirtualKeyToScanCode(VK_F9)),
	F10(VirtualKeyToScanCode(VK_F10)),
	F11(VirtualKeyToScanCode(VK_F11)),
	F12(VirtualKeyToScanCode(VK_F12)),
    SPACE(CharToScanCode(_T(' '))),
	ENTER(CharToScanCode(_T('\n')))
{}

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

const Keys keys;