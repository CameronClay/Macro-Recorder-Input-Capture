#include "RawInp.h"
#include <tchar.h>
#include <assert.h>

static LRESULT CALLBACK RawInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INPUT:
		DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

static void Input(HINSTANCE hInst, RawInp& rawInp)
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInst;
	wc.lpfnWndProc = RawInputProc;
	wc.lpszClassName = _T("RAW_INPUT");

	if (!RegisterClassEx(&wc))
		MessageBox(NULL, _T("Call to RegisterClass failed!"), wc.lpszClassName, MB_OK);

	HWND wnd = CreateWindowEx(NULL, wc.lpszClassName, _T("RAW_INPUT"), NULL, 0, 0, 0, 0, NULL, NULL, wc.hInstance, nullptr);
	if (!wnd)
		MessageBox(NULL, _T("Call to CreateWindowEX failed!"), wc.lpszClassName, MB_OK);

	if(!rawInp.InitializeInputDevices(wnd))
		MessageBox(NULL, _T("Call to InitializeInputDevices failed!"), wc.lpszClassName, MB_OK);

	static constexpr uint32_t buffSize = 128;
	char buffer[buffSize];
	DWORD prevTime = 0;

	MSG msg = {};
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (msg.message == WM_INPUT)
		{
			uint32_t outSize = buffSize;
			uint32_t res = GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, buffer, &outSize, sizeof(RAWINPUTHEADER));
			if (res == 0)
				MessageBox(NULL, _T("Error at GetRawInputData, res is 0"), _T("Error"), MB_OK);

			RAWINPUT* rawinput = reinterpret_cast<RAWINPUT*>(buffer);
			if (rawinput->header.dwType == RIM_TYPEKEYBOARD)
				rawInp.SendKbdProc(rawinput->data.keyboard, msg.time - prevTime);
			else if(rawinput->header.dwType == RIM_TYPEMOUSE)
				rawInp.SendMouseProc(rawinput->data.mouse, msg.time - prevTime);
			else
				MessageBox(NULL, _T("Error at GetRawInputData, invalid rawinput device type"), _T("Error"), MB_OK);

			if (prevTime > msg.time)
				MessageBox(NULL, _T("Error times reordered"), _T("Error"), MB_OK);
				
			prevTime = msg.time;
		}
		DispatchMessage(&msg);
	}

	UnregisterClass(wc.lpszClassName, wc.hInstance);
}


RawInp::RawInp(HINSTANCE hInst, MouseProc mouseProc, KeyboardProc kbdProc)
	:
	thrd(&::Input, hInst, std::ref(*this)),
	mouseProc(mouseProc),
	kbdProc(kbdProc)
{}

RawInp::~RawInp()
{
	PostMessage(wnd, WM_CLOSE, NULL, NULL);
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


void RawInp::SendMouseProc(const RAWMOUSE& mouse, DWORD delay)
{
	mouseProc(mouse, delay);
}

void RawInp::SendKbdProc(const RAWKEYBOARD& kbd, DWORD delay)
{
	kbdProc(kbd, delay);
}