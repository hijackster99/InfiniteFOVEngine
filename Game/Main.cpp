#include "App.h"

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		return App{}.Go();
	}
	catch (const IFException& e)
	{
		MessageBox(nullptr, e.twhat(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch ( ... )
	{
		MessageBox(nullptr, TEXT("No Details Available"), TEXT("Unknown Exception"), MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}