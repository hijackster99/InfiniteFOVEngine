#pragma once
#include "IFEWin.h"
#include <string>
#include <sstream>

#ifdef UNICODE
typedef std::wstring tstring;
typedef std::wostringstream tostringstream;
#else
typedef std::string tstring;
typedef std::ostringstream tostringstream;
#endif
