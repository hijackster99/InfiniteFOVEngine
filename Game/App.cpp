#include "App.h"

App::App()
	:
	wnd(1080, 720, TEXT("Game Window"), true)
{}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessage())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	if (wnd.mouse.IsMiddlePressed())
	{
		wnd.SetTitle("Middle");
	}
	else if (wnd.mouse.IsX1Pressed())
	{
		wnd.SetTitle("X1");
	}
	else if (wnd.mouse.IsX2Pressed())
	{
		wnd.SetTitle("X2");
	}
}