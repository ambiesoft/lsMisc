
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
}