#pragma once

#ifdef _WIN32
#ifdef _DEBUG

// Define AMBIESOFT_USE_DEBUGNEW to use debug new and malloc for
// checking memory leak.
// DebugNew.cpp must be also included
#ifdef AMBIESOFT_USE_DEBUGNEW

#ifndef DEBUG_NEW
#include <stdlib.h>
#include <Windows.h>
#include <crtdbg.h>

#define DEBUG_NEW_DEFINED
#define malloc(size) _malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define calloc(s1,s2) _calloc_dbg(s1, s2, _NORMAL_BLOCK, __FILE__, __LINE__ )
#define realloc(p,s) _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define free(p)  _free_dbg(p, _NORMAL_BLOCK)

#ifdef __cplusplus
#pragma warning(disable: 4229)
void* operator new(size_t nSize, int nType, LPCSTR lpszFileName, int nLine);
void* operator new[](size_t nSize, int nType, LPCSTR lpszFileName, int nLine);
void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
void* operator new[](size_t nSize, LPCSTR lpszFileName, int nLine);
void operator delete(void* pData, LPCSTR, int);
void operator delete[](void* pData, LPCSTR, int);
#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW
#define DEBUG_NEW_DEFINED
#endif

#endif  // DEBUG_NEW
#endif  // AMBIESOFT_USE_DEBUGNEW

#endif  // _DEBUG
#endif  // _WIN32
