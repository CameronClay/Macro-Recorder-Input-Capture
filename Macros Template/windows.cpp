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

const TCHAR szWindowClass[] = _T("Macros");
const TCHAR szTitle[] = _T("Macros");

const TCHAR DIRECTORY[] = _T("Records");

const TCHAR INSTRUCTIONS[] = _T("| SELECT / TOGGLE_REC - CTRL + F1 | SIM - CTRL + F2 | ADD - CTRL + MENU + A | DEL - CTRL + MENU + D | EXIT - CTRL + DOWN | ");
const TCHAR ADDINGRECORD[] = _T("Adding Record... waiting for key combination");
const TCHAR DELETINGRECORD[] = _T("Deleting Record... waiting for key combination");
const TCHAR RECORDING[] = _T("Recording....");
const TCHAR SIMUALTINGRECORD[] = _T("Simulating Record...");
const TCHAR CURRENTRECORD[] = _T("Current Record = ");

MainWindow::MainWindow(HINSTANCE hInst)
	:
	Window(hInst, WNDPROCP{ {&MainWindow::WndProc}, this })
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
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
		if (!fs::exists(DIRECTORY))
			fs::create_directory(DIRECTORY);

		const std::string dir = fs::current_path().string() + _T('/') + DIRECTORY;
		fs::current_path(dir);

		recordList.Initialize(_T("./"));
		
		rawInput = std::make_unique<RawInp>(hInst, MOUSEPROC{ {&MainWindow::MouseBIProc}, this }, KBDPROC{ {&MainWindow::KbdBIProc}, this });

		outStrings.AddString(INSTRUCTIONS);

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		if (!styles.initialized)
		{
			auto hf = CreateFont(-MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72), 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("Arial"));
			auto hp = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
			auto hb = CreateSolidBrush(RGB(255, 255, 255));
			auto clrb = CreateSolidBrush(RGB(255, 0, 255));

			styles.Initalize(hdc, hp, hb, clrb, hf);
		}

		FillRect(hdc, &ps.rcPaint, styles.hb);

		int yPos = 60;

		auto& strings = outStrings.GetOutStrings();
		outStrings.Lock();
		for (auto& it : strings)
		{
			TextOut(hdc, 0, yPos, it.c_str(), it.size());
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

void MainWindow::MouseBIProc(const RAWMOUSE& mouse, DWORD delay)
{
	if (recordList.IsRecording())
	{
		if (delay != 0)
		{
			Input* ptr = recordList.GetBack();
			if (!ptr->AddDelay(delay))
				recordList.AddEventToRecord<DelayData>(delay);
		}

		if (mouse.usFlags == MOUSE_MOVE_RELATIVE)
		{
			if ((bool)mouse.lLastX || (bool)mouse.lLastY)
				recordList.AddEventToRecord<MouseMoveData>(mouse.lLastX, mouse.lLastY, false);
		}
		else if (mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
		{
			recordList.AddEventToRecord<MouseMoveData>(mouse.lLastX, mouse.lLastY, true);
		}

		if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN)
		{
			recordList.AddEventToRecord<MouseClickData>(true, true, false, false);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP)
		{
			recordList.AddEventToRecord<MouseClickData>(false, true, false, false);
		}

		if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN)
		{
			recordList.AddEventToRecord<MouseClickData>(true, false, true, false);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP)
		{
			recordList.AddEventToRecord<MouseClickData>(false, false, true, false);
		}


		if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
		{
			recordList.AddEventToRecord<MouseScrollData>((short)mouse.usButtonData / WHEEL_DELTA);
		}


		else if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
		{
			recordList.AddEventToRecord<MouseClickData>(true, false, false, true);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP)
		{
			recordList.AddEventToRecord<MouseClickData>(false, false, false, true);
		}

		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) //side button 1
		{
			recordList.AddEventToRecord<MouseXClickData>(true, true, false);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP)
		{
			recordList.AddEventToRecord<MouseXClickData>(false, true, false);
		}

		if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) //side button 2
		{
			recordList.AddEventToRecord<MouseXClickData>(true, false, true);
		}
		else if (mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP)
		{
			recordList.AddEventToRecord<MouseXClickData>(false, false, true);
		}
	}
}

