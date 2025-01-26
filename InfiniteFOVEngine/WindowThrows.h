#pragma once

#define IF_WND_EXCEPT(hr) Window::HrException(__LINE__, TEXT(__FILE__), hr)
#define IF_WND_LASTEXCEPT() Window::HrException(__LINE__, TEXT(__FILE__), GetLastError())
#define IF_NOGFX_EXCEPT() Window::NoGfxException(__LINE__, TEXT(__FILE__))