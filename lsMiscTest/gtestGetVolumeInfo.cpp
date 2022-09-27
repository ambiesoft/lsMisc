
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

#include <cassert>
#include <string.h>
#include <string>

#include "gtest/gtest.h"

#include "../GetVolumeInfo.h"

using namespace Ambiesoft;
using namespace std;

TEST(GetVolumeInfo,Basic)
{
	vector<VolumeInfo> infos;
	EXPECT_EQ(0, GetVolumeInfo(&infos));
	EXPECT_NE(0, infos.size());
}