#pragma once

#include "Window.h"
#include "Keys.h"
#include "RawInp.h"
#include "SimInp.h"
#include "InputHandler.h"
#include "CheckKey.h"
#include "RecordList.h"
#include "KeyComboRec.h"
#include "StringSet.h"
#include "IgnoreKeys.h"
#include "File.h"
#include "Styles.h"

class MainWindow : public Window
{
public:
	MainWindow(HINSTANCE hInst);

private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void MouseBIProc(const RAWMOUSE& mouse, DWORD delay);
	void KbdBIProc(const RAWKEYBOARD& kbd, DWORD delay);

	Styles styles;

	std::unique_ptr<RawInp> rawInput;

	Keys keys;
	KeyComboRec comboRec;
	RecordList recordList;
	Ignorekeys ignoreKeys;
	StringSet outStrings;
};