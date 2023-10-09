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


//////////////////////////////////////////////////////
// SaveArg<0>のテスト
//////////////////////////////////////////////////////
struct SomeData { int a; };
class ISomeClass
{
public:
    virtual ~ISomeClass() = default;
    virtual void foo(SomeData*) = 0;
};

void someFunction(ISomeClass& a)
{
    SomeData b{ 1 };
    a.foo(&b);
}

class SomeMock : public ISomeClass
{
public:
    MOCK_METHOD1(foo, void(SomeData*));
};

TEST(TestSomeFoo, shallPassOne)
{
    SomeData actualData{};
    SomeMock aMock;
    EXPECT_CALL(aMock, foo(::testing::_))
        .WillOnce(::testing::DoAll(::testing::SaveArgPointee<0>(&actualData)));
    someFunction(aMock);
    ASSERT_EQ(1, actualData.a);
}

/////////////////////////////////////////////
// https://stackoverflow.com/a/56547601
class API {
public:
    virtual bool Init(uint8_t* dataPointer) = 0;
    virtual bool Store() = 0;
    virtual bool Restore() = 0;
};

class MockAPI : public API {
public:
    MOCK_METHOD1(Init,
        bool(uint8_t* dataPointer));
    MOCK_METHOD0(Store,
        bool());
    MOCK_METHOD0(Restore,
        bool());
};

class ClassUnderTest {
public:
    explicit ClassUnderTest(std::shared_ptr<API> api) : api_(api) {
        dataPtr_ = new uint8_t;
        api_->Init(dataPtr_);
    }
    ~ClassUnderTest() {
        delete dataPtr_;
    }
    bool anotherMethod() {
        api_->Restore();
        return true;
    }
    uint8_t takeALookAtTheData() {
        return *dataPtr_;
    }
private:
    std::shared_ptr<API> api_;
    uint8_t* dataPtr_;
};

using testing::_;
using testing::Invoke;

TEST(xxx, yyy) {
    // NiceMock prevent warning
    auto mockApi = std::make_shared<::testing::NiceMock<MockAPI>>();
    uint8_t* dataPtr(nullptr);
    uint8_t testValue = 123;
    ON_CALL(*mockApi, Init(_)).WillByDefault(Invoke([&dataPtr](uint8_t* dataPointer) {
        dataPtr = dataPointer;
        return true;
        }));
    ON_CALL(*mockApi, Restore()).WillByDefault(Invoke([&dataPtr, testValue]() {
        *dataPtr = testValue;
        return true;
        }));
    ClassUnderTest sut(mockApi);
    ASSERT_NE(nullptr, dataPtr);
    sut.anotherMethod();
    ASSERT_EQ(testValue, *dataPtr);
    ASSERT_EQ(testValue, sut.takeALookAtTheData());
}