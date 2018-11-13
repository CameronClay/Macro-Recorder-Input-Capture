#include "InputData.h"
#include "SimInp.h"
#include "Keys.h"
#include <thread>
#include <chrono>

DelayData::DelayData( std::ifstream & is )
{
	ReadData( is );
}

DelayData::DelayData(DWORD delayMilli)
	:
	delayMilli(delayMilli)
{}
void DelayData::AddDelay(DWORD delay)
{
	delayMilli += delay;
}
void DelayData::ReadData( std::ifstream & is )
{
	is.read( ( char* )&delayMilli, sizeof( DWORD ) );
}
void DelayData::SaveData( std::ostream& os )const
{
	os.write( ( const char* )&uuid, sizeof( int ) );
	os.write( ( const char* )&delayMilli, sizeof( DWORD ) );
}

MouseClickData::MouseClickData( std::ifstream & is )
{
	ReadData( is );
}

MouseClickData::MouseClickData(bool down, bool left, bool right, bool middle)
	:
	down(down),
	left(left),
	right(right),
	middle(middle)
{}
void MouseClickData::ReadData( std::ifstream & is )
{
	is.read( ( char* )&down, sizeof( bool ) );
	is.read( ( char* )&left, sizeof( bool ) );
	is.read( ( char* )&right, sizeof( bool ) );
	is.read( ( char* )&middle, sizeof( bool ) );
}
void MouseClickData::SaveData( std::ostream & os )const
{
	os.write( ( const char* )&uuid, sizeof( int ) );
	os.write( ( const char* )&down, sizeof( bool ) );
	os.write( ( const char* )&left, sizeof( bool ) );
	os.write( ( const char* )&right, sizeof( bool ) );
	os.write( ( const char* )&middle, sizeof( bool ) );
}

MouseXClickData::MouseXClickData( std::ifstream & is )
{
	ReadData( is );
}

MouseXClickData::MouseXClickData(bool down, bool x1, bool x2)
	:
	down(down),
	x1(x1),
	x2(x2)
{}
void MouseXClickData::ReadData( std::ifstream& is )
{
	is.read( ( char* )&down, sizeof( bool ) );
	is.read( ( char* )&x1, sizeof( bool ) );
	is.read( ( char* )&x2, sizeof( bool ) );
}
void MouseXClickData::SaveData( std::ostream & os )const
{
	os.write( reinterpret_cast< const char* >( uuid ), sizeof( int ) );
	os.write( reinterpret_cast< const char* >( down ), sizeof( bool ) );
	os.write( reinterpret_cast< const char* >( x1 ), sizeof( bool ) );
	os.write( reinterpret_cast< const char* >( x2 ), sizeof( bool ) );
}

MouseMoveData::MouseMoveData( std::ifstream & is )
{
	ReadData( is );
}

MouseMoveData::MouseMoveData(int x, int y, bool absolute)
	:
	x(x),
	y(y),
	absolute(absolute)
{}
void MouseMoveData::ReadData( std::ifstream & is )
{
	is.read( ( char* )&x, sizeof( int ) );
	is.read( ( char* )&y, sizeof( int ) );
	is.read( ( char* )&absolute, sizeof( bool ) );
}
void MouseMoveData::SaveData( std::ostream& os )const
{
	os.write( ( const char* )&uuid, sizeof( int ) );
	os.write( ( const char* )&x, sizeof( int ) );
	os.write( ( const char* )&y, sizeof( int ) );
	os.write( ( const char* )&absolute, sizeof( bool ) );
}

MouseScrollData::MouseScrollData( std::ifstream & is )
{
	ReadData( is );
}

MouseScrollData::MouseScrollData(int nClicks)
	:
	nClicks(nClicks)
{}
void MouseScrollData::ReadData( std::ifstream& is )
{
	is.read( ( char* )&nClicks, sizeof( int ) );
}
void MouseScrollData::SaveData( std::ostream& os )const
{
	os.write( ( const char* )&uuid, sizeof( int ) );
	os.write( ( const char* )&nClicks, sizeof( int ) );
}

