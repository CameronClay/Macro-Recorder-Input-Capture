#pragma once
#include <mutex>
#include <condition_variable>

class Event
{
public:
	Event(bool initialState = false);
	void Set(bool all = true);
	void Reset();
	void Wait();
	bool WaitFor(uint32_t timeMilli);
protected:
	std::mutex mutex;
	std::condition_variable cv;
	bool isSet;
};

class EventAutoReset : public Event
{
public:
	EventAutoReset(bool initialState = false);
	void Set(uint32_t waitCount = 1);
	void Reset();
	void Wait();
	bool WaitFor(uint32_t timeMilli);
private:
	uint32_t counter;
};

