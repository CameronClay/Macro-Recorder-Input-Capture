#include "InputData.h"
#include "SimInp.h"
#include "Keys.h"
#include <thread>
#include <chrono>

DelayData::DelayData(DWORD delayMilli)
	:
	delayMilli(delayMilli)
{}

void DelayData::AddDelay(DWORD delay)
{
	delayMilli += delay;
}


MouseClickData::MouseClickData(bool down, bool left, bool right, bool middle)
	:
	down(down),
	left(left),
	right(right),
	middle(middle)
{}

MouseXClickData::MouseXClickData(bool down, bool x1, bool x2)
	:
	down(down),
	x1(x1),
	x2(x2)
{}

MouseMoveData::MouseMoveData(int x, int y, bool absolute)
	:
	x(x),
	y(y),
	absolute(absolute)
{}

MouseScrollData::MouseScrollData(int nClicks)
	:
	nClicks(nClicks)
{}

KbdData::KbdData(WORD key, bool down, bool sc)
	:
	key(key),
	down(down),
	sc(sc)
{}


SaveVisitor::SaveVisitor(std::ostream& os) : os{ os } {}
void SaveVisitor::operator()(DelayData const& d)
{
	os.write((char*)&d.uuid, sizeof(int));
	os.write((char*)&d.delayMilli, sizeof(DWORD));
}
void SaveVisitor::operator()(MouseClickData const& d)
{
	os.write((char*)&d.uuid, sizeof(int));
	os.write((char*)&d.down, sizeof(bool));
	os.write((char*)&d.left, sizeof(bool));
	os.write((char*)&d.right, sizeof(bool));
	os.write((char*)&d.middle, sizeof(bool));
}
void SaveVisitor::operator()(MouseXClickData const& d)
{
	os.write((char*)&d.uuid, sizeof(int));
	os.write((char*)&d.down, sizeof(bool));
	os.write((char*)&d.x1, sizeof(bool));
	os.write((char*)&d.x2, sizeof(bool));
}
void SaveVisitor::operator()(MouseMoveData const& d)
{
	os.write((char*)&d.uuid, sizeof(int));
	os.write((char*)&d.x, sizeof(int));
	os.write((char*)&d.y, sizeof(int));
	os.write((char*)&d.absolute, sizeof(bool));
}
void SaveVisitor::operator()(MouseScrollData const& d)
{
	os.write((char*)&d.uuid, sizeof(int));
	os.write((char*)&d.nClicks, sizeof(int));
}
void SaveVisitor::operator()(KbdData const& d)
{
	os.write((char*)&d.uuid, sizeof(int));
	os.write((char*)&d.key, sizeof(WORD));
	os.write((char*)&d.down, sizeof(bool));
	os.write((char*)&d.sc, sizeof(bool));
}

void SimulateVisitor::operator()(DelayData const& d)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(d.delayMilli));
}
void SimulateVisitor::operator()(MouseClickData const& d)
{
	if (d.down)
		SimInp::SendClickDown(d.left, d.right, d.middle);
	else
		SimInp::SendClickUp(d.left, d.right, d.middle);
}
void SimulateVisitor::operator()(MouseXClickData const& d)
{
	if (d.down)
		SimInp::SendXClickDown(d.x1, d.x2);
	else
		SimInp::SendXClickUp(d.x1, d.x2);
}
void SimulateVisitor::operator()(MouseMoveData const& d)
{
	SimInp::SendMousePosition(d.x, d.y, d.absolute);
}
void SimulateVisitor::operator()(MouseScrollData const& d)
{
	SimInp::SendMouseScroll(d.nClicks);
}
void SimulateVisitor::operator()(KbdData const& d)
{
	if (d.sc)
	{
		if (d.down)
			SimInp::SendKbdDownSC(d.key);
		else
			SimInp::SendKbdUpSC(d.key);
	}
	else
	{
		if (d.down)
			SimInp::SendKbdDown(d.key);
		else
			SimInp::SendKbdUp(d.key);
	}
}

ReadVisitor::ReadVisitor(std::ifstream& is) : is(is) {}

void ReadVisitor::operator()(DelayData const& d)
{
	is.read((char*)&d.delayMilli, sizeof(DWORD));
}
void ReadVisitor::operator()(MouseClickData const& d)
{
	is.read((char*)&d.down, sizeof(bool));
	is.read((char*)&d.left, sizeof(bool));
	is.read((char*)&d.right, sizeof(bool));
	is.read((char*)&d.middle, sizeof(bool));
}
void ReadVisitor::operator()(MouseXClickData const& d)
{
	is.read((char*)&d.down, sizeof(bool));
	is.read((char*)&d.x1, sizeof(bool));
	is.read((char*)&d.x2, sizeof(bool));
}
void ReadVisitor::operator()(MouseMoveData const& d)
{
	is.read((char*)&d.x, sizeof(int));
	is.read((char*)&d.y, sizeof(int));
	is.read((char*)&d.absolute, sizeof(bool));
}
void ReadVisitor::operator()(MouseScrollData const& d)
{
	is.read((char*)&d.nClicks, sizeof(int));
}
void ReadVisitor::operator()(KbdData const& d)
{
	is.read((char*)&d.key, sizeof(WORD));
	is.read((char*)&d.down, sizeof(bool));
	is.read((char*)&d.sc, sizeof(bool));
}
