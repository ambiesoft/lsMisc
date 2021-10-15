#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include "gtest/gtest.h"

#include "../stop_watch.h"

using namespace std;
//using namespace Ambiesoft;
//using namespace Ambiesoft::stdosd;

TEST(stop_watch, Basic)
{
	wstop_watch sw;
	sw.start();
	Sleep(500);
	wstring waited500 = sw.lookDiff();
	Sleep(300);
	wstring waited300 = sw.lookDiff();

	int w500 = _wtoi(waited500.c_str());
	EXPECT_TRUE(0 < w500);
	EXPECT_TRUE(w500 < 600);

	int w300 = _wtoi(waited300.c_str());
	EXPECT_TRUE(0 < w300);
	EXPECT_TRUE(w300 < 400);

	sw.stop();
}
