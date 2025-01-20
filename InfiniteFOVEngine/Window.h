#pragma once
#include "IFEWin.h"
#include "IFException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <optional>

class __declspec(dllexport) Window
{
public:
	class __declspec(dllexport) Exception : public IFException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	class WinDef
	{
	public:
		static const TCHAR* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WinDef() noexcept;
		~WinDef();
		WinDef(const WinDef&) = delete;
		WinDef& operator=(const WinDef&) = delete;
		static constexpr const TCHAR* wndClassName = TEXT("IF Engine Window");
		static WinDef defClass;
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const TCHAR* name, bool isPrimary = false);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessage();
	bool isPrimary() noexcept;
	bool isAlive() noexcept;
	void Kill();
private:
	static LRESULT CALLBACK HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	Keyboard kbd;
	Mouse mouse;
private:
	int width;
	int height;
	HWND hwnd;
	bool _isPrimary;

	bool alive;
};

#define IFWNDEXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define IFWNDLASTEXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())