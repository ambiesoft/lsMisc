
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

#include <cassert>
#include <string.h>
#include <string>

#include "gtest/gtest.h"

#include "../CheckMessageBox.h"

using namespace Ambiesoft;
using namespace std;

TEST(CheckMessageBox,Basic)
{
	int ret;
	bool bApplyAll;
	ret = CheckMessageBox(nullptr,
		L"Text", L"Caption", MB_OK, IDOK, &bApplyAll);
	EXPECT_EQ(ret, IDOK);

	ret = CheckMessageBox(nullptr,
		L"Text", L"Caption", MB_YESNO|MB_ICONQUESTION, IDYES, &bApplyAll);
	EXPECT_EQ(ret, IDYES);
}