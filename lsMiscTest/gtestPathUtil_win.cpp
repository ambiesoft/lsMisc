//#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>

#include <cassert>
#include <string.h>
#include <string>

#include "gtest/gtest.h"

#include "../stdosd/PathUtil.h"

#include "debug.h"

using namespace Ambiesoft::stdosd;

TEST(PathUtil, IsRoot)
{
    EXPECT_TRUE(IsPathRoot(L"C:"));
    EXPECT_TRUE(IsPathRoot(L"C:\\"));
    EXPECT_TRUE(!IsPathRoot(L"C:\\a"));
    EXPECT_TRUE(IsPathRoot(L"\\\\"));
    EXPECT_TRUE(IsPathRoot(L"\\\\thexp"));
    EXPECT_TRUE(!IsPathRoot(L"\\\\thexp\\a"));
    EXPECT_TRUE(!IsPathRoot(L"\\\\thexp\\a\\b"));
    EXPECT_TRUE(!IsPathRoot(L""));
    EXPECT_TRUE(!IsPathRoot(L".."));
    EXPECT_TRUE(IsPathRoot(L"../../../../../../../../../../../../../../../../../../../../../"));

    EXPECT_TRUE(!IsPathRoot(L"/afwefwea/fjweo/fwejo"));
}

TEST(PathUtil, IsChild)
{
	LPCTSTR p1, p2;
	std::wstring duppath;

	p1 = L"C:\\a\\b\\c";
	p2 = L"C:\\a\\b/d";
	EXPECT_TRUE(!IsPathChildIncluded(p1, p2));

	p1 = L"C:\\a\\b\\c/../..";
	p2 = L"C:\\a\\b/d";
	EXPECT_TRUE(IsPathChildIncluded(p1, p2, &duppath));
	EXPECT_TRUE(lstrcmpi(duppath.c_str(), L"c:\\a")==0);

	p1 = L"\\\\thexp\\share\\";
	p2 = L"\\\\THEXP\\share\\b";
	EXPECT_TRUE(IsPathChildIncluded(p1, p2, &duppath));
	EXPECT_TRUE(lstrcmpi(duppath.c_str(), L"\\\\thexp\\share") == 0);

	p1 = L"\\\\thexp\\share\\xxx";
	p2 = L"\\\\THEXP\\share\\b";
	EXPECT_TRUE(!IsPathChildIncluded(p1, p2, &duppath));

	p1 = L"\\\\thexp\\share\\aaa";
	p2 = L"\\\\THEXP\\share\\aaa\\vvvsdf\\fwefew";
	EXPECT_TRUE(IsPathChildIncluded(p1, p2, &duppath));
	EXPECT_TRUE(lstrcmpi(duppath.c_str(), L"\\\\thexp\\share\\aaa") == 0);


	p1 = L"c:\\a\\b\\x\\f";
	p2 = L"c:\\a\\b/y";
	EXPECT_TRUE(!IsPathChildIncluded(p1, p2, &duppath));

	p1 = L"c:\\t";
	p2 = L"c:\\";
	EXPECT_TRUE(IsPathChildIncluded(p1, p2, &duppath));

	p1 = L"/a/b/c/d";
	p2 = L"/a/b/c/d/e";
	EXPECT_TRUE(IsPathChildIncluded(p1, p2, &duppath));
}
