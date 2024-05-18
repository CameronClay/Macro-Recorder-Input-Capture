#include "InputData.h"
#include "SimInp.h"
#include "Keys.h"
#include <thread>
#include <chrono>

DelayData::DelayData(std::ifstream & is) {
	ReadData(is);
}
DelayData::DelayData(DWORD delayMilli)
	:
	delayMilli(delayMilli)
{}

void DelayData::AddDelay(DWORD delay) {
	delayMilli += delay;
}
void DelayData::ReadData(std::ifstream & is) {
	is.read((char*)&delayMilli, sizeof(DWORD));
}
void DelayData::SaveData(std::ostream& os) const {
	os.write((const char*)&uuid, sizeof(int));
	os.write((const char*)&delayMilli, sizeof(DWORD));
}
void DelayData::Simulate() const {
	std::this_thread::sleep_for(std::chrono::milliseconds(delayMilli));
}

MouseClickData::MouseClickData(std::ifstream & is) {
	ReadData(is);
}

MouseClickData::MouseClickData(bool down, bool left, bool right, bool middle)
	:
	down(down),
	left(left),
	right(right),
	middle(middle)
{}
void MouseClickData::ReadData(std::ifstream& is) {
	is.read((char*)&down, sizeof(bool));
	is.read((char*)&left, sizeof(bool));
	is.read((char*)&right, sizeof(bool));
	is.read((char*)&middle, sizeof(bool));
}
void MouseClickData::SaveData(std::ostream& os) const {
	os.write((const char*)&uuid, sizeof(int));
	os.write((const char*)&down, sizeof(bool));
	os.write((const char*)&left, sizeof(bool));
	os.write((const char*)&right, sizeof(bool));
	os.write((const char*)&middle, sizeof(bool));
}

void MouseClickData::Simulate() const {
	if (down)
		SimInp::SendClickDown(left, right, middle);
	else
		SimInp::SendClickUp(left, right, middle);
}

MouseXClickData::MouseXClickData(std::ifstream& is) {
	ReadData(is);
}
MouseXClickData::MouseXClickData(bool down, bool x1, bool x2)
	:
	down(down),
	x1(x1),
	x2(x2)
{}

void MouseXClickData::ReadData(std::ifstream& is) {
	is.read((char*)&down, sizeof(bool));
	is.read((char*)&x1, sizeof(bool));
	is.read((char*)&x2, sizeof(bool));
}
void MouseXClickData::SaveData(std::ostream & os)const {
	os.write((char*)&uuid, sizeof(int));
	os.write((char*)&down, sizeof(bool));
	os.write((char*)&x1, sizeof(bool));
	os.write((char*)&x2, sizeof(bool));
}

void MouseXClickData::Simulate() const {
	if (down)
		SimInp::SendXClickDown(x1, x2);
	else
		SimInp::SendXClickUp(x1, x2);
}

MouseMoveData::MouseMoveData(std::ifstream& is) {
	ReadData(is);
}
MouseMoveData::MouseMoveData(int x, int y, bool absolute)
	:
	x(x),
	y(y),
	absolute(absolute)
{}

void MouseMoveData::ReadData(std::ifstream& is) {
	is.read((char*)&x, sizeof(int));
	is.read((char*)&y, sizeof(int));
	is.read((char*)&absolute, sizeof(bool));
}
void MouseMoveData::SaveData(std::ostream& os) const {
	os.write((const char*)&uuid, sizeof(int));
	os.write((const char*)&x, sizeof(int));
	os.write((const char*)&y, sizeof(int));
	os.write((const char*)&absolute, sizeof(bool));
}

void MouseMoveData::Simulate() const {
	SimInp::SendMousePosition(x, y, absolute);
}

MouseScrollData::MouseScrollData(std::ifstream& is) {
	ReadData(is);
}

MouseScrollData::MouseScrollData(int nClicks)
	:
	nClicks(nClicks)
{}
void MouseScrollData::ReadData(std::ifstream& is) {
	is.read((char*)&nClicks, sizeof(int));
}
void MouseScrollData::SaveData(std::ostream& os) const {
	os.write((const char*)&uuid, sizeof(int));
	os.write((const char*)&nClicks, sizeof(int));
}

void MouseScrollData::Simulate() const {
	SimInp::SendMouseScroll(nClicks);
}

KbdData::KbdData(std::ifstream& is) {
	ReadData(is);
}
KbdData::KbdData(WORD key, bool down, bool sc, bool E0)
	:
	key(key),
	down(down),
	sc(sc),
	E0(E0)
{}

void KbdData::ReadData(std::ifstream& is) {
	is.read((char*)&key, sizeof(WORD));
	is.read((char*)&down, sizeof(bool));
	is.read((char*)&sc, sizeof(bool));
	is.read((char*)&E0, sizeof(bool));
}
void KbdData::SaveData(std::ostream& os) const {
	os.write((const char*)&uuid, sizeof(int));
	os.write((const char*)&key, sizeof(WORD));
	os.write((const char*)&down, sizeof(bool));
	os.write((const char*)&sc, sizeof(bool));
	os.write((const char*)&E0, sizeof(bool));
}

void KbdData::Simulate() const {
	if (sc)
	{
		if (down)
			SimInp::SendKbdDownSC(key, E0);
		else
			SimInp::SendKbdUpSC(key, E0);
	}
	else
	{
		if (down)
			SimInp::SendKbdDown(key);
		else
			SimInp::SendKbdUp(key);
	}
}