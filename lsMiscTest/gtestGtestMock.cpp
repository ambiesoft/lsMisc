#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MyMock {
public:
    MOCK_METHOD(int, myMethod, (int));
};

TEST(MyMockTest, TestMyMockMethod) {
    MyMock mock;

    // myMethodが1を引数に呼ばれることを期待
    EXPECT_CALL(mock, myMethod(1)).WillOnce(testing::Return(42));

    // myMethodを呼ぶ
    int result = mock.myMethod(1);

    // 戻り値が42であることを確認
    EXPECT_EQ(result, 42);
}

int DoSomethingWithFunction(std::function<int(int)> func, int value) {
    return func(value) * 2;
}

int Return42() {
    return 42;
}
class MyMockTest2 : public testing::Test {
protected:
    MyMockTest2() {}
    ~MyMockTest2() override {}

    void SetUp() override {}
    void TearDown() override {}
};
// テスト対象の関数をモックするためのクラスを作成
class MockFunction2 {
public:
    MOCK_METHOD(int, Call, (int), (const));
};

TEST_F(MyMockTest2, TestDoSomethingWithFunction) {
    MockFunction2 mock;

    // 期待する呼び出しを設定
    //((mock).gmock_Call(42))(
    //    ::testing::internal::GetWithoutMatchers(),
    //    nullptr).
    //    InternalExpectedAt(
    //        "C:\\local\\msys2\\home\\gGt55ktgftr\\gitdev\\lsMisc\\lsMiscTest\\gtestGtestMock.cpp",
    //        44,
    //        "mock",
    //        "Call(42)").
    //    WillOnce(testing::Return(100));
    EXPECT_CALL(mock, Call(Return42())).WillOnce(testing::Return(100));

    // テスト対象の関数を呼び出す
    int result = DoSomethingWithFunction(
        [&mock](int value) {
            return mock.Call(value); 
        }, 
        42);

    // 上の意味
    // １，mockがCall(42)で呼ばれるはずで、その時は100を返す
    // ２，DoSomethingWithFunctionが42で呼ばれて、ラムダを呼ぶ
    // ３, ラムダはmock.Callであり、100で呼ばれる
    // ４，DoSomethingWithFunctionは100*2を返す
    
    // 結果の検証
    // switch (0) case 0: default: if (const ::testing::AssertionResult gtest_ar = (::testing::internal::EqHelper::Compare("result", "200", result, 200))); else ::testing::internal::AssertHelper(::testing::TestPartResult::kNonFatalFailure, "C:\\local\\msys2\\home\\gGt55ktgftr\\gitdev\\lsMisc\\lsMiscTest\\gtestGtestMock.cpp", 50, gtest_ar.failure_message()) = ::testing::Message();
    EXPECT_EQ(result, 200);
}
