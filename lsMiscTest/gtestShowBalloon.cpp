#include "stdafx.h"

#include "gtest/gtest.h"

#include "../showBalloon.h"

using namespace Ambiesoft;



TEST(ShowBalloon, Show)
{
	EXPECT_TRUE(showballoon(
				 NULL,
				 L"tttitle",
				 L"teeeeeeeext",
				 NULL,
				 3000,
				 GetTickCount(),
				 FALSE,
 				 1));
}