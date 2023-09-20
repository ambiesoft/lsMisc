
#include <tchar.h>
#include <iostream>
#include <regex>

#include "gtest/gtest.h"

#include "../IsDuplicateInstance.h"
#include "../OpenCommon.h"
#include "../CHandle.h"
#include "../stdosd/stdosd.h"

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

TEST(IsDuplicateInstance, Basic)
{
	LPCWSTR pMutexName = L"lsmisctest_IsDuplicateInstance";

	{
		CKernelHandle hGetMutexProcess;
		EXPECT_TRUE(OpenCommon(nullptr,
			stdCombinePath(stdGetParentDirectory(stdGetModuleFileName()), L"GetMutex.exe").c_str(),
			nullptr,
			nullptr,
			&hGetMutexProcess));
		Sleep(1000);
		EXPECT_TRUE(IsDuplicateInstance(pMutexName));
		WaitForSingleObject(hGetMutexProcess, INFINITE);
		DWORD dwExitCode = -1;
		EXPECT_TRUE(GetExitCodeProcess(hGetMutexProcess, &dwExitCode));
		EXPECT_EQ(dwExitCode, 0);
	}
	EXPECT_FALSE(IsDuplicateInstance(pMutexName));
	EXPECT_FALSE(IsDuplicateInstance(pMutexName));

	{
		CKernelHandle hGetMutexProcess;
		EXPECT_TRUE(OpenCommon(nullptr,
			stdCombinePath(stdGetParentDirectory(stdGetModuleFileName()), L"GetMutex.exe").c_str(),
			nullptr,
			nullptr,
			&hGetMutexProcess));
		Sleep(1000);
		EXPECT_FALSE(IsDuplicateInstance(pMutexName));
		WaitForSingleObject(hGetMutexProcess, INFINITE);
		DWORD dwExitCode = -1;
		EXPECT_TRUE(GetExitCodeProcess(hGetMutexProcess, &dwExitCode));
		EXPECT_EQ(dwExitCode, 1);
	}

}
