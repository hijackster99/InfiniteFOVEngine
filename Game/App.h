#pragma once
#include "Window.h"
#include "IFTimer.h"

class App
{
public:
	App();
	int Go();
private:
	void DoFrame();
private:
	Window wnd;
	IFTimer timer;
};