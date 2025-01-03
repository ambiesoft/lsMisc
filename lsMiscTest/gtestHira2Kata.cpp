﻿
#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>

#include "gtest/gtest.h"

#include "../LCMapCommon.h"

using namespace std;

TEST(Hira2Kata,Basic)
{
	// "あああアアア ｱｱｱ"
	// UTF16-LE
	wchar_t p[] = { 0x3042, 0x3042, 0x3042, 0x30A2, 0x30A2, 0x30A2, 0x0020, 0xFF71, 0xFF71, 0xFF71, 0x00 };
	tstring s = Kata2Hira(p);
	// "ああああああ ｱｱｱ"
	wchar_t ret[] = { 0x3042, 0x3042, 0x3042, 0x3042, 0x3042, 0x3042, 0x0020, 0xFF71, 0xFF71, 0xFF71, 0x00 };
	EXPECT_STREQ(s.c_str(), ret);

	s=Hira2Kata(p);
	// "アアアアアア ｱｱｱ"
	// UTF16-LE
	wchar_t ret2[] = { 0x30A2, 0x30A2, 0x30A2, 0x30A2, 0x30A2, 0x30A2, 0x0020, 0xFF71, 0xFF71, 0xFF71, 0x00 };
	EXPECT_STREQ(s.c_str(), ret2);

}