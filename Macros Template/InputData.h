#pragma once
#include <fstream>
#include <Windows.h>
#include <variant>
#include <optional>
struct DelayData
{
	static constexpr int uuid = 0;
	DelayData( std::ifstream& is );
	DelayData(DWORD delayMilli = 0);

	void AddDelay(DWORD delay);
	void ReadData( std::ifstream& is );
	void SaveData( std::ostream& os )const;
	void Simulate()const;

	DWORD delayMilli = 0;
};

struct MouseClickData
{
	static constexpr int uuid = 1;
	MouseClickData( std::ifstream& is );
	MouseClickData(bool down, bool left, bool right, bool middle);
	MouseClickData() = default;
	void ReadData( std::ifstream& is );
	void SaveData( std::ostream& os )const;
	void Simulate()const;

	bool down = false, left = false, right = false, middle = false;
};

struct MouseXClickData
{
	static constexpr int uuid = 2;
	MouseXClickData( std::ifstream& is );
	MouseXClickData(bool down, bool x1, bool x2);
	MouseXClickData() = default;
	void ReadData( std::ifstream& is );
	void SaveData( std::ostream& os )const;
	void Simulate()const;

	bool down = false, x1 = false, x2 = false;
};

struct MouseMoveData
{
	static constexpr int uuid = 3;
	MouseMoveData( std::ifstream& is );
	MouseMoveData(int x, int y, bool absolute);
	MouseMoveData() = default;

	void ReadData( std::ifstream& is );
	void SaveData( std::ostream& os )const;
	void Simulate()const;
	
	int x = 0, y = 0;
	bool absolute = false;
};

struct MouseScrollData
{
	static constexpr int uuid = 4;
	MouseScrollData( std::ifstream& is );
	MouseScrollData(int nClicks);
	MouseScrollData() = default;

	void ReadData( std::ifstream& is );
	void SaveData( std::ostream& os )const;
	void Simulate()const;

	int nClicks = 0;
};

struct KbdData
{
	static constexpr int uuid = 5;
	KbdData( std::ifstream& is );
	KbdData(WORD key, bool down, bool sc);
	KbdData() = default;

	void ReadData( std::ifstream& is );
	void SaveData( std::ostream& os )const;
	void Simulate()const;

	WORD key = 0;
	bool down = false, sc = false;
};


using InputData = std::variant<DelayData, MouseClickData, MouseXClickData, MouseMoveData, MouseScrollData, KbdData>;

class Input
{
public:
	Input() = default;
	template<typename T>
	Input( T&& _value )
		:
		data( std::move( _value ) )
	{}
	void ReadData( std::ifstream& is )
	{
		auto read_data = [ & ]( auto& _data )
		{
			_data.ReadData( is );
		};

		std::visit( read_data, data );
	}
	void SaveData( std::ostream& os )const
	{
		auto write_data = [ & ]( const auto& _data )
		{
			_data.SaveData( os );
		};

		std::visit( write_data, data );
	}
	void Simulate()const
	{
		auto simulate = []( const auto& _data )
		{
			_data.Simulate();
		};

		std::visit( simulate, data );
	}
	bool AddDelay( float _delay )
	{
		auto add_delay = [ & ]( auto& _data )
		{
			using type = std::decay_t<decltype( _data )>;
			if constexpr( std::is_same_v<type, DelayData> )
			{
				_data.AddDelay( _delay );
				return true;
			}

			return false;
		};

		return std::visit( add_delay, data );
	}
	template<typename T, typename IfSame, typename IfNotSame>
	void ConditionalCall( IfSame&& _ifsame, IfNotSame&& _ifnotsame )
	{
		auto call = [ & ]( auto& _data )
		{
			using type = std::decay_t<decltype( _data )>;
			if constexpr( std::is_same_v<type, T> )
			{
				_ifsame(_data);
			}
			else
			{
				_ifnotsame();
			}
		};

		std::visit( call, data );
	}
private:
	InputData data;
};