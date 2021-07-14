#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

#include <cassert>
#include <string.h>
#include <string>

#include "gtest/gtest.h"

#include "../RevealFolder.h"

#include "debug.h"

using namespace Ambiesoft;

TEST(RevealFolder,Basic)
{
	LPCWSTR p = L"P:\\";
	EXPECT_TRUE(RevealFolder(p));
	EXPECT_TRUE(PathIsDirectory(p));

	p = L"P:\\AAAA";
	EXPECT_TRUE(RevealFolder(p));
	EXPECT_TRUE(PathIsDirectory(p));

	EXPECT_FALSE(RevealFolder(L"P:\\AAAfwefewfewwefewweA"));
}