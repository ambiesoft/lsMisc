#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE


// https://qiita.com/hkuno/items/7b8daa37d9b68e390d7e
#if 1 // check WINVER,_WIN32_*.
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#pragma message("WINVER        : " STRING(WINVER))
#pragma message("_WIN32_WINNT  : " STRING(_WIN32_WINNT))
//  #pragma message("_WIN32_WINDOWS: " STRING(_WIN32_WINDOWS)) // Windows9x only
#pragma message("_WIN32_IE     : " STRING(_WIN32_IE))
#endif