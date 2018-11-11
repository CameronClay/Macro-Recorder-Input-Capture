#pragma once
#include <Windows.h>
#include <memory>
#include <thread>
#include "Function.h"

using MOUSEPROC = PFunc<void, const RAWMOUSE&, DWORD>;
using KEYBOARDPROC = PFunc<void, const RAWKEYBOARD&, DWORD>;

class RawInp
{
public:
	RawInp(HINSTANCE hInst, MOUSEPROC mouseProc = nullptr, KEYBOARDPROC kbdProc = nullptr);
	~RawInp();

	bool InitializeInputDevices(HWND wnd);
	void SendMouseProc(const RAWMOUSE& mouse, DWORD delay);
	void SendKbdProc(const RAWKEYBOARD& kbd, DWORD delay);
private:
	std::thread thrd;
	HWND wnd;
	MOUSEPROC mouseProc;
	KEYBOARDPROC kbdProc;
};