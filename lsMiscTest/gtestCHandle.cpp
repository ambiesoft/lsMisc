#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>

#include "gtest/gtest.h"

#include "../CHandle.h"
#include "../CreateSimpleWindow.h"

using namespace std;
using namespace Ambiesoft;

TEST(CHandle, Basic)
{
	{
		Ambiesoft::CHandle h(CreateMutex(NULL, TRUE, NULL));
	}

	{
		CHModule h(LoadLibrary(L"kernel32.dll"));
	}

	{
		CGetProcAddress<BOOL (WINAPI *)(LPSECURITY_ATTRIBUTES,BOOL,LPCSTR)> proc(L"kernel32.dll", "CreateMutexA");
		if (proc)
			proc.GetProc()(NULL, TRUE, "aaa");
	}

	{
		CHWnd h(CreateSimpleWindow());
		EXPECT_TRUE(h);
		EXPECT_TRUE(IsWindow(h));
	}

	{
		CHWnd h;
		{
			EXPECT_FALSE(h);
			h = CreateSimpleWindow();
			EXPECT_TRUE(h);
			EXPECT_TRUE(IsWindow(h));
		}
		EXPECT_TRUE(h);
		DestroyWindow(h);
		EXPECT_FALSE(h);
	}

	{
		constexpr size_t count = 3;
		vector<CHandle> threads;
		threads.reserve(count);

		for (int i = 0; i < count; ++i)
		{
			unsigned dwThreadId = 0;
			CHandle t(CreateEvent(NULL,TRUE,FALSE,NULL));

			threads.emplace_back(move(t));
		}
	}
}
