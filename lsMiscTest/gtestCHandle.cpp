
#include <windows.h>
#include <tchar.h>
#include <iostream>

#include "gtest/gtest.h"

#include "../CHandle.h"
#include "../CreateSimpleWindow.h"
#include "../stdosd/stdosd.h"

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

TEST(CHandle, Basic)
{
	{
		Ambiesoft::CKernelHandle h(CreateMutex(NULL, TRUE, NULL));
		EXPECT_TRUE(h);
		EXPECT_TRUE(h != NULL);
		EXPECT_TRUE(h != nullptr);
		EXPECT_FALSE(!h);
		EXPECT_FALSE(h == NULL);
		EXPECT_FALSE(h == nullptr);
	}
	{
		Ambiesoft::CKernelHandle h1(CreateMutex(NULL, TRUE, NULL));
		Ambiesoft::CKernelHandle h2(std::move(h1));
		EXPECT_FALSE(h1);
		EXPECT_TRUE(h2);
		h2 = std::move(h1);
		EXPECT_FALSE(h2);
	}
	{
		Ambiesoft::CKernelHandle h1(CreateMutex(NULL, TRUE, NULL));
		HANDLE hh1 = h1;
		Ambiesoft::CKernelHandle h2(CreateMutex(NULL, TRUE, NULL));
		Ambiesoft::CKernelHandle h3;
		h2 = std::move(h1);
		EXPECT_FALSE(h1);
		h3 = std::move(h2);
		EXPECT_FALSE(h2);
		EXPECT_EQ(hh1, h3);
	}
	{
		Ambiesoft::CKernelHandle h1(CreateMutex(NULL, TRUE, NULL));
		HANDLE hh1 = h1;
		h1 = std::move(h1);
		EXPECT_EQ(hh1, h1);
		EXPECT_EQ(WAIT_OBJECT_0, WaitForSingleObject(h1, INFINITE));
	}
	{
		Ambiesoft::CKernelHandle h1(CreateMutex(NULL, TRUE, NULL));
		HANDLE hh1 = h1;
		Ambiesoft::CKernelHandle h2;
		h2 = std::move(h1);
		EXPECT_EQ(WAIT_OBJECT_0, WaitForSingleObject(h2, INFINITE));
	}
	{
		Ambiesoft::CKernelHandle h1(CreateMutex(NULL, TRUE, NULL));
		HANDLE hh1 = h1;
		Ambiesoft::CKernelHandle h2;
		h2 = std::move(h1);
		h1 = std::move(h2);
		EXPECT_EQ(WAIT_OBJECT_0, WaitForSingleObject(h1, INFINITE));
	}

	{
		Ambiesoft::CFileHandle file;
		EXPECT_FALSE(file);
	}
	{
		Ambiesoft::CFileHandle file(CreateFile(stdGetModuleFileName().c_str(),
			GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL));
		EXPECT_TRUE(file);
	}
	{
		Ambiesoft::CFileHandle file(CreateFile(stdGetModuleFileName().c_str(),
			GENERIC_READ, FILE_SHARE_READ, NULL,
			CREATE_NEW, 0, NULL));
		EXPECT_FALSE(file);
	}
	{
		CFileHandle file1(CreateFile(stdGetModuleFileName().c_str(),
			GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL));
		CFileHandle file2(std::move(file1));
		EXPECT_FALSE(file1);
		EXPECT_TRUE(file2);
	}

	{
		{
			CKernelHandle h1(CreateMutex(NULL, TRUE, NULL));
			HANDLE hh1 = h1;
			EXPECT_TRUE(h1 == h1);
			EXPECT_TRUE(h1 == hh1);
			EXPECT_TRUE(hh1 == h1);
		}
		{
			CFileHandle f1(CreateFile(stdGetModuleFileName().c_str(),
				GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, 0, NULL));
			HANDLE ff1 = f1;
			EXPECT_TRUE(f1 == f1);
			EXPECT_TRUE(f1 == ff1);
			EXPECT_TRUE(ff1 == f1);
		}
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
		CHMenu menu(CreatePopupMenu());
	}
	{
		// CHIcon icon(CreateIcon(NULL, 16, 16, 1, 1, NULL, NULL));
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
		constexpr size_t count = 3;
		vector<CKernelHandle> threads;
		threads.reserve(count);

		for (int i = 0; i < count; ++i)
		{
			unsigned dwThreadId = 0;
			CKernelHandle t(CreateEvent(NULL,TRUE,FALSE,NULL));

			threads.emplace_back(move(t));
		}
	}
}
