
#include <string>
#include <QString>

#include "gtest/gtest.h"

#include "../stdQt/stdQt.h"

using namespace AmbiesoftQt;
using namespace std;
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
