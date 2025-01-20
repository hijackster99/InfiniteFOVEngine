#pragma once
#include <chrono>

class __declspec(dllexport) IFTimer
{
public:
	IFTimer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};