KbdData::KbdData( std::ifstream & is )
{
	ReadData( is );
}

KbdData::KbdData(WORD key, bool down, bool sc)
	:
	key(key),
	down(down),
	sc(sc)
{}
void KbdData::ReadData( std::ifstream& is )
{
	is.read( ( char* )&key, sizeof( WORD ) );
	is.read( ( char* )&down, sizeof( bool ) );
	is.read( ( char* )&sc, sizeof( bool ) );
}
void KbdData::SaveData( std::ostream& os )const
{
	os.write( ( const char* )&uuid, sizeof( int ) );
	os.write( ( const char* )&key, sizeof( WORD ) );
	os.write( ( const char* )&down, sizeof( bool ) );
	os.write( ( const char* )&sc, sizeof( bool ) );
}


//SaveVisitor::SaveVisitor(std::ostream& os) : os{ os } {}
//void SaveVisitor::operator()(DelayData const& d)
//{
//	os.write((char*)&d.uuid, sizeof(int));
//	os.write((char*)&d.delayMilli, sizeof(DWORD));
//}
//void SaveVisitor::operator()(MouseClickData const& d)
//{
//	os.write((char*)&d.uuid, sizeof(int));
//	os.write((char*)&d.down, sizeof(bool));
//	os.write((char*)&d.left, sizeof(bool));
//	os.write((char*)&d.right, sizeof(bool));
//	os.write((char*)&d.middle, sizeof(bool));
//}
//void SaveVisitor::operator()(MouseXClickData const& d)
//{
//	os.write((char*)&d.uuid, sizeof(int));
//	os.write((char*)&d.down, sizeof(bool));
//	os.write((char*)&d.x1, sizeof(bool));
//	os.write((char*)&d.x2, sizeof(bool));
//}
//void SaveVisitor::operator()(MouseMoveData const& d)
//{
//	os.write((char*)&d.uuid, sizeof(int));
//	os.write((char*)&d.x, sizeof(int));
//	os.write((char*)&d.y, sizeof(int));
//	os.write((char*)&d.absolute, sizeof(bool));
//}
//void SaveVisitor::operator()(MouseScrollData const& d)
//{
//	os.write((char*)&d.uuid, sizeof(int));
//	os.write((char*)&d.nClicks, sizeof(int));
//}
//void SaveVisitor::operator()(KbdData const& d)
//{
//	os.write((char*)&d.uuid, sizeof(int));
//	os.write((char*)&d.key, sizeof(WORD));
//	os.write((char*)&d.down, sizeof(bool));
//	os.write((char*)&d.sc, sizeof(bool));
//}

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

//ReadVisitor::ReadVisitor(std::ifstream& is) : is(is) {}
//
//void ReadVisitor::operator()(DelayData const& d)
//{
//	is.read((char*)&d.delayMilli, sizeof(DWORD));
//}
//void ReadVisitor::operator()(MouseClickData const& d)
//{
//	is.read((char*)&d.down, sizeof(bool));
//	is.read((char*)&d.left, sizeof(bool));
//	is.read((char*)&d.right, sizeof(bool));
//	is.read((char*)&d.middle, sizeof(bool));
//}
//void ReadVisitor::operator()(MouseXClickData const& d)
//{
//	is.read((char*)&d.down, sizeof(bool));
//	is.read((char*)&d.x1, sizeof(bool));
//	is.read((char*)&d.x2, sizeof(bool));
//}
//void ReadVisitor::operator()(MouseMoveData const& d)
//{
//	is.read((char*)&d.x, sizeof(int));
//	is.read((char*)&d.y, sizeof(int));
//	is.read((char*)&d.absolute, sizeof(bool));
//}
//void ReadVisitor::operator()(MouseScrollData const& d)
//{
//	is.read((char*)&d.nClicks, sizeof(int));
//}
//void ReadVisitor::operator()(KbdData const& d)
//{
//	is.read((char*)&d.key, sizeof(WORD));
//	is.read((char*)&d.down, sizeof(bool));
//	is.read((char*)&d.sc, sizeof(bool));
//}
