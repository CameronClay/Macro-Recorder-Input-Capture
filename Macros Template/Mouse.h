#pragma once

#include <bitset>
#include <Windows.h>

class Mouse
{
public:
	struct Point
	{
		int x = 0, y = 0;
	};
public:
	Mouse();

	bool LeftButtonDown()const
	{
		return flags[ 0 ];
	}
	bool RightButtonDown()const
	{
		return flags[ 1 ];
	}
	bool MiddleButtonDown()const
	{
		return flags[ 2 ];
	}
	bool Button3Down()const
	{
		return flags[ 3 ];
	}
	bool Button4Down()const
	{
		return flags[ 4 ];
	}

	void OnWheelMove( int _delta )
	{
		wheelDelta = _delta;
	}
	void OnButtonUp( unsigned int _flag )
	{
		switch( _flag )
		{
			case RI_MOUSE_LEFT_BUTTON_UP:
				flags[ 0 ] = false;
				break;
			case RI_MOUSE_RIGHT_BUTTON_UP:
				flags[ 1 ] = false;
				break;
			case RI_MOUSE_MIDDLE_BUTTON_UP:
				flags[ 2 ] = false;
				break;
			case RI_MOUSE_BUTTON_4_UP:
				flags[ 3 ] = false;
				break;
			case RI_MOUSE_BUTTON_5_UP:
				flags[ 4 ] = false;
				break;
		}
	}
	void OnButtonDown( unsigned int _flag )
	{	
		switch( _flag )
		{

			case RI_MOUSE_LEFT_BUTTON_DOWN:
				flags[ 0 ] = true;
				break;
			case RI_MOUSE_RIGHT_BUTTON_DOWN:
				flags[ 1 ] = true;
				break;
			case RI_MOUSE_MIDDLE_BUTTON_DOWN:
				flags[ 2 ] = true;
				break;
			case RI_MOUSE_BUTTON_4_DOWN:
				flags[ 3 ] = true;
				break;
			case RI_MOUSE_BUTTON_5_DOWN:
				flags[ 4 ] = true;
				break;
		}
	}
private:
	Point absolute, relative;
	int wheelDelta = 0;
	std::bitset<32> flags;
};

