#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

#include <cassert>
#include <string.h>
#include <string>

#include "gtest/gtest.h"

#include "../GetUnreparsePath.h"

#include "debug.h"

using namespace Ambiesoft;
using namespace std;

TEST(GetUnreparsePath,Basic)
{
	wstring ret;
	ret = GetUnreparsePath(L"");
	ret = GetUnreparsePath(nullptr);
	ret = GetUnreparsePath(L"C:\\T\\down\\aaa.txt");
	ret = GetUnreparsePath(L"C:\\T\\down\\bbb.txt");
}