#include "SimInp.h"
#include "Keys.h"
#include <stdlib.h>
#include <memory>
#include <thread>
#include <chrono>

//Simulate Keyboard functions
bool SimInp::SendKbdDown(WORD key)
{
	INPUT input{ INPUT_KEYBOARD };
	input.ki = { key, NULL, NULL, 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool SimInp::SendKbdUp(WORD key)
{
	INPUT input{ INPUT_KEYBOARD };
	input.ki = { key, NULL, KEYEVENTF_KEYUP, 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool SimInp::SendKbd(WORD key, DWORD delayMilli)
{
	if (delayMilli)
	{
		bool res = SendKbdDown(key);
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMilli));
		return res & SendKbdUp(key);
	}
	else
	{
		INPUT input[]{ { INPUT_KEYBOARD }, { INPUT_KEYBOARD } };
		input[0].ki = { (WORD)key, NULL, NULL, 0, NULL };
		input[1].ki = { (WORD)key, NULL, KEYEVENTF_KEYUP, 0, NULL };
		return SendInput(2, input, sizeof(INPUT)) == 2;
	}
}
void SimInp::KeyCombo(std::initializer_list<TCHAR> keys)
{
	const auto size = keys.size();
	std::unique_ptr<INPUT[]> input = std::make_unique<INPUT[]>(size);
	for (auto i = 0; i < size; i++)
	{
		input[i].type = INPUT_KEYBOARD;
		input[i].ki = { (WORD)*(keys.begin() + i), NULL, NULL, 0, NULL };
	}
	SendInput(size, input.get(), sizeof(INPUT));

	for (auto i = 0; i < size; i++)
		input[i].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(size, input.get(), sizeof(INPUT));
}

void SimInp::KeyCombo(const std::vector<TCHAR>& keys)
{
	const auto size = keys.size();
	std::unique_ptr<INPUT[]> input = std::make_unique<INPUT[]>(size);
	for (auto i = 0; i < size; i++)
	{
		input[i].type = INPUT_KEYBOARD;
		input[i].ki = { (WORD)*(keys.begin() + i), NULL, NULL, 0, NULL };
	}
	SendInput(size, input.get(), sizeof(INPUT));

	for (auto i = 0; i < size; i++)
		input[i].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(size, input.get(), sizeof(INPUT));
}


void SimInp::SendKbd(const TCHAR* str, int len)
{
	for (UINT i = 0; i < len; i++)
		SimInp::SendKbd(keys.CharToVirtualKey(str[i]));
}

bool SimInp::SendKbdDownSC(WORD key)
{
	INPUT input{ INPUT_KEYBOARD };
	input.ki = { NULL, key, KEYEVENTF_SCANCODE, 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool SimInp::SendKbdUpSC(WORD key)
{
	INPUT input{ INPUT_KEYBOARD };
	input.ki = { NULL, key, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool SimInp::SendKbdSC(WORD key, DWORD delayMilli)
{
	if (delayMilli)
	{
		bool res = SendKbdDownSC(key);
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMilli));
		return res & SendKbdUpSC(key);
	}
	else
	{
		INPUT input[]{ { INPUT_KEYBOARD }, { INPUT_KEYBOARD } };
		input[0].ki = { NULL, key, KEYEVENTF_SCANCODE, 0, NULL };
		input[1].ki = { NULL, key, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0, NULL };
		return SendInput(2, input, sizeof(INPUT)) == 2;
	}
}

void SimInp::SendKbdSC(const TCHAR* str, int len)
{
	for (UINT i = 0; i < len; i++)
		SimInp::SendKbdSC(keys.CharToScanCode(str[i]));
}

void SimInp::KeyComboSC(std::initializer_list<WORD> keys)
{
	const auto size = keys.size();
	std::unique_ptr<INPUT[]> input = std::make_unique<INPUT[]>(size);
	for (auto i = 0; i < size; i++)
	{
		input[i].type = INPUT_KEYBOARD;
		input[i].ki = { NULL, *(keys.begin() + i), KEYEVENTF_SCANCODE, 0, NULL };
	}
	SendInput(size, input.get(), sizeof(INPUT));

	for (auto i = 0; i < size; i++)
		input[i].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

	SendInput(size, input.get(), sizeof(INPUT));
}

void SimInp::KeyComboSC(const std::vector<WORD>& keys)
{
	const auto size = keys.size();
	std::unique_ptr<INPUT[]> input = std::make_unique<INPUT[]>(size);
	for (auto i = 0; i < size; i++)
	{
		input[i].type = INPUT_KEYBOARD;
		input[i].ki = { NULL, *(keys.begin() + i), KEYEVENTF_SCANCODE, 0, NULL };
	}
	SendInput(size, input.get(), sizeof(INPUT));

	for (auto i = 0; i < size; i++)
		input[i].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

	SendInput(size, input.get(), sizeof(INPUT));
}



//Simulate Mouse functions
bool SimInp::SendClickDown(bool left, bool right, bool middle)
{
	INPUT input{ INPUT_MOUSE };
	input.mi = { 0, 0, NULL, (DWORD)(left ? MOUSEEVENTF_LEFTDOWN : NULL) | (right ? MOUSEEVENTF_RIGHTDOWN : NULL) | (middle ? MOUSEEVENTF_MIDDLEDOWN : NULL), 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool SimInp::SendClickUp(bool left, bool right, bool middle)
{
	INPUT input{ INPUT_MOUSE };
	input.mi = { 0, 0, NULL, (DWORD)(left ? MOUSEEVENTF_LEFTUP : NULL) | (right ? MOUSEEVENTF_RIGHTUP : NULL) | (middle ? MOUSEEVENTF_MIDDLEUP : NULL), 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool SimInp::SendClick(bool left, bool right, bool middle, DWORD delayMilli)
{
	if (delayMilli)
	{
		bool res = SendClickDown(left, right, middle);
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMilli));
		return res & SendClickUp(left, right, middle);
	}
	else
	{
		INPUT input[]{ { INPUT_MOUSE }, { INPUT_MOUSE } };
		input[0].mi = { 0, 0, NULL, (DWORD)(left ? MOUSEEVENTF_LEFTDOWN : NULL) | (right ? MOUSEEVENTF_RIGHTDOWN : NULL) | (middle ? MOUSEEVENTF_MIDDLEDOWN : NULL), 0, NULL };
		input[1].mi = { 0, 0, NULL, (DWORD)(left ? MOUSEEVENTF_LEFTUP : NULL) | (right ? MOUSEEVENTF_RIGHTUP : NULL) | (middle ? MOUSEEVENTF_MIDDLEUP : NULL), 0, NULL };

		return SendInput(2, input, sizeof(INPUT)) == 2;
	}
}


bool SimInp::SendXClickDown(bool xButton1, bool xButton2)
{
	INPUT input{ INPUT_MOUSE };
	input.mi = { 0, 0, (DWORD)(xButton1 ? XBUTTON1 : NULL) | (xButton2 ? XBUTTON2 : NULL), MOUSEEVENTF_XDOWN, 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool SimInp::SendXClickUp(bool xButton1, bool xButton2)
{
	INPUT input{ INPUT_MOUSE };
	input.mi = { 0, 0, (DWORD)(xButton1 ? XBUTTON1 : NULL) | (xButton2 ? XBUTTON2 : NULL), MOUSEEVENTF_XUP, 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool SimInp::SendXClick(bool xButton1, bool xButton2, DWORD delayMilli)
{
	if (delayMilli)
	{
		bool res = SendXClickDown(xButton1, xButton2);
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMilli));
		return res & SendXClickUp(xButton1, xButton2);
	}
	else
	{
		const DWORD data = (xButton1 ? XBUTTON1 : NULL) | (xButton2 ? XBUTTON2 : NULL);
		INPUT input[]{ { INPUT_MOUSE }, { INPUT_MOUSE } };
		input[0].mi = { 0, 0, data, MOUSEEVENTF_XDOWN, 0, NULL };
		input[1].mi = { 0, 0, data, MOUSEEVENTF_XUP, 0, NULL };
	}
}

bool SimInp::SendMousePosition(int x, int y, bool absolute)
{
	INPUT input{ INPUT_MOUSE };
	input.mi = { x, y, NULL, (DWORD)(absolute ? MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE : MOUSEEVENTF_MOVE), 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}

void SimInp::MouseMove(int x, int y, DWORD duration, DWORD freq)
{
	const DWORD nLoops = ((float)duration / (float)freq) + 0.5f;
	const float iX = (float)x / (float)nLoops,
		iY = (float)y / (float)nLoops;
	float xMove = 0.0f, yMove = 0.0f;

	for (DWORD i = 1; i <= nLoops; i++)
	{
		if ((int)(xMove += iX) || (int)(yMove += iY))
		{
			SimInp::SendMousePosition((int)xMove, (int)yMove);
			xMove -= (int)xMove;
			yMove -= (int)yMove;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(freq));
	}
}


bool SimInp::SendMouseScroll(int nClicks)
{
	INPUT input{ INPUT_MOUSE };
	input.mi = { 0, 0, (DWORD)(nClicks * WHEEL_DELTA), MOUSEEVENTF_WHEEL, 0, NULL };
	return SendInput(1, &input, sizeof(INPUT)) == 1;
}