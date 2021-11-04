// this should not be included
// because Qt project can not include MFC
// #include "stdafx.h"


#if defined(_WIN32)
#if !defined(NOMINMAX)
	#define NOMINMAX
#endif
#include <Windows.h>
#endif

#include <inttypes.h>
#include <limits>

#include "gtest/gtest.h"

#include "../stdosd/stdosd.h"
#include "../stdosd/PathString.h"

using namespace Ambiesoft::stdosd;
using namespace std;


TEST(stdosd, PathString)
{
	{
		string a, b;
		EXPECT_TRUE(a == b);
		EXPECT_EQ(a, b);
	}
	{
		PathString<char> path1;
		PathString<char> path2;
		// EXPECT_TRUE(path1 == path2);
	}
	{
		PathString<char> path("a"); 
	}
}