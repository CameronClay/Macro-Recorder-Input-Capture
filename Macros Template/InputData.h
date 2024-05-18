#pragma once
#include <fstream>
#include <Windows.h>
#include <variant>

class DelayData
{
public:
	static constexpr int uuid = 0;
	DelayData(std::ifstream& is);
	DelayData(DWORD delayMilli = 0);

	void AddDelay(DWORD delay);
	void ReadData(std::ifstream& is);
	void SaveData(std::ostream& os) const;
	void Simulate() const;

private:
	DWORD delayMilli = 0;
};

class MouseClickData
{
public:
	static constexpr int uuid = 1;
	MouseClickData(std::ifstream& is);
	MouseClickData(bool down, bool left, bool right, bool middle);
	MouseClickData() = default;

	void ReadData(std::ifstream& is);
	void SaveData(std::ostream& os) const;
	void Simulate() const;

private:
	bool down = false, left = false, right = false, middle = false;
};

class MouseXClickData
{
public:
	static constexpr int uuid = 2;
	MouseXClickData(std::ifstream& is);
	MouseXClickData(bool down, bool x1, bool x2);
	MouseXClickData() = default;

	void ReadData(std::ifstream& is);
	void SaveData(std::ostream& os) const;
	void Simulate() const;

private:
	bool down = false, x1 = false, x2 = false;
};

class MouseMoveData
{
public:
	static constexpr int uuid = 3;
	MouseMoveData(std::ifstream& is);
	MouseMoveData(int x, int y, bool absolute);
	MouseMoveData() = default;

	void ReadData(std::ifstream& is);
	void SaveData(std::ostream& os) const;
	void Simulate() const;

private:
	int x = 0, y = 0;
	bool absolute = false;
};

class MouseScrollData
{
public:
	static constexpr int uuid = 4;
	MouseScrollData(std::ifstream& is);
	MouseScrollData(int nClicks);
	MouseScrollData() = default;

	void ReadData(std::ifstream& is);
	void SaveData(std::ostream& os) const;
	void Simulate() const;

private:
	int nClicks = 0;
};

class KbdData
{
public:
	static constexpr int uuid = 5;
	KbdData(std::ifstream& is);
	KbdData(WORD key, bool down, bool sc, bool E0);
	KbdData() = default;

	void ReadData(std::ifstream& is);
	void SaveData(std::ostream& os) const;
	void Simulate() const;

private:
	WORD key = 0;
	bool down = false, sc = false, E0 = false;
};

using InputData = std::variant<DelayData, MouseClickData, MouseXClickData, MouseMoveData, MouseScrollData, KbdData>;

class Input
{
public:
	Input() = default;
	template<typename T>
	Input(T&& _value)
		:
		data(std::move(_value))
	{}

	void ReadData(std::ifstream& is) {
		auto read_data = [&](auto& _data) {
			_data.ReadData(is);
		};

		std::visit(read_data, data);
	}
	void SaveData(std::ostream& os) const {
		auto write_data = [&](const auto& _data) {
			_data.SaveData(os);
		};

		std::visit(write_data, data);
	}
	void Simulate() const {
		auto simulate = [](const auto& _data) {
			_data.Simulate();
		};

		std::visit(simulate, data);
	}
	bool AddDelay(float _delay) {
		auto add_delay = [&](auto& _data) {
			using type = std::decay_t<decltype(_data)>;
			if constexpr (std::is_same_v<type, DelayData>) {
				_data.AddDelay(static_cast<DWORD>(_delay));
				return true;
			}

			return false;
		};

		return std::visit(add_delay, data);
	}

	template<typename T, typename IfSame, typename IfNotSame>
	void ConditionalCall(IfSame&& _ifsame, IfNotSame&& _ifnotsame) {
		auto call = [&](auto& _data) {
			using type = std::decay_t<decltype(_data)>;
			if constexpr (std::is_same_v<type, T>)
				_ifsame(_data);
			else
				_ifnotsame();
		};

		std::visit(call, data);
	}
private:
	InputData data;
};