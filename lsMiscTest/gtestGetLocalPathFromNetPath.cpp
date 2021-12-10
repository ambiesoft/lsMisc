#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

#include <cassert>
#include <string.h>
#include <string>

#include "gtest/gtest.h"

#include "../GetLocalPathFromNetPath.h"

using namespace Ambiesoft;
using namespace std;

TEST(GetLocalPathFromNetPath,Basic)
{
	wstring ret;
	GetLocalPathFromNetPath(L"G:\\stanford-NPL\\stanford-parser-full-2018-10-17\\data\\chinese-onesent-unseg-gb18030.txt");
	GetLocalPathFromNetPath(L"C:\\local\\msys2\\home\\hkIurwvKj\\gitdev\\lsMisc\\lsMiscTest\\gtestStdosd_literal.cpp");
	GetLocalPathFromNetPath(L"P:\\MineWaZ\\FrontPage - PukiWiki.url");
	GetLocalPathFromNetPath(L"G:\\boost_1_71_0.7z");
}