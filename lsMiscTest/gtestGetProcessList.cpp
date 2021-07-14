#include "stdafx.h"

#include "gtest/gtest.h"

#include "../stdwin32/stdwin32.h"
#include "../stdosd/stdosd.h"
#include "../GetProcessList.h"

using namespace Ambiesoft::stdosd;
using namespace std;
using namespace Ambiesoft::stdwin32;

TEST(GetProcessList, Basic)
{
	vector<PROCESSENTRY32> ret;
	EXPECT_TRUE(GetProcessList(ret));
	EXPECT_GT(ret.size(), 0UL);
	
	bool ok = false;
	DWORD thisid = GetCurrentProcessId();
	wstring thisexefilename = stdGetFileName(stdGetModuleFileName<wchar_t>());
	for (PROCESSENTRY32& entry : ret)
	{
		if (thisid == entry.th32ProcessID)
		{
			EXPECT_STREQ(entry.szExeFile, thisexefilename.c_str());
			ok = true;
		}
	}
	EXPECT_TRUE(ok);
}
