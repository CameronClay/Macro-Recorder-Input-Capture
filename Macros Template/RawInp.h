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
	void SendMouseProc(const RAWMOUSE& mouse, DWORD delay) const;
	void SendKbdProc(const RAWKEYBOARD& kbd, DWORD delay) const;
	void UpdateTimeStamp(DWORD t);

private:
	friend LRESULT CALLBACK RawInputProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK RawInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	std::thread thrd;
	HWND wnd;
	MOUSEPROC mouseProc;
	KEYBOARDPROC kbdProc;
	DWORD prevTime, curTime;
};