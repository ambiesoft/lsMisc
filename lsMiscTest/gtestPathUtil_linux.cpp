//#include "stdafx.h"

#include <cassert>
#include <string.h>
#include <string>

#include "gtest/gtest.h"

#include "../stdosd/PathUtil.h"

// #include "debug.h"

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace std;

TEST(PathUtil, Canonicalize)
{
    EXPECT_TRUE(stdPathCanonicalize(u"/")==u"/");
    EXPECT_TRUE(stdPathCanonicalize(u"/a")==u"/a");
    EXPECT_TRUE(stdPathCanonicalize(u"/a/b")==u"/a/b");
    EXPECT_TRUE(stdPathCanonicalize(u"/a/b/c")==u"/a/b/c");
    EXPECT_TRUE(stdPathCanonicalize(u"/a/")==u"/a/");
    EXPECT_TRUE(stdPathCanonicalize(u"/a/b/")==u"/a/b/");
    EXPECT_TRUE(stdPathCanonicalize(u"/a/b/c/")==u"/a/b/c/");
}
TEST(PathUtil, IsRoot)
{
    EXPECT_TRUE(!IsPathRoot(u"C:"));
    EXPECT_TRUE(!IsPathRoot(u"C:\\"));
    EXPECT_TRUE(!IsPathRoot(u"C:\\a"));
    EXPECT_TRUE(!IsPathRoot(u"\\\\"));
    EXPECT_TRUE(!IsPathRoot(u"\\\\thexp"));
    EXPECT_TRUE(!IsPathRoot(u"\\\\thexp\\a"));
    EXPECT_TRUE(!IsPathRoot(u"\\\\thexp\\a\\b"));
    EXPECT_TRUE(!IsPathRoot(u""));
    EXPECT_TRUE(!IsPathRoot(u".."));
    EXPECT_TRUE(IsPathRoot(u"../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../../"));

    EXPECT_TRUE(!IsPathRoot(u"/afwefwea/fjweo/fwejo"));

    EXPECT_TRUE(!IsPathRoot(u"//"));


    EXPECT_TRUE(IsPathRoot(u"/"));

}

TEST(PathUtil, IsChild)
{
    u16string p1, p2;
    u16string duppath;

    p1 = u"/a/b/c";
    p2 = u"/a/b/d";
    EXPECT_TRUE(!IsPathChildIncluded(p1.c_str(), p2.c_str()));

    p1 = u"/a/b/c/../..";
    p2 = u"/a/b/d";
    EXPECT_TRUE(IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));
    EXPECT_TRUE(duppath == u16string(u"/a"));

    p1 = u"\\\\thexp\\share\\";
    p2 = u"\\\\thexp\\share\\b";
    EXPECT_TRUE(!IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));


    p1 = u"/a/b/x/f";
    p2 = u"/a/b/y";
    EXPECT_TRUE(!IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));

    p1 = u"/t";
    p2 = u"/";
    EXPECT_TRUE(!IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));

    p1 = u"/";
    p2 = u"/t";
    EXPECT_TRUE(IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));

    p1 = u"/a/b/c/d";
    p2 = u"/a/b/c/d/e";
    EXPECT_TRUE(IsPathChildIncluded(p1.c_str(), p2.c_str(), &duppath));
}
