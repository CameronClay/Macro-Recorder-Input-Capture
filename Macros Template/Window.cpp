#include "Window.h"
#include <tchar.h>

static LRESULT CALLBACK WndProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* pThis = nullptr;

	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pThis = static_cast<Window*>(lpcs->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
		return pThis->wndProc(hWnd, message, wParam, lParam);

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::Window(HINSTANCE hInst, WNDPROCP wndProc)
	:
	hInst(hInst),
	hWnd(NULL),
	wndClss(),
	wndProc(wndProc)
{}

Window::~Window()
{
	if(wndClss)
		UnregisterClass(wndClss, hInst);
}

void Window::MsgBox(LPCTSTR msg, LPCTSTR caption, UINT type)
{
	MessageBox(NULL, msg, caption, type);
}

bool Window::Create(int left, int top, int width, int height, LPCSTR wndClss, LPCSTR title, bool hidden, bool layered)
{
	this->wndClss = wndClss;

	WNDCLASSEX wcex {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = hInst;
	wcex.lpszClassName = wndClss;
	wcex.lpfnWndProc = WndProcCallback;
	if (!hidden)
	{
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	}

	if (!RegisterClassEx(&wcex))
	{
		Window::MsgBox(_T("Call to RegisterClassEx failed!"), title);
		return false;
	}


	const DWORD exStyle = layered ? (WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST) : 0UL;
	const DWORD style = !hidden ? (WS_POPUPWINDOW) : 0UL;

	hWnd = CreateWindowEx(
		exStyle,
		wcex.lpszClassName,
		title,
		style,
		left, top,
		width, height,
		NULL,
		NULL,
		wcex.hInstance,
		reinterpret_cast<LPVOID>(this));

	if (!hWnd)
	{
		Window::MsgBox(_T("Call to CreateWindow failed!"), title);
		int err = GetLastError();
		return false;
	}

	if (!hidden)
	{
		ShowWindow(hWnd, SW_MAXIMIZE);
		UpdateWindow(hWnd);
	}

	return true;
}
bool Window::CreateFullscreen(LPCSTR wndClss, LPCSTR title, bool layered)
{
	const int width  = GetSystemMetrics(SM_CXVIRTUALSCREEN),
			  height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

	if (!width || !height)
		return false;

	return Create(
		0,
		0,
		width,
		height,
		wndClss, title, 
		false, layered);
}

bool Window::SetLayeredAttrib(BYTE alpha)
{
	COLORREF colorKey = GetSysColor(COLOR_WINDOW);
	return SetLayeredWindowAttributes(hWnd, colorKey, alpha, LWA_COLORKEY | LWA_ALPHA);
}


void Window::Close()
{
	PostMessage(hWnd, WM_CLOSE, NULL, NULL);
}

void Window::Redraw()
{
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_INTERNALPAINT);
}

HWND Window::GetHWND() const
{
	return hWnd;
}

RECT Window::GetRect() const
{
	RECT rect{};
	GetWindowRect(hWnd, &rect);
	return rect;
}

std::tuple<int, int> Window::GetMetricsXY() const
{
	RECT rect = GetRect();

	return { rect.right - rect.left, rect.bottom - rect.top };
}
