#include <QtTest/QtTest>

#include "../stdQt.h"
#include "teststdqt.h"

QTEST_MAIN(TestStdQt)

using namespace AmbiesoftQt;

void TestStdQt::test_rstrip()
{
    QVERIFY(rstrip("aaa",'a') == "");
    QVERIFY(rstrip("aab",'a') == "aab");
    QVERIFY(rstrip("aab",'b') == "aa");
}
void TestStdQt::test_pathCombine()
{
    QVERIFY(pathCombine("a","b") == "a/b");
    QVERIFY(pathCombine("a","b","ccc") == "a/b/ccc");
}
void TestStdQt::test_normalizeDir()
{
    QVERIFY(normalizeDir("a/b")=="a/b/");
    QVERIFY(normalizeDir("a/b/")=="a/b/");
    QVERIFY(normalizeDir("a/b//")=="a/b/");
}
void TestStdQt::test_doublequoteIfNecessary()
{
    QVERIFY(doublequoteIfNecessary("")=="\"\"");
    QVERIFY(doublequoteIfNecessary("a")=="a");
    QVERIFY(doublequoteIfNecessary("aaa")=="aaa");
    QVERIFY(doublequoteIfNecessary("a a a")=="\"a a a\"");
}
void TestStdQt::test_undoublequoteIfNecessary()
{
    QVERIFY(undoublequoteIfNecessary("")=="");
    QVERIFY(undoublequoteIfNecessary("a")=="a");
    QVERIFY(undoublequoteIfNecessary("aaa")=="aaa");
    QVERIFY(undoublequoteIfNecessary("a a a")=="a a a");

    QVERIFY(undoublequoteIfNecessary("\"\"")=="");
    QVERIFY(undoublequoteIfNecessary("\"a\"")=="a");
    QVERIFY(undoublequoteIfNecessary("\"aaa\"")=="aaa");
    QVERIFY(undoublequoteIfNecessary("\"a a a\"")=="a a a");
}
