#include "stdafx.h"

#include <Shlwapi.h>

#include "gtest/gtest.h"

#pragma comment(lib, "shlwapi.lib")

#include <cassert>
#include <string>
#include <ostream>
#include <fstream>

#include "../stdwin32/stdwin32.h"
using namespace Ambiesoft::stdwin32;

#include "../UTF16toUTF8.h"
#include "../stdosd/I18NBeta.h"
using namespace Ambiesoft;

#include "../stdosd/stdosd.h"
using namespace Ambiesoft::stdosd;

using namespace std;


static void prepareLang()
{
	static auto once = false;
	if(!once)
	{
		once = true;

		wstring langdir = stdCombinePath(stdGetParentDirectory(stdGetModuleFileName<wchar_t>()), L"lang");
		CreateDirectory(langdir.c_str(), NULL);

		wstring langfile = stdCombinePath(langdir, L"jpn.txt");
		//if (PathFileExists(langfile.c_str()))
		//	return;

		wstring data(L"\"AAA\"=\"あああ\"");
		unique_ptr<char> pUtf8(UTF16toUTF8Ex(data.c_str()));

		FILE* f = NULL;
		_wfopen_s(&f, langfile.c_str(), L"wb");
		assert(f);
		fwrite(pUtf8.get(), strlen((char*)pUtf8.get()), 1, f);
		fclose(f);
		
	}
}

TEST(I18N,i18nBasicTestBetaA)
{
	prepareLang();
	i18nInitLangmap(NULL, "JPN");

	// wchar_t aaa[] = { 0x4230, 0x4230, 0x4230, 0 };
	//char aaa[] = "あああ";
	//EXPECT_STREQ(I18N("AAA"), aaa);
}
TEST(I18N, i18nBasicTestBetaW)
{
	prepareLang();
	i18nInitLangmap(NULL, L"JPN");

	// wchar_t aaa[] = { 0x4230, 0x4230, 0x4230, 0 };
	wchar_t aaa[] = { 0x3042, 0x3042, 0x3042, 0 };
	EXPECT_STREQ(I18N(L"AAA"), aaa);
}