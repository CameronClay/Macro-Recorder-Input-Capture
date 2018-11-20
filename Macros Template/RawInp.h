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
	RawInp(HINSTANCE hInst, MOUSEPROC mouseProc = (MOUSEPROC)nullptr, KBDPROC kbdProc = (KBDPROC)nullptr);
	~RawInp();

private:
	static void Input(HINSTANCE, RawInp&);
	LRESULT CALLBACK RawInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool InitializeInputDevices();
	void UpdateTimeStamp(DWORD t);

	std::thread thrd;
	Window wnd;
	MOUSEPROC mouseProc;
	KBDPROC kbdProc;
	DWORD prevTime, curTime;
};