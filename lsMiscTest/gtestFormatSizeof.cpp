
#include "stdafx.h"
#include <string>
#include <limits>
#include <gtest/gtest.h>
#include "../FormatSizeof.h"

#ifndef NOMINMAX
#error NOMINMAX must be defined.
#endif

using namespace std;

TEST(FormatSizeof, Basic)
{
	EXPECT_STREQ(FormatSizeof(10LL).c_str(), L"10");
	EXPECT_STREQ(FormatSizeof(1233LL).c_str(), L"1.2K");
	EXPECT_STREQ(FormatSizeof(334643131LL).c_str(), L"319.1M");
	EXPECT_STREQ(FormatSizeof(13778971131LL).c_str(), L"12.8G");
	EXPECT_STREQ(FormatSizeof(79741314LL).c_str(), L"76.0M");
	EXPECT_STREQ(FormatSizeof(123456777666LL).c_str(), L"115.0G");
	EXPECT_STREQ(FormatSizeof(4676464874646464465LL).c_str(), L"4.1E");

	// TODO(this looks not corrent)
	wstring t = FormatSizeof(std::numeric_limits<double>::max());
}