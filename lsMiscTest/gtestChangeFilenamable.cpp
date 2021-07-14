#include "stdafx.h"
#include <tchar.h>
#include <string>
#include <gtest/gtest.h>

#include "../MakeFilenamable.h"

using namespace std;

TEST(MakeFilenamable, Basic)
{
	LPCTSTR p;
	wstring ret;

	p = NULL;
	ret = MakeFilenamable(p);
	EXPECT_STREQ(L"newfile", ret.c_str());


	p = _T("開発機構特別資料");
	ret = MakeFilenamable(p);
	EXPECT_STREQ(p, ret.c_str());

	p = _T("aaa\\bbb");
	ret = MakeFilenamable(p);
	EXPECT_STREQ(ret.c_str(), _T("aaa_bbb"));

	p = _T("aaa ");
	ret = MakeFilenamable(p);
	EXPECT_STREQ(ret.c_str(), _T("aaa"));

	p = _T("aaa .");
	ret = MakeFilenamable(p);
	EXPECT_STREQ(ret.c_str(), _T("aaa"));

	p = _T("CON");
	ret = MakeFilenamable(p);
	EXPECT_STREQ(ret.c_str(), _T("CON_"));

	p = _T("CON ");
	ret = MakeFilenamable(p);
	EXPECT_STREQ(ret.c_str(), _T("CON_"));

	p = _T("CON a");
	ret = MakeFilenamable(p);
	EXPECT_STREQ(ret.c_str(), _T("CON a"));

}