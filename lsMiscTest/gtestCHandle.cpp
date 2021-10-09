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
		Ambiesoft::CHandle h1(CreateMutex(NULL, TRUE, NULL));
		Ambiesoft::CHandle h2(std::move(h1));
		EXPECT_FALSE(h1);
		EXPECT_TRUE(h2);
		h2 = std::move(h1);
		EXPECT_FALSE(h2);
	}
	{
		Ambiesoft::CHandle h1(CreateMutex(NULL, TRUE, NULL));
		HANDLE hh1 = h1;
		Ambiesoft::CHandle h2(CreateMutex(NULL, TRUE, NULL));
		Ambiesoft::CHandle h3;
		h2 = std::move(h1);
		EXPECT_FALSE(h1);
		h3 = std::move(h2);
		EXPECT_FALSE(h2);
		EXPECT_EQ(hh1, h3);
	}
	{
		Ambiesoft::CHandle h1(CreateMutex(NULL, TRUE, NULL));
		HANDLE hh1 = h1;
		Ambiesoft::CHandle h2(std::move(h1));
		EXPECT_EQ(WAIT_OBJECT_0, WaitForSingleObject(h2, INFINITE));
	}
	{
		Ambiesoft::CHandle h1(CreateMutex(NULL, TRUE, NULL));
		HANDLE hh1 = h1;
		Ambiesoft::CHandle h2;
		h2 = std::move(h1);
		EXPECT_EQ(WAIT_OBJECT_0, WaitForSingleObject(h2, INFINITE));
	}
	{
		Ambiesoft::CHandle h1(CreateMutex(NULL, TRUE, NULL));
		HANDLE hh1 = h1;
		Ambiesoft::CHandle h2;
		h2 = std::move(h1);
		h1 = std::move(h2);
		EXPECT_EQ(WAIT_OBJECT_0, WaitForSingleObject(h1, INFINITE));
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
