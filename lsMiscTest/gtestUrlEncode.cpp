#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cassert>
// #include <stlsoft/smartptr/scoped_handle.hpp>

#include "gtest/gtest.h"

#include "../UrlEncode.h"
#include "../UTF16toUTF8.h"

using namespace std;
using namespace Ambiesoft;
TEST(UrlEncode, BasicWithChar)
{
	EXPECT_STREQ(UrlEncodeStd("aaa").c_str(), "aaa");
	EXPECT_STREQ(UrlEncodeStd("aaa a").c_str(), "aaa+a");

	EXPECT_STREQ(UrlEncodeStd("aaa", 1).c_str(), "a");
}
TEST(UrlEncode, BasicWithWchar)
{
	EXPECT_STREQ(wstring(UrlEncodeStd(L"aaa")).c_str(), L"aaa");
	EXPECT_STREQ(wstring(UrlEncodeStd(L"aaa a")).c_str(), L"aaa+a");

	EXPECT_STREQ(wstring(UrlEncodeStd(L"aaa a", 1)).c_str(), L"a");
}
TEST(UrlEncode, Complex)
{
	{
		LPCSTR p = "https://social.msdn.microsoft.com/Search/en-US?query=isalnum&beta=0&ac=8";
		unique_ptr<char> penc(UrlEncodeEx(p));
		unique_ptr<char> pdec(UrlDecodeEx(penc.get()));
		
		EXPECT_STREQ(p, pdec.get());
	}

	{
		LPCSTR p = "https://social.msdn.microsoft.com/Search/en-US?query=%e3%81%82%e3%81%b0%e3%81%b0%e3%81%b0%ef%bd%82%e3%81%98%e3%81%88%e3%81%88%ef%bd%97%e3%82%8c%ef%bd%97&beta=0&ac=8";
		unique_ptr<char> penc(UrlEncodeEx(p));
		unique_ptr<char> pdec(UrlDecodeEx(penc.get()));
		EXPECT_STREQ(p, pdec.get());

		LPCSTR cp = "https://social.msdn.microsoft.com/Search/en-US?query=%e3%81%82%e3%81%b0%e3%81%b0%e3%81%b0%ef%bd%82%e3%81%98%e3%81%88%e3%81%88%ef%bd%97%e3%82%8c%ef%bd%97&beta=0&ac=8";
		string c1 = UrlEncodeStd(cp);
		string c2 = UrlDecodeStd<string>(c1.c_str());
		EXPECT_EQ(cp, c2);

		LPCWSTR wp = L"https://social.msdn.microsoft.com/Search/en-US?query=%e3%81%82%e3%81%b0%e3%81%b0%e3%81%b0%ef%bd%82%e3%81%98%e3%81%88%e3%81%88%ef%bd%97%e3%82%8c%ef%bd%97&beta=0&ac=8";
		wstring w1 = UrlEncodeStd(wp);
		wstring w2 = UrlDecodeStd<wstring>(w1.c_str());
		EXPECT_EQ(wp, w2);
	}

	{
		// LPCWSTR wp = L"あいうえお";
		// "あいうえお"
		// UTF16-LE
		wchar_t wp[] = {0x3042,0x3044,0x3046,0x3048,0x304A,0x00};
		std::wstring wenc = UrlEncodeStd(wp);
		std::wstring wdec = UrlDecodeStd<wstring>(wenc.c_str());
		EXPECT_STREQ(wp, wdec.c_str());
	}

	{
		// "あいうえお"
		// UTF8
		unsigned char cp[] = { 0xE3,0x81,0x82,0xE3,0x81,0x84,0xE3,0x81,0x86,0xE3,0x81,0x88,0xE3,0x81,0x8A,0x0 };
		string c1 = UrlEncodeStd((const char*)cp);
		string c2 = UrlDecodeStd<string>(c1.c_str());
		EXPECT_STREQ((const char*)cp, c2.c_str());
	}
	{
		// shiftjs encoded "あいうえお"
		char cp[] = "%82%A0%82%A2%82%A4%82%A6%82%A8";
		string co = UrlDecodeStd<string>(cp);
		string c1 = UrlEncodeStd(co.c_str());
		EXPECT_EQ(cp, c1);
	}
}

TEST(UrlEncode, CrossType)
{
	{
		// LPCWSTR wp = L"あいうえお";
		// "あいうえお"
		// UTF16-LE
		wchar_t wp[] = { 0x3042,0x3044,0x3046,0x3048,0x304A,0x00 };
		wstring wenc = UrlEncodeStd(wp);
		string dec = UrlDecodeStd<string>(wenc.c_str());
		wstring wdec = toStdWstringFromUtf8(dec);
		EXPECT_STREQ(wp, wdec.c_str());
	}
}