void MainWindow::KbdBIProc(const RAWKEYBOARD& kbd, DWORD delay)
{
	if ((kbd.Message == WM_KEYDOWN) || (kbd.Message == WM_SYSKEYDOWN))
		keys.OnPress(kbd.VKey);
	else if((kbd.Message == WM_KEYUP) || (kbd.Message == WM_SYSKEYUP))
		keys.OnRelease(kbd.VKey);

	//if (/*!(bool)(kbd.Flags & RI_KEY_BREAK) && */(kbd.MakeCode == keys.VirtualKeyToScanCode(VK_TAB)))
	//{
	//	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	//	{
	//		int a = 0;
	//	}
	//}
	//if (/*((kbd.Message == WM_KEYDOWN) || (kbd.Message == WM_SYSKEYDOWN)) && */(kbd.VKey == VK_TAB))
	//{
	//	int a = 0;
	//}

	const int previousRecord = recordList.GetCurrentRecord();

	if (comboRec.GetRecordType() == KeyComboRec::RECORDING)
	{
		if (kbd.Message == WM_KEYDOWN)
		{
			comboRec.AddVKey(kbd.VKey);
			return;
		}
		else if (comboRec.HasRecorded())
		{
			comboRec.Stop();
			if (!recordList.AddRecord(comboRec.GetVKeys()))
			{
				comboRec.StartRecording();
				return;
			}

			outStrings.Lock();
			outStrings.RemoveStringNL(ADDINGRECORD);

			outStrings.RemoveStringNL(CURRENTRECORD + std::to_string(previousRecord));

			outStrings.AddStringNL(CURRENTRECORD + std::to_string(recordList.GetCurrentRecord()));
			outStrings.Unlock();

			Redraw();
			return;
		}
	}
	else if (comboRec.GetRecordType() == KeyComboRec::DELETING)
	{
		if (kbd.Message == WM_KEYDOWN)
		{
			comboRec.AddVKey(kbd.VKey);
			return;
		}
		else if (comboRec.HasRecorded())
		{
			comboRec.Stop();

			if (!recordList.DeleteRecord(comboRec.GetVKeys()))
			{
				return;
			}

			outStrings.Lock();
			outStrings.RemoveStringNL(DELETINGRECORD);

			outStrings.RemoveStringNL(CURRENTRECORD + std::to_string(previousRecord));

			outStrings.AddStringNL(CURRENTRECORD + std::to_string(recordList.GetCurrentRecord()));
			outStrings.Unlock();

			Redraw();
			return;
		}
	}

	// Select Record
	if (keys.IsPressedCombo({ VK_CONTROL, VK_F1 }))
	{
		if (recordList.IsRecording())
		{
			// Remove CTRL + F1 release from list
			recordList.PopBack();
			recordList.PopBack();

			recordList.Save();

			outStrings.RemoveString(RECORDING);
			Redraw();
		}
		else if (recordList.GetCurrentRecord() != RecordList::INVALID)
		{
			ignoreKeys.SetKeys({ { VK_CONTROL, WM_KEYUP, true }, { VK_F1, WM_KEYUP, true } });

			const auto metrics = GetMetricsXY();

			//Set starting mouse position
			POINT pt;
			GetCursorPos(&pt);
			int mx = (pt.x * USHRT_MAX) / metrics.first;
			int my = (pt.y * USHRT_MAX) / metrics.second;

			outStrings.AddString(RECORDING);
			Redraw();

			recordList.StartRecording();
			recordList.AddEventToRecord<MouseMoveData>(mx, my, true);
		}
		return;
	}

	// Simulate Record
	if (keys.IsPressedCombo({ VK_CONTROL, VK_F2 }))
	{
		if (recordList.HasRecorded() && !recordList.IsRecording())
		{
			outStrings.AddString(SIMUALTINGRECORD);
			Redraw();

			recordList.SimulateRecord();

			outStrings.RemoveString(SIMUALTINGRECORD);
			Redraw();
		}
		return;
	}

	// Exit program
	if (keys.IsPressedCombo({ VK_CONTROL, /*VK_ESCAPE*/VK_DOWN })) // for some reason a VK_ESCAPE with WM_KEYDOWN does not reach the message queue even with raw_input
	{
		if (!(recordList.IsRecording() || recordList.IsSimulating()))
		{
			Close();
		}
		return;
	}

	// Add Record
	if (keys.IsPressedCombo({ VK_CONTROL, VK_MENU, Keys::CharToVirtualKey(_T('A')) }))
	{
		outStrings.AddString(ADDINGRECORD);
		Redraw();

		comboRec.StartRecording();
		return;
	}

	// Delete record
	if (keys.IsPressedCombo({ VK_CONTROL, VK_MENU, Keys::CharToVirtualKey(_T('D')) }))
	{
		outStrings.AddString(DELETINGRECORD);
		Redraw();

		comboRec.StartDeleting();
		return;
	}

	if (recordList.SelectRecord(kbd) != RecordList::INVALID)
	{
		if (previousRecord != recordList.GetCurrentRecord())
		{
			outStrings.Lock();
			if (previousRecord != RecordList::INVALID)
				outStrings.RemoveStringNL(CURRENTRECORD + std::to_string(previousRecord));

			outStrings.AddStringNL(CURRENTRECORD + std::to_string(recordList.GetCurrentRecord()));
			outStrings.Unlock();

			Redraw();
		}
	}

	if (recordList.IsRecording())
	{
		if (!ignoreKeys.KeyIgnored(kbd))
		{
			if (delay != 0)
			{
				Input* ptr = recordList.GetBack();
				if (!ptr->AddDelay(delay))
					recordList.AddEventToRecord<DelayData>(delay);
			}

			recordList.AddEventToRecord<KbdData>(kbd.MakeCode, !(bool)(kbd.Flags & RI_KEY_BREAK), true, (bool)(kbd.Flags & RI_KEY_E0));
			//recordList.AddEventToRecord<KbdData>(kbd.VKey, (kbd.Message == WM_KEYDOWN) || (kbd.Message == WM_SYSKEYDOWN), false);
		}
	}
}