#include "stdafx.h"
#include <string>

#include "gtest/gtest.h"

#include "../stdwin32/stdwin32.h"
#include "../OpenCommon.h"

#include "../stdosd/stdosd.h"
using namespace std;
using namespace Ambiesoft::stdwin32;
using namespace Ambiesoft::stdosd;
using namespace Ambiesoft;

TEST(OpenCommon, ThisFolder)
{
	wstring thisexe = stdGetModuleFileName<wchar_t>();
	EXPECT_EQ(OpenFolder(NULL, thisexe.c_str()), TRUE);
}