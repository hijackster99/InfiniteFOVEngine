#pragma once
#include "Window.h"
#include "IFTimer.h"
#include "Box.h"

class App
{
public:
	App();
	int Go();
	~App();
private:
	void DoFrame();
private:
	Window wnd;
	IFTimer timer;
	std::vector<std::unique_ptr<Box>> boxes;
};