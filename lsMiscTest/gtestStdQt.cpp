
#include <string>
#include <QString>

#include "gtest/gtest.h"

#include "../stdQt/stdQt.h"
#include "../stdosd/stdosd.h"

using namespace AmbiesoftQt;
using namespace std;
using namespace Ambiesoft::stdosd;

#define EXPECT_QSTREQ(a,b) EXPECT_STREQ(QString(a).toStdWString().c_str(), QString(b).toStdWString().c_str())

TEST(StdQt, NormalizeDir)
{
#ifdef _WIN32
    EXPECT_QSTREQ( normalizeDir("C:\\T"), "C:/T/");
    EXPECT_QSTREQ( normalizeDir("C:\\T\\"), "C:/T/");
    EXPECT_QSTREQ( normalizeDir("\\\\aaa\\bbb"), "//aaa/bbb/");
    EXPECT_QSTREQ( normalizeDir("\\\\aaa\\bbb\\"), "//aaa/bbb/");
    EXPECT_QSTREQ( normalizeDir("\\\\aaa\\bbb/"), "//aaa/bbb/");
#endif
    EXPECT_QSTREQ( normalizeDir("/aaa/bbb"), "/aaa/bbb/");
}

TEST(StdQt, isRootDriveExists)
{
#ifdef _WIN32
    EXPECT_FALSE(isRootDriveExists(""));
    EXPECT_TRUE(isRootDriveExists("aaa"));
    EXPECT_FALSE(isRootDriveExists("x:\\"));
    EXPECT_FALSE(isRootDriveExists("\\\\nnnnn\\aaa\\"));
    EXPECT_TRUE(isRootDriveExists(QString::fromStdWString( stdGetModuleFileName() )));
    EXPECT_TRUE(isRootDriveExists("//Thexp/Share/Japan/aaa"));
#endif
}
