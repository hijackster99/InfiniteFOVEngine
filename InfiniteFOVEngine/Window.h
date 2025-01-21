#pragma once
#include "IFEWin.h"
#include "IFException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>

class __declspec(dllexport) Window
{
public:
	class __declspec(dllexport) Exception : public IFException
	{
		using IFException::IFException;
	public:
		static tstring TranslateErrorCode(HRESULT hr) noexcept;
	};
	class __declspec(dllexport) HrException : public Exception
	{
	public:
		HrException(int line, const TCHAR* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const WCHAR* wwhat() const noexcept override;
		virtual const TCHAR* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		tstring GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const TCHAR* GetType() const noexcept override;
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
	void SetTitle(const tstring& title);
	static std::optional<int> ProcessMessage();
	Graphics& Gfx();
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
	std::unique_ptr<Graphics> pGfx;
	bool _isPrimary;

	bool alive;
};

#define IF_WND_EXCEPT(hr) Window::HrException(__LINE__, TEXT(__FILE__), hr)
#define IF_WND_LASTEXCEPT() Window::HrException(__LINE__, TEXT(__FILE__), GetLastError())
#define IF_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, TEXT(__FILE__))