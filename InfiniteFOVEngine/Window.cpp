#include "pch.h"
#include "Window.h"
#include "resource.h"
#include <sstream>

Window::WinDef Window::WinDef::defClass;

Window::WinDef::WinDef() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wnd = { 0 };
	wnd.cbSize = sizeof(wnd);
	wnd.style = CS_OWNDC;
	wnd.lpfnWndProc = HandleMsg;
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hInstance = GetInstance();
	wnd.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	wnd.hCursor = nullptr;
	wnd.hbrBackground = nullptr;
	wnd.lpszMenuName = nullptr;
	wnd.lpszClassName = GetName();
	wnd.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	RegisterClassEx(&wnd);
}

Window::WinDef::~WinDef()
{
	UnregisterClass(wndClassName, GetInstance());
}

const TCHAR* Window::WinDef::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WinDef::GetInstance() noexcept
{
	return defClass.hInst;
}

Window::Window(int width, int height, const TCHAR* name, bool isPrimary)
	:
	width(width),
	height(height)
{
	_isPrimary = isPrimary;
	this->width = width;
	this->height = height;
	RECT rect;
	rect.left = 100;
	rect.right = width + rect.left;
	rect.top = 100;
	rect.bottom = height + rect.top;
	if (AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw IF_WND_LASTEXCEPT();
	};

	hwnd = CreateWindow(WinDef::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, WinDef::GetInstance(), nullptr);
	
	if (hwnd == nullptr)
	{
		throw IF_WND_LASTEXCEPT();
	}

	alive = true;

	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	
	ShowWindow(hwnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(hwnd);
}
Window::~Window()
{
	if(alive)
		DestroyWindow(hwnd);
}

void Window::SetTitle(const tstring& title)
{
	if (SetWindowText(hwnd, title.c_str()) == 0)
	{
		throw IF_WND_LASTEXCEPT();
	}
}

std::optional<int> Window::ProcessMessage()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

Graphics& Window::Gfx()
{
	if (!pGfx)
		throw IF_NOGFX_EXCEPT();
	return *pGfx;
}

bool Window::isPrimary() noexcept
{
	return _isPrimary;
}
bool Window::isAlive() noexcept
{
	return alive;
}
void Window::Kill()
{
	if (alive)
	{
		DestroyWindow(hwnd);
		alive = false;
	}
}
LRESULT CALLBACK Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	switch (msg)
	{
	case WM_CLOSE:
		if(window->isPrimary()) PostQuitMessage(0);
		else window->Kill();
		return 0;
	case WM_KILLFOCUS:
		window->kbd.ClearState();
		break;

	//Keyboard
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lparam & 0x40000000) || window->kbd.IsAutoRepeat())
		{
			window->kbd.OnKeyPress(static_cast<unsigned char>(wparam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		window->kbd.OnKeyRelease(static_cast<unsigned char>(wparam));
		break;
	case WM_CHAR:
		window->kbd.OnChar(static_cast<unsigned char>(wparam));
		break;

	//Mouse
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		if (pt.x >= 0 && pt.x < window->width && pt.y >= 0 && pt.y < window->height)
		{
			window->mouse.OnMouseMove(pt.x, pt.y);
			if (!window->mouse.InWindow())
			{
				SetCapture(window->hwnd);
				window->mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wparam & (MK_LBUTTON | MK_RBUTTON))
			{
				window->mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				window->mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		window->mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		window->mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		window->mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		window->mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		window->mouse.OnMiddlePressed(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		window->mouse.OnMiddleReleased(pt.x, pt.y);
		break;
	}
	case WM_XBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		if (GET_XBUTTON_WPARAM(wparam) == XBUTTON1)
		{
			window->mouse.OnX1Pressed(pt.x, pt.y);
		}
		else if(GET_XBUTTON_WPARAM(wparam) == XBUTTON2)
		{
			window->mouse.OnX2Pressed(pt.x, pt.y);
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		if (GET_XBUTTON_WPARAM(wparam) == XBUTTON1)
		{
			window->mouse.OnX1Released(pt.x, pt.y);
		}
		else if (GET_XBUTTON_WPARAM(wparam) == XBUTTON2)
		{
			window->mouse.OnX2Released(pt.x, pt.y);
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wparam);
		window->mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

Window::HrException::HrException(int line, const TCHAR* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
#ifndef UNICODE

	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
#else
	return "";
#endif
}

const WCHAR* Window::HrException::wwhat() const noexcept
{
#ifdef UNICODE
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
#else
	return L"";
#endif
}

const TCHAR* Window::HrException::GetType() const noexcept
{
	return TEXT("IFE Window Exception");
}

tstring Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	TCHAR* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0)
	{
		return TEXT("Unidentified Error Code");
	}
	tstring errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
	return hr;
}

tstring Window::HrException::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}

const TCHAR* Window::NoGfxException::GetType() const noexcept
{
	return TEXT("IFE No Grapohics Exception");
}