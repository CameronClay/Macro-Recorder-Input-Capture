#pragma once
#include <Windows.h>
#include <memory>
#include <thread>
#include "Function.h"

using MouseProc = PFunc<void, const RAWMOUSE&, DWORD>;
using KeyboardProc = PFunc<void, const RAWKEYBOARD&, DWORD>;

class RawInp
{
public:
	RawInp(HINSTANCE hInst, MouseProc mouseProc = nullptr, KeyboardProc kbdProc = nullptr);
	~RawInp();

	bool InitializeInputDevices(HWND wnd);
	void SendMouseProc(const RAWMOUSE& mouse, DWORD delay);
	void SendKbdProc(const RAWKEYBOARD& kbd, DWORD delay);
private:
	std::thread thrd;
	HWND wnd;
	MouseProc mouseProc;
	KeyboardProc kbdProc;
};