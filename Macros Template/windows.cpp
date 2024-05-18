#ifndef NOMINMAX
#define NOMINMAX
#endif

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
#include "Text.h"

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <time.h>
#include <memory>
#include <assert.h>
#include <string>
#include "Windows.h"

#pragma comment(lib, "Winmm.lib")

MainWindow::MainWindow(HINSTANCE hInst)
	:
	Window(hInst, WNDPROCP::Function( &MainWindow::WndProc, this ))
{}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	MainWindow window{ hInstance };

	window.CreateFullscreen(szWindowClass, szTitle, true);
	window.SetLayeredAttrib(255);

	MSG msg = {};
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case WM_CREATE:
	{
		// Create Records directory if it does not exist
		if (!fs::exists(DIRECTORY))
			fs::create_directory(DIRECTORY);

		const std::string dir = fs::current_path().string() + _T('/') + DIRECTORY;
		fs::current_path(dir);

		recordList.Initialize(_T("./"));
		
		rawInput = std::make_unique<RawInp>(hInst, MOUSEPROC{ &MainWindow::MouseBIProc, this }, KBDPROC{ &MainWindow::KbdBIProc, this });

		outStrings.AddString(Text::INSTRUCTIONS);

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		if (!styles.initialized) {
			auto hf = CreateFont(-MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("Arial"));
			auto hp = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			auto hb = CreateSolidBrush(RGB(255, 255, 255));
			auto clrb = CreateSolidBrush(RGB(255, 0, 255));

			styles.Initalize(hdc, hp, hb, clrb, hf, BACK_COLOR, TEXT_COLOR);
		}

		FillRect(hdc, &ps.rcPaint, styles.hb);

		outStrings.Lock();
		const auto& strings = outStrings.GetOutStrings();

		int yPos = 60;
		for (const auto& it : strings) {
			TextOut(hdc, 0, yPos, it.c_str(), static_cast<int>(it.size()));
			yPos += 30;
		}
		outStrings.Unlock();

		SelectBrush(hdc, styles.clrb);

		EndPaint(hWnd, &ps);
	}	break;

	case WM_DESTROY:
		styles.Cleanup();

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void MainWindow::MouseBIProc(const RAWMOUSE& mouse, DWORD delay) {
	if (recordList.IsRecording()) {
		if (delay != 0) {
			Input* ptr = recordList.GetBack();
			if (!ptr->AddDelay(static_cast<float>(delay)))
				recordList.AddEventToRecord<DelayData>(delay);
		}

		if (mouse.usFlags == MOUSE_MOVE_RELATIVE) {
			if (static_cast<bool>(mouse.lLastX) || static_cast<bool>(mouse.lLastY))
				recordList.AddEventToRecord<MouseMoveData>(mouse.lLastX, mouse.lLastY, false);
		}
		else if (mouse.usFlags & MOUSE_MOVE_ABSOLUTE) {
			recordList.AddEventToRecord<MouseMoveData>(mouse.lLastX, mouse.lLastY, true);
		}

		if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
			recordList.AddEventToRecord<MouseClickData>(true, true, false, false);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
			recordList.AddEventToRecord<MouseClickData>(false, true, false, false);
		}

		if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
			recordList.AddEventToRecord<MouseClickData>(true, false, true, false);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
			recordList.AddEventToRecord<MouseClickData>(false, false, true, false);
		}


		if (mouse.usButtonFlags & RI_MOUSE_WHEEL) {
			recordList.AddEventToRecord<MouseScrollData>(static_cast<SHORT>(mouse.usButtonData) / WHEEL_DELTA);
		}


		else if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
			recordList.AddEventToRecord<MouseClickData>(true, false, false, true);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
			recordList.AddEventToRecord<MouseClickData>(false, false, false, true);
		}

		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) { //side button 1
			recordList.AddEventToRecord<MouseXClickData>(true, true, false);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
			recordList.AddEventToRecord<MouseXClickData>(false, true, false);
		}

		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) { //side button 2
			recordList.AddEventToRecord<MouseXClickData>(true, false, true);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
			recordList.AddEventToRecord<MouseXClickData>(false, false, true);
		}
	}
}

