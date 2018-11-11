#pragma once
#include <windows.h>

class Timer
{
public:
	Timer();
	void StartWatch();
	void StopWatch();
	void Reset();

	double GetTimeMilli() const;
	bool Stopped() const;
private:
	double invFreqMilli;
	bool watchStopped;
	__int64 currentCount, startCount;
};