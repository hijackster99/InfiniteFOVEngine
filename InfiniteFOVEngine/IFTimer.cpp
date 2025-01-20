#include "pch.h"
#include "IFTimer.h"

IFTimer::IFTimer()
{
	last = std::chrono::steady_clock::now();
}

float IFTimer::Mark()
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frametime = last - old;
	return frametime.count();
}

float IFTimer::Peek() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}