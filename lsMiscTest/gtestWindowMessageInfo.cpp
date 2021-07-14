#include "stdafx.h"

#include "../WindowMessageInfo.h"

using namespace Ambiesoft;

TEST(WindowMessageInfoTest, WindowMessageInfoTest)
{
	EXPECT_EQ(120, WindowMessageInfo<WM_MOUSEWHEEL>::delta(MAKELONG(0,120), 0));
}