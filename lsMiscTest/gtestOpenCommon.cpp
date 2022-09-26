
#include <string>

#include "gtest/gtest.h"

#include "../OpenCommon.h"

#include "../stdosd/stdosd.h"
using namespace std;
using namespace Ambiesoft::stdosd;
using namespace Ambiesoft;

TEST(OpenCommon, ThisFolder)
{
	wstring thisexe = stdGetModuleFileName<wchar_t>();
	EXPECT_EQ(OpenFolder(NULL, thisexe.c_str()), TRUE);
}