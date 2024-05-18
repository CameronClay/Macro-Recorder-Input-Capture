#pragma once
#include <windows.h>
#include <tuple>
#include "Function.h"

using WNDPROCP = Function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

class Window
{
public:
	friend LRESULT CALLBACK WndProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Window(HINSTANCE hInst, WNDPROCP wndProc);
	~Window();

	static void MsgBox(LPCTSTR msg, LPCTSTR caption = NULL, UINT type = MB_OK);

	bool Create(int left, int top, int width, int height, LPCSTR wndClss, LPCSTR title, bool hidden = false, bool layered = false);
	bool CreateFullscreen(LPCSTR wndClss, LPCSTR title, bool layered = false);
	bool SetLayeredAttrib(BYTE alpha);
	void Close();

	void Redraw();

	HWND GetHWND() const;
	RECT GetRect() const;
	std::pair<int, int> GetMetricsXY() const;

	bool IsActive() const;
	bool IsMinimized() const;

protected:
	HINSTANCE hInst;
	HWND hWnd;
	LPCSTR wndClss;
	WNDPROCP wndProc;
};