#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <iostream>

#include "gtest/gtest.h"

#include "../CenterWindow.h"
#include "../CreateSimpleWindow.h"

using namespace std;
using namespace Ambiesoft;

TEST(MoveWindowCommon, Basic)
{
	HWND h = CreateSimpleWindow();
	EXPECT_NE(h, (HWND)NULL);
	// invisible -> visible returns 0
	EXPECT_EQ(ShowWindow(h, SW_SHOW), 0);
	EXPECT_EQ(MoveWindow(h, 0, 0, 300, 300, TRUE), TRUE);

	EXPECT_EQ(MoveWindowCommon(h, MOVEWINDOW_TOPLEFT), true);
	Sleep(500);
	EXPECT_EQ(MoveWindowCommon(h, MOVEWINDOW_TOPRIGHT), true);
	Sleep(500);
	EXPECT_EQ(MoveWindowCommon(h, MOVEWINDOW_BOTTOMLEFT), true);
	Sleep(500);
	EXPECT_EQ(MoveWindowCommon(h, MOVEWINDOW_BOTTOMRIGHT), true);
	Sleep(500);

	EXPECT_TRUE(DestroyWindow(h));
}