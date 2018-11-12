#include "RawInp.h"
#include <tchar.h>
#include <assert.h>

static LRESULT CALLBACK RawInputProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RawInp* pThis;

	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<RawInp*>(lpcs->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA,reinterpret_cast<LONG_PTR>(pThis));
	}
	else 
	{
		pThis = reinterpret_cast<RawInp*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis) 
		return pThis->RawInputProc(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

static void Input(HINSTANCE hInst, RawInp& rawInp)
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInst;
	wc.lpfnWndProc = &RawInputProcCallback;
	wc.lpszClassName = _T("RAW_INPUT");

	if (!RegisterClassEx(&wc))
		MessageBox(NULL, _T("Call to RegisterClass failed!"), wc.lpszClassName, MB_OK);

	HWND wnd = CreateWindowEx(NULL, wc.lpszClassName, _T("RAW_INPUT"), NULL, 0, 0, 0, 0, NULL, NULL, wc.hInstance, reinterpret_cast<LPVOID>(&rawInp));
	if (!wnd)
		MessageBox(NULL, _T("Call to CreateWindowEX failed!"), wc.lpszClassName, MB_OK);

	if(!rawInp.InitializeInputDevices(wnd))
		MessageBox(NULL, _T("Call to InitializeInputDevices failed!"), wc.lpszClassName, MB_OK);

	MSG msg {};
	while (GetMessage(&msg, 0, 0, 0))
	{
		rawInp.UpdateTimeStamp(msg.time);
		DispatchMessage(&msg);
	}

	UnregisterClass(wc.lpszClassName, wc.hInstance);
}


RawInp::RawInp(HINSTANCE hInst, MOUSEPROC mouseProc, KEYBOARDPROC kbdProc)
	:
	thrd(&::Input, hInst, std::ref(*this)),
	mouseProc(mouseProc),
	kbdProc(kbdProc),
	prevTime(0),
	curTime(0)
{}

RawInp::~RawInp()
{
	PostMessage(wnd, WM_CLOSE, NULL, NULL);
	thrd.join();
}

bool RawInp::InitializeInputDevices(HWND wnd)
{
	this->wnd = wnd;

	int deviceIndex = 0;
	RAWINPUTDEVICE rid[] = { {},{} };
	if (kbdProc)
	{
		rid[deviceIndex].usUsagePage = 0x01;
		rid[deviceIndex].usUsage = 0x06;
		rid[deviceIndex].dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
		rid[deviceIndex].hwndTarget = wnd;

		++deviceIndex;
	}

	if (mouseProc)
	{
		rid[deviceIndex].usUsagePage = 0x01;
		rid[deviceIndex].usUsage = 0x02;
		rid[deviceIndex].dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
		rid[deviceIndex].hwndTarget = wnd;

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
		uint32_t res = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &outSize, sizeof(RAWINPUTHEADER));
		if (res == 0)
			MessageBox(NULL, _T("Catll to GetRawInputData failed!"), _T("Error"), MB_OK);

		RAWINPUT* rawinput = reinterpret_cast<RAWINPUT*>(buffer);
		if (rawinput->header.dwType == RIM_TYPEKEYBOARD)
			SendKbdProc(rawinput->data.keyboard, curTime - prevTime);
		else if (rawinput->header.dwType == RIM_TYPEMOUSE)
			SendMouseProc(rawinput->data.mouse, curTime - prevTime);

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

void RawInp::SendMouseProc(const RAWMOUSE& mouse, DWORD delay) const
{
	mouseProc(mouse, delay);
}

void RawInp::SendKbdProc(const RAWKEYBOARD& kbd, DWORD delay) const
{
	kbdProc(kbd, delay);
}