#include "stdafx.h"
#include <string>

#include "gtest/gtest.h"


#include "../SessionGlobalMemory/SessionGlobalMemory.h"
#include "../stdosd/stdosd.h"
#include "../OpenCommon.h"
#include "../CHandle.h"
#include "../UTF16toUTF8.h"

using namespace std;
using namespace Ambiesoft::stdosd;
using namespace Ambiesoft;
TEST(SessionGlobalMemory, Basic)
{
	CSessionGlobalMemory<int> sgInt("MyInt");
	sgInt = 33333;
	EXPECT_EQ(33333, sgInt);

	CSessionGlobalMemory<int> sgInt2("MyInt");
	EXPECT_EQ(33333, sgInt2);
	sgInt2 = 555;
	EXPECT_EQ(555, sgInt);
}

TEST(SessionGlobalMemory, RemoteProcess)
{
	wstring setExe = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()),
		L"SetSessionGlobalMemory.exe");
	EXPECT_TRUE(!!PathFileExists(setExe.c_str()));
	LPCWSTR pEventName = L"MyLsMiscTestEvent";
	LPCSTR pSgName = "aaa";
	Ambiesoft::CHandle event(CreateEvent(nullptr, TRUE, FALSE, pEventName));
	Ambiesoft::CHandle process;
	CSessionGlobalMemory<int> sgaaaa(pSgName,333);
	EXPECT_TRUE(!!OpenCommon(
		nullptr,
		setExe.c_str(),
		stdFormat(L"-name %s -value 333 -event %s",
			toStdWstringFromACP(pSgName).c_str(),
			pEventName).c_str(),
		nullptr,
		&process));
	WaitForSingleObject(event, INFINITE);
	CSessionGlobalMemory<int> sg(pSgName);
	EXPECT_EQ(333, sg);

	TerminateProcess(process, 1);
}