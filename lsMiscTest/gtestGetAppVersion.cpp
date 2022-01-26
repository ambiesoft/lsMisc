
#include <string>
#include <Shlwapi.h>
#include "gtest/gtest.h"

#include "../RunCommandGetResult.h"
#include "../GetAppVersion.h"
#include "../stdosd/stdosd.h"

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace std;

TEST(GetAppVersionTest, Gtest)
{
	wstring printverexe = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()),
		L"printversion.exe");
	EXPECT_TRUE(!!PathFileExists(printverexe.c_str()));
	

	{
		string ver = GetAppVersion(printverexe.c_str(), L"--httpd", 3);
		EXPECT_EQ(ver, "2.4.23");
	}

	{
		string ver = GetAppVersion(printverexe.c_str(), L"--mysqld", 3);
		EXPECT_EQ(ver, "5.7.26");
	}
}

