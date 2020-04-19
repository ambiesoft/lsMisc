

//#if _MSC_VER <= 1800
//// Basically gtest leaks, so turn it off
//#ifndef NDEBUG
//// include only in DEBUG mode
//#include <vld.h>
//#endif
//#endif // _MSC_VER <= 1800



#include <afxwin.h>

// In lsMisc directory, run the following command
// git submodule init
// git submodule update
#include "gtest/gtest.h"

#ifdef _DEBUG
#include <crtdbg.h>
#define malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define calloc(s1,s2) _calloc_dbg(s1, s2, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define realloc(p,s) _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define free(p)  _free_dbg(p, _NORMAL_BLOCK)
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#ifndef NOMINMAX
#error NOMINMAX must be defined.
#endif

#include <Windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <Shlwapi.h>
#include <tchar.h>

#pragma warning(push)
#pragma warning(disable: 4091)
#include <ShlObj.h>
#pragma warning(pop)

#include <cctype>
#include <cstdarg>
#include <functional>
#include <locale>


#include <cassert>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>

#include <time.h>



#include "../PragmaShowVersions.h"
