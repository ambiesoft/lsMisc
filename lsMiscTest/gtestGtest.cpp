
#include <gtest/gtest.h>

using namespace std;

void MyExit(bool death,int exitCode)
{
	if (death)
	{
		cerr << "this is my death test" << endl;
		exit(exitCode);
	}
	cerr << "I'm Alive!!" << endl;
}
bool MyExited(int exitCode, int exitedCode)
{
	return exitCode == exitedCode;
}

// A Test starting with 'DISABLED_' will be disabled
TEST(DISABLED_GTestTest, DeathTest)
{
	int exitCode = 0;
	EXPECT_EXIT(MyExit(true, 1), ::testing::ExitedWithCode(1), "this is my death test");
	EXPECT_EXIT(MyExit(true, 54321), std::bind(MyExited, 54321, std::placeholders::_1), "this is my death test");
	EXPECT_DEATH(MyExit(true, 1), "this is my death test");

    // failed test
	EXPECT_EXIT(MyExit(false, 1), ::testing::ExitedWithCode(1), "this is my death test");
	EXPECT_EXIT(MyExit(true, 12345), std::bind(MyExited, 54321, std::placeholders::_1), "this is my death test");
	EXPECT_DEATH(MyExit(true, 1), "this is NOT my death test");
}

/////////////////////////////////Fixture//////////////////////
// Fixture shares resources with many tests
// First Create a fixture class

// http://opencv.jp/googletestdocs/advancedguide.html
class FooTest : public ::testing::Test {
protected:
	// テストケース毎の set-up．
	// このテストケースの最初のテストよりも前に呼び出されます．
	// 必要ならば変更するしてください．
	static void SetUpTestCase() {
		shared_resource_ = new int;
		*shared_resource_ = 9999;
	}

	// テストケース毎の tear-down．
	// このテストケースの最後のテストの後で呼び出されます．
	// 必要ならば変更するしてください．
	static void TearDownTestCase() {
		delete shared_resource_;
		shared_resource_ = NULL;
	}

	// テスト毎の set-up と tear-down も通常通り定義できます．
	virtual void SetUp() {  
		non_shared_resource_ = new int;
		*non_shared_resource_ = 100;
	}
	virtual void TearDown() {  
		delete non_shared_resource_;
		non_shared_resource_ = nullptr;
	}

	// 高コストのリソースが，全てのテストで共有されます．
	static int* shared_resource_;

	int* non_shared_resource_;
};
// static
int* FooTest::shared_resource_ = nullptr;

TEST_F(FooTest, Test1) {
	// ... ここで，shared_resource を参照できます ...
	EXPECT_EQ(*shared_resource_, 9999);
}
TEST_F(FooTest, Test2) {
	// ... ここで，shared_resource を参照できます ...
	EXPECT_EQ(*shared_resource_, 9999);
}


//// Parameterized Test ///
// 1, Bad way
bool IsEven(int i) {
	//if (i == 6)
	//	return false; // intentional error
	return i % 2 == 0;
}

TEST(BadEven, IsEvenTest) {
	EXPECT_TRUE(IsEven(0));
	EXPECT_FALSE(IsEven(1));
	EXPECT_TRUE(IsEven(2));
	EXPECT_FALSE(IsEven(3));
	EXPECT_TRUE(IsEven(4));
	EXPECT_FALSE(IsEven(5));
	EXPECT_TRUE(IsEven(6));
	EXPECT_FALSE(IsEven(7));
	EXPECT_TRUE(IsEven(8));
	EXPECT_FALSE(IsEven(9));
}

TEST(BadEven2, IsEvenTest) {
	for (int i = 0; i < 10; i += 2)
		EXPECT_TRUE(IsEven(i));

	for (int i = 1; i < 10; i += 2)
		EXPECT_FALSE(IsEven(i));
}

// 2, Good way
class GoodEvenTest : public ::testing::TestWithParam<int> {
	// ここは，通常のフィクスチャクラスのメンバと同じように書くことができます．
	// テストパラメータにアクセスするには，クラスから GetParam() を呼び出します．
	// TestWithParam<T>.
};

TEST_P(GoodEvenTest, DoesBlah)
{
	// テスト内部では，TestWithParam<T> クラスの GetParam() メソッド
	// を用いてテストパラメータにアクセスしてください：
	int v = GetParam();
	EXPECT_TRUE(IsEven(v));
}

INSTANTIATE_TEST_SUITE_P(EvenNumber,
	GoodEvenTest,
	::testing::Values(0, 2, 4, 6, 8));

class GoodOddTest : public ::testing::TestWithParam<int> {
	// ここは，通常のフィクスチャクラスのメンバと同じように書くことができます．
	// テストパラメータにアクセスするには，クラスから GetParam() を呼び出します．
	// TestWithParam<T>.
};

TEST_P(GoodOddTest, DoesBlah)
{
	// テスト内部では，TestWithParam<T> クラスの GetParam() メソッド
	// を用いてテストパラメータにアクセスしてください：
	int v = GetParam();
	EXPECT_FALSE(IsEven(v));
}

INSTANTIATE_TEST_SUITE_P(OddNumber,
	GoodOddTest,
	::testing::Values(1, 3, 5, 7, 9));

