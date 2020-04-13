#include "stdafx.h"

#include <Shlwapi.h>

#include <string>

#include "../sqlite3.h"

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
static wstring GetTestIniFile()
{
	return stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()),
		L"lsMiscTest.sqlite");
}

static int sqlcallback(void* pString, int argc, char** argv, char** azColName)
{
	std::string* pResult = (std::string*)pString;
	for (int i = 0; i < argc; i++)
	{
		std::string strt;
		strt = argv[i] ? argv[i] : "NULL";
		(*pResult) += strt;
		if ((i + 1) < argc)
			(*pResult) += "\t";
	}
	(*pResult) += "\r\n";
	return 0;
}
static void CreateWCFindTestDB(bool bAddData = false)
{
	DeleteFile(GetTestIniFile().c_str());
	EXPECT_FALSE(PathFileExists(GetTestIniFile().c_str()));

	sqlite3* pDB = nullptr;
	EXPECT_EQ(SQLITE_OK, sqlite3_open16(GetTestIniFile().c_str(), &pDB));
	EXPECT_TRUE(!!pDB);

	string s;
	char* p = nullptr;
	string sql = "CREATE TABLE [Option] ( [c1] TEXT, [c2] TEXT,PRIMARY KEY([c1]))";
	EXPECT_EQ(SQLITE_OK, sqlite3_exec(pDB,
		sql.c_str(),
		sqlcallback,
		&s,
		&p));

	if (bAddData) {
		EXPECT_EQ(SQLITE_OK, sqlite3_exec(pDB,
			"INSERT INTO Option (c1,c2) VALUES('aaa', '111');",
			sqlcallback,
			&s,
			&p));
		EXPECT_EQ(SQLITE_OK, sqlite3_exec(pDB,
			"INSERT INTO Option (c1,c2) VALUES(\"bbb\", \"222\");",
			sqlcallback,
			&s,
			&p));
	}

	EXPECT_EQ(SQLITE_OK, sqlite3_close(pDB));
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

TEST(SqlPrivateProfile, WCFindVer1Empty)
{
	{
		CreateWCFindTestDB();

		wchar_t buff[256];
		sqlGetPrivateProfileString(
			L"Option",
			L"KEY",
			L"",
			buff,
			_countof(buff),
			GetTestIniFile().c_str());
		EXPECT_STREQ(L"", buff);
	}
	{
		CreateWCFindTestDB();

		wchar_t buff[256];
		EXPECT_TRUE(!!sqlWritePrivateProfileString(
			L"Option",
			L"KEY",
			L"abc",
			GetTestIniFile().c_str()));
		EXPECT_TRUE(!!sqlGetPrivateProfileString(
			L"Option",
			L"KEY",
			L"",
			buff,
			_countof(buff),
			GetTestIniFile().c_str()));
		EXPECT_STREQ(L"abc", buff);
	}
}
TEST(SqlPrivateProfile, WCFindVer1WithData)
{
	{
		CreateWCFindTestDB(true);

		wchar_t buff[256];
		sqlGetPrivateProfileString(
			L"Option",
			L"bbb",
			L"",
			buff,
			_countof(buff),
			GetTestIniFile().c_str());
		EXPECT_STREQ(L"222", buff);
	}
}
