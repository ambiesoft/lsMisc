
#include <tchar.h>
#include <iostream>
#include <regex>

#include "gtest/gtest.h"

#include "../GetVersionStringFromResource.h"

#include "../stdosd/stdosd.h"
using namespace std;
using namespace Ambiesoft::stdosd;
using namespace Ambiesoft;

TEST(GetVersionString, Explorer)
{
	wstring explorer = stdExpandEnvironmentStrings(L"%SystemRoot%\\explorer.exe");
	wstring ver = GetVersionStringFromResource(explorer.c_str());

	EXPECT_TRUE(std::regex_match(ver, std::wregex(L"\\d+\\.\\d+\\.\\d+\\.\\d+")));
}

TEST(GetVersionString, ThisExe)
{
	wstring exe = stdGetModuleFileName<wchar_t>();
	wstring ver = GetVersionStringFromResource(exe.c_str());

	EXPECT_TRUE(std::regex_match(ver, std::wregex(L"\\d+\\.\\d+\\.\\d+\\.\\d+")));
}

TEST(GetVersionString, NonExistentFile)
{
	wstring exe = L"T:\\aaewjojweejwowejewjojweoe\\weejowwejoejwowejoejw3237293277\\3232070327.exe";
	EXPECT_EQ(GetFileAttributes(exe.c_str()), 0xFFFFFFFF);

	wstring ver = GetVersionStringFromResource(exe.c_str());
	EXPECT_STREQ(ver.c_str(), L"");
}