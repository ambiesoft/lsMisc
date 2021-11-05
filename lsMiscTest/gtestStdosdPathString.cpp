// this should not be included
// because Qt project can not include MFC
// #include "stdafx.h"


#if defined(_WIN32)
#if !defined(NOMINMAX)
	#define NOMINMAX
#endif
#include <Windows.h>
#endif

#include <inttypes.h>
#include <limits>

#include "gtest/gtest.h"

#include "../stdosd/stdosd.h"
#include "../stdosd/PathString.h"

using namespace Ambiesoft::stdosd;
using namespace std;


TEST(stdosd, PathString)
{
	{
		string a, b;
		EXPECT_TRUE(a == b);
		EXPECT_EQ(a, b);
	}
	{
		string s(1, 'a');
		EXPECT_STREQ(s.c_str(), "a");
	}
	{
		PathString<wchar_t> path1;
		PathString<wchar_t> path2;
		EXPECT_TRUE(path1 == path2);
		EXPECT_EQ(path1, path2);
	}
	{
		PathString<char> path1("a");
		PathString<char> path2("a");
		EXPECT_EQ(path1, path2);
	}
	{
		PathString<char> path1("a");
		PathString<char> path2("b");
		EXPECT_NE(path1, path2);
	}
	{
		PathString<char> path1("/a");
		PathString<char> path2("/a/");
		EXPECT_EQ(path1, path2);
	}
	{
		PathString<char> path1("C:\\a");
		PathString<char> path2("C:\\a\\");
		EXPECT_EQ(path1, path2);
	}
	{
		PathString<char> path1("C:/a");
		PathString<char> path2("C:\\a\\");
		EXPECT_EQ(path1, path2);
	}
	{
		PathString<char> path1("C:/a/\\b");
		PathString<char> path2("C:\\a\\b\\");
		EXPECT_EQ(path1, path2);
	}
	{
		EXPECT_EQ(PathString<char>("a"), PathString<char>("a"));
	}
	{
		string a, b;
		EXPECT_EQ(PathString<char>(a), PathString<char>(b));
	}
	{
		string a = "a";
		string b = "b";
		EXPECT_NE(PathString<char>(a), PathString<char>(b));
	}
	{
		EXPECT_TRUE(stdIsSamePathString("a", "a"));
		EXPECT_FALSE(stdIsSamePathString("a", "b"));
	}
	{
		EXPECT_TRUE(stdIsSamePathString(L"a", L"a"));
		EXPECT_FALSE(stdIsSamePathString(L"a", L"b"));
	}
	{
		EXPECT_TRUE(stdIsSamePathString(L"C:/a/b/c", L"c:\\a\\b\\c\\"));
	}
	{
		PathString<char> path1("c:/a");
		EXPECT_TRUE(path1.cdUp());
		EXPECT_FALSE(path1.cdUp());
	}
	{
		PathString<char> path1("C:/a/\\b/c/d");
		EXPECT_TRUE(path1.cdUp());
		EXPECT_TRUE(path1.cdUp());
		PathString<char> path2("C:\\a\\b\\");
		EXPECT_EQ(path1, path2);
	}
}