//Handle key presses and menu transitions
void MainWindow::KbdBIProc(const RAWKEYBOARD& kbd, DWORD delay) {
	if ((kbd.Message == WM_KEYDOWN) || (kbd.Message == WM_SYSKEYDOWN))
		keys.OnPress(static_cast<unsigned char>(kbd.VKey));
	else if((kbd.Message == WM_KEYUP) || (kbd.Message == WM_SYSKEYUP))
		keys.OnRelease(static_cast<unsigned char>(kbd.VKey));

	const int previousRecord = recordList.GetCurrentRecord();

	//change record display strings
	const auto strings_crec = [&]() {
		if (previousRecord != RecordList::INVALID) {
			outStrings.RemoveStringNL(Text::CURRENT_RECORD + std::to_string(previousRecord));
		}

		const int newRecord = recordList.GetCurrentRecord();
		if (newRecord != RecordList::INVALID) {
			outStrings.AddStringNL(Text::CURRENT_RECORD + std::to_string(recordList.GetCurrentRecord()));
		}
	};

	if (comboRec.GetRecordType() == KeyComboRec::RECORDING) { // If is recording
		if (kbd.Message == WM_KEYDOWN) { //if key was pressed add key to combo
			comboRec.AddVKey(static_cast<TCHAR>(kbd.VKey));
			return;
		}
		else if (comboRec.HasRecorded()) {
			comboRec.Stop();
			//add record with toggle keys
			if (!recordList.AddRecord(comboRec.GetVKeys())) { // If record already exists start recording
				comboRec.StartRecording();
			}
			else { //if record does not already exist
				outStrings.RemoveStringNL(Text::ADDING_RECORD);
				strings_crec();

				Redraw();
			}

			return;
		}
	}
	else if (comboRec.GetRecordType() == KeyComboRec::DELETING) { // If is deleting
		if (kbd.Message == WM_KEYDOWN) { //if key was pressed add key to combo
			comboRec.AddVKey(static_cast<TCHAR>(kbd.VKey));
			return;
		}
		else if (comboRec.HasRecorded()) {
			comboRec.Stop();

			if (!recordList.DeleteRecord(comboRec.GetVKeys())) {
				return;
			}

			outStrings.RemoveStringNL(Text::DELETING_RECORD);
			strings_crec();

			Redraw();
			return;
		}
	}

	// Toggle Recording
	if (keys.IsPressedCombo({ VK_CONTROL, VK_F1 })) {
		//If is recording, stop recording
		if (recordList.IsRecording()) {
			// Remove CTRL + F1 key combo release from list
			recordList.PopBack();
			recordList.PopBack();
			recordList.Save();

			outStrings.RemoveString(Text::RECORDING);
			Redraw();
		}
		//if a record is selected, start recording
		else if (recordList.GetCurrentRecord() != RecordList::INVALID) {
			ignoreKeys.SetKeys({ { VK_CONTROL, WM_KEYUP, true }, { VK_F1, WM_KEYUP, true } });

			const auto metrics = GetMetricsXY();

			//Set starting mouse position
			POINT pt;
			GetCursorPos(&pt);
			const int mx = (pt.x * USHRT_MAX) / metrics.first;
			const int my = (pt.y * USHRT_MAX) / metrics.second;

			outStrings.AddString(Text::RECORDING);
			Redraw();

			recordList.StartRecording();
			recordList.AddEventToRecord<MouseMoveData>(mx, my, true);
		}
		return;
	}

	// Simulate Record
	if (keys.IsPressedCombo({ VK_CONTROL, VK_F2 })) {
		if (recordList.HasRecorded() && !recordList.IsRecording()) {
			outStrings.AddString(Text::SIMUALTING_RECORD);
			Redraw();

			recordList.SimulateRecord();

			outStrings.RemoveString(Text::SIMUALTING_RECORD);
			Redraw();
		}
		return;
	}

	// Add Record
	if (keys.IsPressedCombo({ VK_CONTROL, VK_MENU, Keys::CharToVirtualKey(_T('A')) })) {
		outStrings.AddString(Text::ADDING_RECORD);
		outStrings.RemoveStringNL(Text::DELETING_RECORD);
		Redraw();

		comboRec.StartRecording();
		return;
	}

	// Delete record
	if (keys.IsPressedCombo({ VK_CONTROL, VK_MENU, Keys::CharToVirtualKey(_T('D')) })) {
		outStrings.AddString(Text::DELETING_RECORD);
		outStrings.RemoveStringNL(Text::ADDING_RECORD);
		Redraw();

		comboRec.StartDeleting();
		return;
	}

	// Exit program
	if (keys.IsPressedCombo({ VK_CONTROL, /*VK_ESCAPE*/VK_DOWN })) { // For some reason a VK_ESCAPE with WM_KEYDOWN does not reach the message queue even with raw_input
		if (!(recordList.IsRecording() || recordList.IsSimulating())) {
			Close();
		}
		return;
	}

	// Select record
	if (recordList.SelectRecord(kbd) != RecordList::INVALID) {
		if (previousRecord != recordList.GetCurrentRecord()) {
			strings_crec();

			Redraw();
		}
	}

	// If recording
	if (recordList.IsRecording()) {
		if (!ignoreKeys.KeyIgnored(kbd)) { // And key is not ignored (keys used to access menu)
			if (delay != 0) {
				Input* ptr = recordList.GetBack();
				if (ptr && !ptr->AddDelay(static_cast<float>(delay)))
					recordList.AddEventToRecord<DelayData>(delay);
			}

			//add scan code (key press) to recordList
			recordList.AddEventToRecord<KbdData>(kbd.MakeCode, !static_cast<bool>(kbd.Flags & RI_KEY_BREAK), true, static_cast<bool>(kbd.Flags & RI_KEY_E0));
		}
	}
}