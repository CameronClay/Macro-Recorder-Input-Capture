#include "Timer.h"

Timer::Timer()
{
	__int64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	invFreqMilli = 1.0 / ((double)frequency / 1000.0);
	StartWatch();
}

void Timer::StopWatch()
{
	if (!watchStopped)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);
		watchStopped = true;
	}
}

void Timer::StartWatch()
{
	watchStopped = false;
	QueryPerformanceCounter((LARGE_INTEGER*)&startCount);
}

void Timer::Reset()
{
	watchStopped = true;
	currentCount = 0;
	startCount = 0;
}


double Timer::GetTimeMilli() const
{
	if (!watchStopped)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);
		return (double)(currentCount - startCount) * invFreqMilli;
	}
	else
	{
		return (double)(currentCount - startCount) * invFreqMilli;
	}
}

bool Timer::Stopped() const
{
	return watchStopped;
}