#pragma once
#include <Windows.h>
#include <memory>
#include <thread>
#include "Function.h"
#include "Window.h"

using MOUSEPROC = Function<void, const RAWMOUSE&, DWORD>;
using KBDPROC   = Function<void, const RAWKEYBOARD&, DWORD>;

class RawInp
{
public:
	friend static void Input(HINSTANCE hInst, RawInp& rawInp);

	RawInp(HINSTANCE hInst, MOUSEPROC mouseProc = (MOUSEPROC)nullptr, KBDPROC kbdProc = (KBDPROC)nullptr);
	~RawInp();

private:
	LRESULT CALLBACK RawInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool InitializeInputDevices();
	void UpdateTimeStamp(DWORD t);

	std::thread thrd;
	Window wnd;
	MOUSEPROC mouseProc;
	KBDPROC kbdProc;
	DWORD prevTime, curTime;
};