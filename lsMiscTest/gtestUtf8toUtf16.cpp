#include "StdAfx.h"
#include <memory>
#include "gtest/gtest.h"

#include "../UTF16toUTF8.h"

using namespace Ambiesoft;
using namespace std;

static void MyFree(void* p)
{
	free(p);
}
TEST(UTF8TOUTF16, convert8to16)
{
	{
		std::unique_ptr<char[]> p(new char[3]);
		std::unique_ptr<wchar_t> pW(UTF8toUTF16Ex("aaa"));
		EXPECT_TRUE(wcscmp(pW.get(), L"aaa") == 0);
	}
	{
		int outlen;
		std::unique_ptr<wchar_t> pW(UTF8toUTF16Ex("aaa", 3, &outlen));
		EXPECT_TRUE(wcscmp(pW.get(), L"aaa") == 0);
		EXPECT_EQ(outlen, 3);
	}
	{
		int outlen;
		std::unique_ptr<wchar_t> pW(UTF8toUTF16Ex("abcdef", 2, &outlen));
		EXPECT_TRUE(wcscmp(pW.get(), L"ab") == 0);
		EXPECT_EQ(outlen, 2);
	}

	{
		string s = "a";
		std::unique_ptr<wchar_t> pW(UTF8toUTF16Ex(s.c_str()));
		EXPECT_STREQ(pW.get(), L"a");
	}

	{
		int outlen;
		LPCWSTR p = MultiBytetoUTF16Ex(CP_ACP, "", 0, &outlen);
		EXPECT_STREQ(p, L"");
		EXPECT_EQ(outlen, 0);
		delete p;
	}

	{
		int outlen;
		LPCWSTR p = MultiBytetoUTF16Ex(CP_ACP, "aaa", 3, &outlen);
		EXPECT_STREQ(p, L"aaa");
		EXPECT_EQ(outlen, 3);
		delete p;
	}

	{
		string s = "aaa";
		EXPECT_EQ(toStdWstringFromUtf8(s), wstring(L"aaa"));
	}
}

TEST(UTF8TOUTF16, convert16to8)
{
	{
		std::unique_ptr<char> p(UTF16toUTF8Ex(L"aaa"));
		EXPECT_STREQ(p.get(), "aaa");
	}
	{
		int outlen;
		std::unique_ptr<char> p(UTF16toUTF8Ex(L"aaa", 3, &outlen));
		EXPECT_STREQ(p.get(), "aaa");
		EXPECT_EQ(outlen, 3);
	}

	{
		int outlen;
		std::unique_ptr<char> p(UTF16toMultiByteEx(CP_ACP, L"aaa", 3, &outlen));
		EXPECT_STREQ(p.get(), "aaa");
		EXPECT_EQ(outlen, 3);
	}
	{
		int outlen;
		std::unique_ptr<char> p(UTF16toMultiByteEx(CP_ACP, L"", 0, &outlen));
		EXPECT_STREQ(p.get(), "");
		EXPECT_EQ(outlen, 0);
	}
	{
		int outlen;
		std::unique_ptr<char> p(UTF16toMultiByteEx(CP_ACP, nullptr, 0, &outlen));
		EXPECT_STREQ(p.get(), nullptr);
		EXPECT_EQ(outlen, 0);
	}
	{
		wstring s = L"sss";
		EXPECT_EQ(toStdUtf8String(s), string("sss"));
	}
	{
		wchar_t c = L'c';
		EXPECT_EQ(toStdUtf8String(c), string("c"));
	}
}

TEST(UTF8TOUTF16, EUC)
{
	{
		unsigned char t[2] = { 0xb0, 0xb1 };
		string s = toStdUtf8String(51932, (char*)t, 2);
		EXPECT_EQ(2, s.size());
	}

}