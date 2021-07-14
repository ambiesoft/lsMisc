#include <QtTest/QtTest>

#include "testqstring.h"

//QTEST_MAIN(TestQString)
//#include "testqstring.moc"

void TestQString::toUpper_data()
{
    QTest::addColumn<QString>("string");
    QTest::addColumn<QString>("result");

    QTest::newRow("all lower") << "hello" << "HELLO";
    QTest::newRow("mixed")     << "Hello" << "HELLO";
    QTest::newRow("all upper") << "HELLO" << "HELLO";
}

void TestQString::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");
    QCOMPARE(str.toUpper(), QString("HELLO"));
    QCOMPARE(QString("hello").toUpper(), QString("HELLO"));
    QCOMPARE(QString("Hello").toUpper(), QString("HELLO"));
    QCOMPARE(QString("HellO").toUpper(), QString("HELLO"));
    QCOMPARE(QString("HELLO").toUpper(), QString("HELLO"));
}
