#include "stdafx.h"

#include <Shlwapi.h>

#include <string>

#include "gtest/gtest.h"

#include "../sqlite3.h"
#include "../sqliteserialize.h"
#include "../stdosd/stdosd.h"

using namespace Ambiesoft;
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
static void testString(LPCWSTR pApp, LPCWSTR pString)
{
	wstring iniFile = GetIniFile();

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
		testString(L"vvv", pString);
	}
}

static void testInt(LPCWSTR pApp, const int idata)
{
	wstring iniFile = GetIniFile();

	LPCWSTR pKey = L"MyKey";
	EXPECT_TRUE(!!sqlWritePrivateProfileInt(pApp, pKey, idata, iniFile.c_str()));

	int outint = sqlGetPrivateProfileInt(pApp, pKey, 98745, iniFile.c_str());

	EXPECT_EQ(idata, outint);
}
TEST(SqlPrivateProfile, BasicInt)
{
	std::map<std::wstring, int> idatas = {
		{L"Rwei",-1},
		{L"Jack",-2},
		{L"Jfwef",0},
		{L"wefwek",1000},
		{L"verk",-34542},
		{L"3434k",-2},
	};
	for (auto&& a : idatas)
		testInt(a.first.c_str(), a.second);
}

void testArray(LPCWSTR pApp, vector<wstring> ss)
{
	LPCWSTR pKey = L"MyKey";
	
	EXPECT_TRUE(!!sqlWritePrivateProfileStringArray(pApp, pKey, ss, GetIniFile().c_str()));
	
	vector<wstring> oo;
	EXPECT_TRUE(!!sqlGetPrivateProfileStringArray(pApp, pKey, oo, GetIniFile().c_str()));
	EXPECT_EQ(ss, oo);
}

TEST(SqlPrivateProfile, BasicStringArray)
{
	map<wstring, vector<wstring>> datas = {
		{
			L"zzzz",
			{},
		},
		{
			L"fwefwef",
			{ L"" },
		},
		{
			L"vadrewrw",
			{ L"",L"" },
		},
		{
			L"vvadf",
			{ L"aaa",L"bbb" },
		},
		{
			L"zzzwefeez",
			{ L"aaa",L"bbb",L"cc c  ccc",L"deeee",L"fafeojfewoj",L"",L"vvv" },
		},
	};

	for (auto&& a : datas)
		testArray(a.first.c_str(), a.second);
}

TEST(SqlPrivateProfile, SameKey)
{
	testArray(L"samekey", { L"aaa",L"bbb" });
	testString(L"samekey", L"nnnnnnnnnnnnnnn nnnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
	testArray(L"samekey", { L"aaa",L"vvvv",L"jfffffwejfowejfowjfj" });
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


TEST(SqlPrivateProfile, LongString)
{
	//CreateWCFindTestDB(true);


	//char buff[3];
	//stdGetRandomString(buff, 3);
	wchar_t buffw[1024];
	stdGetRandomString(buffw, _countof(buffw));
	
	EXPECT_TRUE(!!sqlWritePrivateProfileString(L"Option", L"random", buffw,
		GetTestIniFile().c_str()));
	wchar_t buffw2[1024];
	EXPECT_TRUE(!!sqlGetPrivateProfileString(L"Option", L"random", L"", 
		buffw2, _countof(buffw2),
		GetTestIniFile().c_str()));
	EXPECT_EQ(wcslen(buffw), 1023);
	EXPECT_STREQ(buffw, buffw2);
}


TEST(SqlPrivateProfile, LongStringArray)
{
	vector<wstring> vIn;
	vIn.reserve(10000);
	for (int i = 0; i < 10000; ++i)
	{
		vIn.push_back(to_wstring(i));
	}
	EXPECT_TRUE(!!sqlWritePrivateProfileStringArray(
		L"Option",
		L"longstringarray",
		vIn,
		GetTestIniFile().c_str()));
	vector<wstring> vOut;
	EXPECT_TRUE(!!sqlGetPrivateProfileStringArray(
		L"Option",
		L"longstringarray",
		vOut,
		GetTestIniFile().c_str()));
	EXPECT_EQ(vIn, vOut);
}
TEST(SqlPrivateProfile, StrangeString)
{
	static const wchar_t p[] = { 1,2,3,0 };
	EXPECT_TRUE(!!sqlWritePrivateProfileString(L"Option", L"random", p,
		GetTestIniFile().c_str()));
	wchar_t buffw2[1024];
	EXPECT_TRUE(!!sqlGetPrivateProfileString(L"Option", L"random", L"",
		buffw2, _countof(buffw2),
		GetTestIniFile().c_str()));
	EXPECT_STREQ(buffw2, p);

}