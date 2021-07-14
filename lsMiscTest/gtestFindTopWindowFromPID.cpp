#include "stdafx.h"
#include <tchar.h>
#include <string>
#include <gtest/gtest.h>

#include "../../SessionGlobalMemory/SessionGlobalMemory.h"
#include "../OpenCommon.h"
#include "../stdosd/stdosd.h"
#include "../FindTopWindowFromPID.h"

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

TEST(FindTopWindowFromPID, Basic)
{
	LPCSTR pSession = "sgReturnHWD";
	LPCSTR pMutex = "lsMiscTestCloseReturnMainWndHandleMutex";

	CSessionGlobalMemory<HWND> sgReturnHWD(pSession);

	HANDLE hCloseMutex = CreateMutexA(NULL, TRUE, pMutex);
	EXPECT_NE(GetLastError(), ERROR_ALREADY_EXISTS);
	EXPECT_TRUE(hCloseMutex != NULL);

	string arg = stdFormat("-s %s -c %s",
		pSession,
		pMutex);
	//DWORD retCommand, dwLE;
	//string output, error;
	//bool bReleaseMutexCalled = false;
	
	string exe = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<char>()),
		"ReturnMainWndHandle.exe");
	EXPECT_TRUE(!!PathFileExistsA(exe.c_str()));
	HANDLE process = NULL;
	BOOL b=OpenCommonA(NULL,
		exe.c_str(),
		arg.c_str(),
		NULL,
		&process);
	//BOOL b = RunCommandGetResultCallBack(exe.c_str(),
	//	arg.c_str(),
	//	&retCommand,
	//	&dwLE,
	//	nullptr,
	//	[&](PROCESS_INFORMATION* ppi) {
	//		// After process created
	//		WaitForInputIdle(ppi->hProcess,INFINITE);
	//		hWndFound = FindTopWindowFromPID(ppi->dwProcessId);
	//		ReleaseMutex(hCloseMutex);
	//		bReleaseMutexCalled = true;
	//	},
	//	nullptr, nullptr,
	//	nullptr, nullptr);
	//EXPECT_EQ(dwLE, NO_ERROR);
	WaitForInputIdle(process, INFINITE);
	set<HWND> topHwnds = FindTopWindowFromPID(GetProcessId(process));
	ReleaseMutex(hCloseMutex);
	CloseHandle(process);
	EXPECT_TRUE(!!b);

	HWND hSgSet;
	sgReturnHWD.get(hSgSet);
	// EXPECT_EQ(topHwnds.size(), 1);
	EXPECT_TRUE(topHwnds.find(hSgSet) != topHwnds.end());
}
