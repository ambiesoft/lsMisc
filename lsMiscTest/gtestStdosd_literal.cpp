// this should not be included
// because Qt project can not include MFC
// #include "stdafx.h"

#if defined(_WIN32)
#include <Windows.h>
#endif

#include "gtest/gtest.h"

#include "../stdosd/stdosd_literal.h"

using namespace Ambiesoft::stdosd;
using namespace std;


TEST(stdosd, LiteralTest)
{
	{
        EXPECT_TRUE(stdLiterals<char>::period == '.');
        EXPECT_TRUE(stdLiterals<char>::N5 == '5');
        EXPECT_TRUE(stdLiterals<char>::NDoubleQuote == '"');

		EXPECT_STREQ(stdLiterals<char>::nulString(), "");
		EXPECT_STREQ(stdLiterals<char>::CRLF(), "\r\n");
	}
	{
        EXPECT_TRUE(stdLiterals<wchar_t>::period == L'.');
        EXPECT_TRUE(stdLiterals<wchar_t>::N5 == L'5');
        EXPECT_TRUE(stdLiterals<wchar_t>::NDoubleQuote == L'"');

		EXPECT_STREQ(stdLiterals<wchar_t>::nulString(), L"");
		EXPECT_STREQ(stdLiterals<wchar_t>::CRLF(), L"\r\n");
	}
#if defined(CHAR16T_AVAILABLE)
	{
        EXPECT_TRUE(stdLiterals<char16_t>::period == u'.');
        EXPECT_TRUE(stdLiterals<char16_t>::N5 == u'5');
        EXPECT_TRUE(stdLiterals<char16_t>::NDoubleQuote == u'"');

        // GTest does not have STREQ for char_16
        EXPECT_EQ(stdLiterals<char16_t>::nulString(), std::u16string(u""));
        EXPECT_EQ(stdLiterals<char16_t>::CRLF(), std::u16string(u"\r\n"));
	}
#endif
}

