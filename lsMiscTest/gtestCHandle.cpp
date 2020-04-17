#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>

#include "gtest/gtest.h"

#include "../CHandle.h"


using namespace std;
using namespace Ambiesoft;

TEST(CHandle, Basic)
{
	{
		Ambiesoft::CHandle h = CreateMutex(NULL, TRUE, NULL);
	}

	{
		CHModule h = LoadLibrary(L"kernel32.dll");
	}

	{
		CGetProcAddress<BOOL (WINAPI *)(LPSECURITY_ATTRIBUTES,BOOL,LPCSTR)> proc(L"kernel32.dll", "CreateMutexA");
		if (proc)
			proc.GetProc()(NULL, TRUE, "aaa");
	}
}
