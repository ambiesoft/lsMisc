
#include <Shlwapi.h>

#include "gtest/gtest.h"

#include "../IsWindowsVersion.h"

using namespace Ambiesoft;

#include "../stdosd/stdosd.h"
using namespace Ambiesoft::stdosd;
using namespace std;


TEST(IsWindowVersion, Basic)
{
	EXPECT_TRUE(IsWindowsXPOrAbove() || IsWindowsXPOrBelow());
	EXPECT_TRUE(IsWindowsVistaOrAbove() || IsWindowsVistaOrBelow());
	EXPECT_TRUE(IsWindows7OrAbove() || IsWindows7OrBelow());
	EXPECT_TRUE(IsWindows8OrAbove() || IsWindows8OrBelow());
	EXPECT_TRUE(IsWindows10OrAbove() || IsWindows10OrBelow());

	int vercheckCount = 0;
	if (IsWindows10OrAbove() && IsWindows10OrBelow())
	{
		// This OS is Windows 10
		++vercheckCount;
		EXPECT_FALSE(IsWindows8OrBelow());
		EXPECT_FALSE(IsWindows7OrBelow());
		EXPECT_FALSE(IsWindowsVistaOrBelow());
		EXPECT_FALSE(IsWindowsXPOrBelow());
	}
	if (IsWindows8OrAbove() && IsWindows8OrBelow())
	{
		// This OS is Windows 8
		++vercheckCount;
		EXPECT_FALSE(IsWindows10OrAbove());
		EXPECT_TRUE(IsWindows10OrBelow());

		EXPECT_TRUE(IsWindows7OrAbove());
		EXPECT_FALSE(IsWindows7OrBelow());

		EXPECT_TRUE(IsWindowsVistaOrAbove());
		EXPECT_FALSE(IsWindowsVistaOrBelow());

		EXPECT_TRUE(IsWindowsXPOrAbove());
		EXPECT_FALSE(IsWindowsXPOrBelow());
	}
	if (IsWindows7OrAbove() && IsWindows7OrBelow())
	{
		// This OS is Windows 7
		++vercheckCount;
		EXPECT_FALSE(IsWindows10OrAbove());
		EXPECT_TRUE(IsWindows10OrBelow());

		EXPECT_FALSE(IsWindows8OrAbove());
		EXPECT_TRUE(IsWindows8OrBelow());

		EXPECT_TRUE(IsWindowsVistaOrAbove());
		EXPECT_FALSE(IsWindowsVistaOrBelow());

		EXPECT_TRUE(IsWindowsXPOrAbove());
		EXPECT_FALSE(IsWindowsXPOrBelow());
	}
	if (IsWindowsVistaOrAbove() && IsWindowsVistaOrBelow())
	{
		++vercheckCount;
	}
	if (IsWindowsXPOrAbove() && IsWindowsXPOrBelow())
	{
		++vercheckCount;
	}
	EXPECT_EQ(vercheckCount, 1);
}