
#include <Shlwapi.h>

#include "gtest/gtest.h"

#include "../IsWindowsVersion.h"

using namespace Ambiesoft;

#include "../stdosd/stdosd.h"
using namespace Ambiesoft::stdosd;
using namespace std;


TEST(IsWindowVersion, Basic)
{
	if (IsWindows11OrAbove())
	{
		EXPECT_TRUE(IsWindows11OrAbove());
		EXPECT_TRUE(IsWindows10OrAbove());
		EXPECT_TRUE(IsWindows8OrAbove());
		EXPECT_TRUE(IsWindows7OrAbove());
		EXPECT_TRUE(IsWindowsVistaOrAbove());
		EXPECT_TRUE(IsWindowsXPOrAbove());
	}
	else if (IsWindows10OrAbove())
	{
		EXPECT_FALSE(IsWindows11OrAbove());
		EXPECT_TRUE(IsWindows10OrAbove());
		EXPECT_TRUE(IsWindows8OrAbove());
		EXPECT_TRUE(IsWindows7OrAbove());
		EXPECT_TRUE(IsWindowsVistaOrAbove());
		EXPECT_TRUE(IsWindowsXPOrAbove());
	}
	else if (IsWindows8OrAbove())
	{
		EXPECT_FALSE(IsWindows11OrAbove());
		EXPECT_FALSE(IsWindows10OrAbove());
		EXPECT_TRUE(IsWindows8OrAbove());
		EXPECT_TRUE(IsWindows7OrAbove());
		EXPECT_TRUE(IsWindowsVistaOrAbove());
		EXPECT_TRUE(IsWindowsXPOrAbove());
	}
	else if (IsWindows7OrAbove())
	{
		EXPECT_FALSE(IsWindows11OrAbove());
		EXPECT_FALSE(IsWindows10OrAbove());
		EXPECT_FALSE(IsWindows8OrAbove());
		EXPECT_TRUE(IsWindows7OrAbove());
		EXPECT_TRUE(IsWindowsVistaOrAbove());
		EXPECT_TRUE(IsWindowsXPOrAbove());
	}
	else if (IsWindowsVistaOrAbove())
	{
		EXPECT_FALSE(IsWindows11OrAbove());
		EXPECT_FALSE(IsWindows10OrAbove());
		EXPECT_FALSE(IsWindows8OrAbove());
		EXPECT_FALSE(IsWindows7OrAbove());
		EXPECT_TRUE(IsWindowsVistaOrAbove());
		EXPECT_TRUE(IsWindowsXPOrAbove());
	}
	else if (IsWindowsXPOrAbove())
	{
		EXPECT_FALSE(IsWindows11OrAbove());
		EXPECT_FALSE(IsWindows10OrAbove());
		EXPECT_FALSE(IsWindows8OrAbove());
		EXPECT_FALSE(IsWindows7OrAbove());
		EXPECT_FALSE(IsWindowsVistaOrAbove());
		EXPECT_TRUE(IsWindowsXPOrAbove());
	}
	else
	{
		EXPECT_TRUE(false);
	}
}