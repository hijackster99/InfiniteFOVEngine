#include "pch.h"
#include "IFException.h"
#include <sstream>

IFException::IFException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* IFException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* IFException::GetType() const noexcept
{
	return "Engine Exception";
}

int IFException::GetLine() const noexcept
{
	return line;
}

const std::string& IFException::GetFile() const noexcept
{
	return file;
}

std::string IFException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}