#include "RawInp.h"
#include <tchar.h>
#include <assert.h>

void RawInp::Input(HINSTANCE hInst, RawInp& rawInp)
{
	if (!rawInp.wnd.Create(0, 0, 0, 0, _T("RAW_INPUT"), _T("RAW_INPUT"), true))
		return;

	if (!rawInp.InitializeInputDevices())
	{
		Window::MsgBox(_T("Call to InitializeInputDevices failed!"));
		return;
	}

	MSG msg {};
	while (GetMessage(&msg, 0, 0, 0))
	{
		rawInp.UpdateTimeStamp(msg.time);
		DispatchMessage(&msg);
	}
}

RawInp::RawInp(HINSTANCE hInst, MOUSEPROC mouseProc, KBDPROC kbdProc)
	:
	thrd(&RawInp::Input, hInst, std::ref(*this)),
	wnd(hInst, WNDPROCP{ {&RawInp::RawInputProc}, this }),
	mouseProc(mouseProc),
	kbdProc(kbdProc),
	prevTime(0),
	curTime(0)
{}

RawInp::~RawInp()
{
	wnd.Close();
	thrd.join();
}

bool RawInp::InitializeInputDevices()
{
	int deviceIndex = 0;
	RAWINPUTDEVICE rid[] = { {},{} };
	if (kbdProc)
	{
		rid[deviceIndex].usUsagePage = 0x01;
		rid[deviceIndex].usUsage = 0x06;
		rid[deviceIndex].dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
		rid[deviceIndex].hwndTarget = wnd.GetHWND();

		++deviceIndex;
	}

	if (mouseProc)
	{
		rid[deviceIndex].usUsagePage = 0x01;
		rid[deviceIndex].usUsage = 0x02;
		rid[deviceIndex].dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
		rid[deviceIndex].hwndTarget = wnd.GetHWND();

		++deviceIndex;
	}

	return RegisterRawInputDevices(rid, deviceIndex, sizeof(RAWINPUTDEVICE)) == TRUE;
}

void RawInp::UpdateTimeStamp(DWORD t)
{
	prevTime = curTime;
	curTime = t;
}


LRESULT CALLBACK RawInp::RawInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INPUT:
	{
		static constexpr uint32_t buffSize = 128;
		char buffer[buffSize];

		uint32_t outSize = buffSize;
		uint32_t res = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &outSize, sizeof(RAWINPUTHEADER));
		if (res == 0)
			Window::MsgBox(_T("Call to GetRawInputData failed!"));

		RAWINPUT* rawinput = reinterpret_cast<RAWINPUT*>(buffer);
		if (rawinput->header.dwType == RIM_TYPEKEYBOARD)
			kbdProc(rawinput->data.keyboard, curTime - prevTime);
		else if (rawinput->header.dwType == RIM_TYPEMOUSE)
			mouseProc(rawinput->data.mouse, curTime - prevTime);

		DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}