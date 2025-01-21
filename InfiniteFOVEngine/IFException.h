#pragma once

#include "IFEWin.h"
#include <exception>
#include "tstring.h"

class __declspec(dllexport) IFException : public std::exception
{
public:
	IFException(int line, const TCHAR* file) noexcept;
	const char* what() const noexcept override;
	virtual const WCHAR* wwhat() const noexcept;
	virtual const TCHAR* GetType() const noexcept;
	int GetLine() const noexcept;
	const tstring& GetFile() const noexcept;
	tstring GetOriginString() const noexcept;
private:
	int line;
	tstring file;
protected:
	mutable tstring whatBuffer;
};

#ifdef UNICODE
#define twhat wwhat
#else
#define twhat what
#endif