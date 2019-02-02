#if !defined(_MSWIN_TARGET_H_INCLUDED_)
#define _MSWIN_TARGET_H_INCLUDED_

#if !defined(_WIN32_WINNT)
#error "_WIN32_WINNT is not defined"
#endif  // !defined(_WIN32_WINNT)

#if !(_WIN32_WINNT >= 0x0601)
#error "_WIN32_WINNT is lower than 0x0601 (_WIN32_WINNT_WIN7)"
#endif  // !defined(_WIN32_WINNT)

#include <Windows.h>

#endif  // !defined(_MSWIN_TARGET_H_INCLUDED_)
