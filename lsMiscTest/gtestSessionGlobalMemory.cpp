
#include <string>
#include <Shlwapi.h>
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
	EXPECT_STREQ("MyInt", sgInt.getName().c_str());

	CSessionGlobalMemory<int> sgInt2("MyInt");
	EXPECT_EQ(33333, sgInt2);
	sgInt2 = 555;
	EXPECT_EQ(555, sgInt);
	EXPECT_STREQ("MyInt", sgInt2.getName().c_str());

	CSessionGlobalMemory<int,wchar_t> sgInt2w(L"MyInt");
	EXPECT_EQ(555, sgInt2w);
	sgInt2w = 777;
	EXPECT_EQ(777, sgInt);
	EXPECT_EQ(777, sgInt2w);
	EXPECT_STREQ(L"MyInt", sgInt2w.getName().c_str());
}

TEST(SessionGlobalMemory, RemoteProcess)
{
	wstring setExe = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()),
		L"SetSessionGlobalMemory.exe");
	EXPECT_TRUE(!!PathFileExists(setExe.c_str()));
	LPCWSTR pEventName = L"MyLsMiscTestEvent";
	LPCSTR pSgName = "aaa";
	Ambiesoft::CKernelHandle event(CreateEvent(nullptr, TRUE, FALSE, pEventName));
	Ambiesoft::CKernelHandle process;
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

TEST(SessionGlobalMemory, DynamicBasic)
{
	BYTE c = 'd';
	const char* pName = "ddd";
	CDynamicSessionGlobalMemory ddd(pName,1);
	ddd.set(&c);
	EXPECT_STREQ(pName, ddd.getName().c_str());

	BYTE d;
	ddd.get(&d);
	EXPECT_EQ(c, d);
}

TEST(SessionGlobalMemory, DynamicBasic2)
{
	BYTE c = 'd';
	const char* pName = "ddd";
	CDynamicSessionGlobalMemory ddd(pName, 1);
	ddd.set(&c);
	EXPECT_STREQ(pName, ddd.getName().c_str());

	CDynamicSessionGlobalMemory ddd2(pName);
	EXPECT_EQ(ddd2.size(), 1);
	BYTE d;
	ddd2.get(&d);
	EXPECT_EQ(c, d);
}

TEST(SessionGlobalMemory, DynamicBig)
{
	BYTE byte[256];
	for (int i = 0; i < _countof(byte); ++i)
		byte[i] = i;

	const char* pName = "big";
	CDynamicSessionGlobalMemory ddd(pName, _countof(byte));
	ddd.set(byte);

	CDynamicSessionGlobalMemory ddd2(pName);
	BYTE byte2[256];
	ddd2.get(byte2);
	EXPECT_TRUE(memcmp(byte, byte2, _countof(byte2)) == 0);
	EXPECT_EQ(byte[100], byte2[100]);
}