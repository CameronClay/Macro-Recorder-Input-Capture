#pragma once
#include <fstream>
#include <Windows.h>
#include <variant>

struct DelayData
{
	static constexpr int uuid = 0;
	DelayData(DWORD delayMilli = 0);
	DelayData(const DelayData&) = default;

	void AddDelay(DWORD delay);
	DWORD delayMilli;
};

struct MouseClickData
{
	static constexpr int uuid = 1;
	MouseClickData(bool down, bool left, bool right, bool middle);
	MouseClickData() = default;
	MouseClickData(const MouseClickData&) = default;

	bool down, left, right, middle;
};

struct MouseXClickData
{
	static constexpr int uuid = 2;
	MouseXClickData(bool down, bool x1, bool x2);
	MouseXClickData() = default;
	MouseXClickData(const MouseXClickData&) = default;

	bool down, x1, x2;
};

struct MouseMoveData
{
	static constexpr int uuid = 3;
	MouseMoveData(int x, int y, bool absolute);
	MouseMoveData() = default;
	MouseMoveData(const MouseMoveData&) = default;

	int x, y;
	bool absolute;
};

struct MouseScrollData
{
	static constexpr int uuid = 4;
	MouseScrollData(int nClicks);
	MouseScrollData() = default;
	MouseScrollData(const MouseScrollData&) = default;

	int nClicks;
};

struct KbdData
{
	static constexpr int uuid = 5;
	KbdData(WORD key, bool down, bool sc, bool E0);
	KbdData() = default;
	KbdData(const KbdData&) = default;

	WORD key;
	bool down, sc, E0;
};


using InputData = std::variant<DelayData, MouseClickData, MouseXClickData, MouseMoveData, MouseScrollData, KbdData>;

struct ReadVisitor
{
	std::ifstream& is;

	ReadVisitor(std::ifstream&);

	void operator()(DelayData const&);
	void operator()(MouseClickData const&);
	void operator()(MouseXClickData const&);
	void operator()(MouseMoveData const&);
	void operator()(MouseScrollData const&);
	void operator()(KbdData const&);
};

struct SaveVisitor
{
	std::ostream& os;

	SaveVisitor(std::ostream&);

	void operator()(DelayData const&);
	void operator()(MouseClickData const&);
	void operator()(MouseXClickData const&);
	void operator()(MouseMoveData const&);
	void operator()(MouseScrollData const&);
	void operator()(KbdData const&);
};

struct SimulateVisitor
{
	SimulateVisitor() = default;

	void operator()(DelayData const&);
	void operator()(MouseClickData const&);
	void operator()(MouseXClickData const&);
	void operator()(MouseMoveData const&);
	void operator()(MouseScrollData const&);
	void operator()(KbdData const&);
};