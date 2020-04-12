#include "stdafx.h"

#include <Shlwapi.h>

#include <string>

#include "gtest/gtest.h"


#include "../sqliteserialize.h"
using namespace Ambiesoft;

#include "../stdosd/stdosd.h"
using namespace Ambiesoft::stdosd;
using namespace std;

static wstring GetIniFile()
{
	return stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()),
		stdGetFileNameWitoutExtension(stdGetModuleFileName<wchar_t>()) + L".sqlite");
}

static void testString(LPCWSTR pString)
{
	wstring iniFile = GetIniFile();

	LPCWSTR pApp = L"MyApp";
	LPCWSTR pKey = L"MyKey";

	EXPECT_TRUE(!!sqlWritePrivateProfileString(pApp, pKey, pString, iniFile.c_str()));

	wchar_t buff[256];
	EXPECT_TRUE(!!sqlGetPrivateProfileString(pApp, pKey, L"", buff, _countof(buff), iniFile.c_str()));

	EXPECT_STREQ(pString, buff);
}
TEST(SqlPrivateProfile, NullString)
{
	wstring iniFile = GetIniFile();

	LPCWSTR pApp = L"MyApp";
	LPCWSTR pKey = L"MyKey";

	EXPECT_TRUE(!!sqlWritePrivateProfileString(pApp, pKey, nullptr, iniFile.c_str()));

	wchar_t buff[256];
	EXPECT_TRUE(!sqlGetPrivateProfileString(pApp, pKey, L"AAA", buff, _countof(buff), iniFile.c_str()));

	EXPECT_STREQ(L"AAA", buff);
}
TEST(SqlPrivateProfile, BasicString)
{
	LPCWSTR ppStrings[] = {
		L"",
		L"MyString",
		L"My               String",
		L"=",
		L"\t",
		L"\n"
	};
	for (auto&& pString : ppStrings)
	{
		testString(pString);
	}
}

static void testInt(const int idata)
{
	wstring iniFile = GetIniFile();

	LPCWSTR pApp = L"MyApp";
	LPCWSTR pKey = L"MyKey";
	EXPECT_TRUE(!!sqlWritePrivateProfileInt(pApp, pKey, idata, iniFile.c_str()));

	int outint = sqlGetPrivateProfileInt(pApp, pKey, 98745, iniFile.c_str());

	EXPECT_EQ(idata, outint);
}
TEST(SqlPrivateProfile, BasicInt)
{
	int idatas[] = {
		-1,-2,0,1000,-123555,
	};
	for (int i : idatas)
		testInt(i);
}

void testArray(vector<wstring> ss)
{
	LPCWSTR pApp = L"MyApp";
	LPCWSTR pKey = L"MyKey";
	
	EXPECT_TRUE(!!sqlWritePrivateProfileStringArray(pApp, pKey, ss, GetIniFile().c_str()));
	
	vector<wstring> oo;
	EXPECT_TRUE(!!sqlGetPrivateProfileStringArray(pApp, pKey, oo, GetIniFile().c_str()));
	EXPECT_EQ(ss, oo);
}

TEST(SqlPrivateProfile, BasicStringArray)
{
	testArray({});
	testArray({ L"" });
	testArray({ L"",L"" });
	testArray({ L"aaa",L"bbb" });
	testArray({ L"aaa",L"bbb",L"cc c  ccc",L"deeee",L"fafeojfewoj",L"",L"vvv" });
}

TEST(SqlPrivateProfile, SameKey)
{
	testArray({ L"aaa",L"bbb" });
	testString(L"nnnnnnnnnnnnnnn nnnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
	testArray({ L"aaa",L"vvvv",L"jfffffwejfowejfowjfj" });
}
