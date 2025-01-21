#include "pch.h"
#include "IFException.h"
#include <sstream>

IFException::IFException(int line, const TCHAR* file) noexcept
	:
	line(line),
	file(file)
{}

const char* IFException::what() const noexcept
{
#ifndef UNICODE

	ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();

	return whatBuffer.c_str();
#else
	return "";
#endif
}

const WCHAR* IFException::wwhat() const noexcept
{
#ifdef UNICODE

	std::wostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();

	return whatBuffer.c_str();
#else
	return L"";
#endif
}

const TCHAR* IFException::GetType() const noexcept
{
	return TEXT("Engine Exception");
}

int IFException::GetLine() const noexcept
{
	return line;
}

const tstring& IFException::GetFile() const noexcept
{
	return file;
}

tstring IFException::GetOriginString() const noexcept
{
	tostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}