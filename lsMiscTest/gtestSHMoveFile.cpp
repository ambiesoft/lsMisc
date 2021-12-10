#include "stdafx.h"
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "gtest/gtest.h"

#include "../stdwin32/stdwin32.h"
#include "../stdosd/stdosd.h"
#include "../SHMoveFile.h"
#include "../CreateCompleteDirectory.h"

#include "../DebugNew.h"

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace stdwin32;

#pragma comment(lib,"Rpcrt4.lib")

static wstring uuidgen()
{
	UUID uuid;
	UuidCreate(&uuid);
	RPC_WSTR p;
	UuidToStringW(&uuid,&p);
	wstring ret((wchar_t*)p);
	RpcStringFree(&p);
	return ret;
}

class FileOperationTest : public ::testing::Test
{
	wstring testRoot_;
protected:
	FileOperationTest() = default;
	virtual void SetUp()
	{
		testRoot_ = L".\\TESTDIR\\" + uuidgen();
		CreateCompleteDirectory(testRoot_.c_str(), NULL);
		ASSERT_TRUE(PathIsDirectory(testRoot_.c_str()));
	}
	virtual void TearDown()
	{
		RemoveDirectory(testRoot_.c_str());
		EXPECT_FALSE(PathIsDirectory(testRoot_.c_str()));
	}
	wstring testRoot() const {
		return testRoot_;
	}
};

void createSimpleFile(wstring file)
{
	wofstream myfile;
	myfile.open(file.c_str());
	ASSERT_TRUE(!!myfile);
	myfile << L"aaa";
	myfile.close();
}

TEST_F(FileOperationTest, MoveFileTestMoveSingle) 
{

	// Create file
	wstring testFile = stdCombinePath(testRoot(), uuidgen());
	createSimpleFile(testFile);

	// Move XXX -> New.txt
	{
		wstring p1 = testFile;
		wstring p2 = stdCombinePath(testRoot(), L"New.txt");

		EXPECT_TRUE(PathFileExists(p1.c_str()));
		EXPECT_FALSE(PathFileExists(p2.c_str()));

		EXPECT_EQ(SHMoveFileEx( p1.c_str(), p2.c_str()), 0);

		EXPECT_FALSE(PathFileExists(p1.c_str()));
		EXPECT_TRUE(PathFileExists(p2.c_str()));

		EXPECT_EQ(SHMoveFileEx( p2.c_str(), p1.c_str()), 0);

		EXPECT_TRUE(PathFileExists(p1.c_str()));
		EXPECT_FALSE(PathFileExists(p2.c_str()));

		DeleteFile(p1.c_str());
		EXPECT_FALSE(PathFileExists(p1.c_str()));
	}
}
TEST_F(FileOperationTest, MoveFileTestCopySingle)
{
	// Create file
	wstring testFile = stdCombinePath(testRoot(), uuidgen());
	createSimpleFile(testFile);

	// copy single
	{
		createSimpleFile(testFile);

		wstring p3 = stdCombinePath(testRoot(), L"New1.txt");
		wstring p4 = stdCombinePath(testRoot(), L"New2.txt");
		EXPECT_FALSE(PathFileExists(p3.c_str()));
		EXPECT_FALSE(PathFileExists(p4.c_str()));

		EXPECT_EQ(SHCopyFileEx( testFile.c_str(), p3.c_str(), FOF_NO_UI), 0);
		EXPECT_EQ(SHCopyFileEx( testFile.c_str(), p4.c_str(), FOF_NO_UI), 0);

		EXPECT_TRUE(PathFileExists(p3.c_str()));
		EXPECT_TRUE(PathFileExists(p4.c_str()));

		DeleteFile(p3.c_str());
		DeleteFile(p4.c_str());
		DeleteFile(testFile.c_str());
	}
}
TEST_F(FileOperationTest, MoveFileTestMoveMultiSame)
{
	{
		// Create file
		wstring testFile = stdCombinePath(testRoot(), uuidgen());
		createSimpleFile(testFile);

		// Create ./xxxxxx/xxxx/
		wstring newdir = stdCombinePath(testRoot(), uuidgen());
		ASSERT_FALSE(PathIsDirectory(newdir.c_str()));
		CreateDirectory(newdir.c_str(), NULL);
		ASSERT_TRUE(PathIsDirectory(newdir.c_str()));
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> newdirremover(newdir.c_str(),
			[](const wchar_t* p) {
			RemoveDirectory(p);
			EXPECT_FALSE(PathIsDirectory(p));
		});

		// multi file move
		// Move ./xxxxx/New1.txt and ./xxxx/New2.txt to ./xxxx/DIR/
	
		wstring p3 = stdCombinePath(testRoot(), L"New1.txt");
		wstring p4 = stdCombinePath(testRoot(), L"New2.txt");
		EXPECT_EQ(SHCopyFileEx( testFile.c_str(), p3.c_str(), FOF_NO_UI), 0);
		EXPECT_EQ(SHCopyFileEx( testFile.c_str(), p4.c_str(), FOF_NO_UI), 0);

		vector<wstring> ws;
		ws.push_back(p3);
		ws.push_back(p4);
		EXPECT_TRUE(PathFileExists(p3.c_str()));
		EXPECT_TRUE(PathFileExists(p4.c_str()));
		EXPECT_TRUE(SHMoveFileEx(ws, newdir.c_str(), FOF_NO_UI) == 0);
		EXPECT_FALSE(PathFileExists(p3.c_str()));
		EXPECT_FALSE(PathFileExists(p4.c_str()));
		wstring newLocation1 = stdCombinePath(newdir, stdGetFileName(p3));
		wstring newLocation2 = stdCombinePath(newdir, stdGetFileName(p4));
		EXPECT_TRUE(PathFileExists(newLocation1.c_str()));
		EXPECT_TRUE(PathFileExists(newLocation2.c_str()));

		DeleteFile(newLocation1.c_str());
		DeleteFile(newLocation2.c_str());

		DeleteFile(testFile.c_str());
	}
}

TEST_F(FileOperationTest, MoveFileTestMoveMultiDiff)
{
	{
		// Create file
		wstring testFile = stdCombinePath(testRoot(), uuidgen());
		createSimpleFile(testFile);

		// move 2 file from different dir to same dir

		wstring dir1 = testRoot();
		wstring dir2 = stdCombinePath(testRoot(), uuidgen());
		CreateDirectory(dir2.c_str(), NULL);
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> dir2remover(dir2.c_str(),
			[](const wchar_t* p) {
			RemoveDirectory(p);
			EXPECT_FALSE(PathIsDirectory(p));
		});
		wstring file1 = stdCombinePath(dir1, L"file1.txt");
		wstring file2 = stdCombinePath(dir2, L"file2.txt");

		createSimpleFile(file1);
		createSimpleFile(file2);

		vector<wstring> ws;
		ws.push_back(file1);
		ws.push_back(file2);

		wstring targetdir = stdCombinePath(testRoot(), uuidgen());
		CreateDirectory(targetdir.c_str(), NULL);
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> targetdirremover(targetdir.c_str(),
			[](const wchar_t* p) {
			RemoveDirectory(p);
			EXPECT_FALSE(PathIsDirectory(p));
		});

		EXPECT_TRUE(PathFileExists(file1.c_str()));
		EXPECT_TRUE(PathFileExists(file2.c_str()));
		EXPECT_TRUE(SHMoveFileEx( ws, targetdir.c_str(), FOF_NO_UI) == 0);
		EXPECT_FALSE(PathFileExists(file1.c_str()));
		EXPECT_FALSE(PathFileExists(file2.c_str()));

		wstring targetfile1 = stdCombinePath(targetdir, stdGetFileName(file1));
		wstring targetfile2 = stdCombinePath(targetdir, stdGetFileName(file2));
		EXPECT_TRUE(PathFileExists(targetfile1.c_str()));
		EXPECT_TRUE(PathFileExists(targetfile2.c_str()));

		DeleteFile(targetfile1.c_str());
		DeleteFile(targetfile2.c_str());

		DeleteFile(testFile.c_str());
	}
}

TEST_F(FileOperationTest, MoveFileTestCopyMultiSame)
{
	{
		const wchar_t* pF1 = L"New1.txt";
		const wchar_t* pF2 = L"New2.txt";

		// Create file
		wstring nf1 = stdCombinePath(testRoot(), pF1);
		wstring nf2 = stdCombinePath(testRoot(), pF2);
		createSimpleFile(nf1);
		createSimpleFile(nf2);

		unique_ptr<const wchar_t, void(*)(const wchar_t*)> nf1remover(nf1.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> nf2remover(nf2.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});

		wstring targetdir = stdCombinePath(testRoot(), L"DIR");
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> targetdirremover(targetdir.c_str(),
			[](const wchar_t* p) {
			RemoveDirectory(p);
			EXPECT_FALSE(PathIsDirectory(p));
		});
		vector<wstring> ws;
		ws.push_back(nf1);
		ws.push_back(nf2);
		EXPECT_TRUE(SHCopyFileEx( ws, targetdir.c_str(), FOF_NO_UI) == 0);
		wstring tf1 = stdCombinePath(targetdir, pF1);
		wstring tf2 = stdCombinePath(targetdir, pF2);
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> tf1remover(tf1.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> tf2remover(tf2.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});

		EXPECT_TRUE(PathFileExists(nf1.c_str()));
		EXPECT_TRUE(PathFileExists(nf2.c_str()));
		EXPECT_TRUE(PathFileExists(tf1.c_str()));
		EXPECT_TRUE(PathFileExists(tf1.c_str()));
	}
}

TEST_F(FileOperationTest, MoveFileTestMoveMultiToMulti)
{
	{
		const wchar_t* pF1 = L"New1.txt";
		const wchar_t* pF2 = L"New2.txt";

		// Create source file
		wstring nf1 = stdCombinePath(testRoot(), pF1);
		wstring nf2 = stdCombinePath(testRoot(), pF2);
		createSimpleFile(nf1);
		createSimpleFile(nf2);

		// create dest files
		const wchar_t* pF5 = L"New5.txt";
		const wchar_t* pF6 = L"New6.txt";


		wstring nf5 = stdCombinePath(testRoot(), pF5);
		wstring nf6 = stdCombinePath(testRoot(), pF6);

		unique_ptr<const wchar_t, void(*)(const wchar_t*)> nf5remover(nf5.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> nf6remover(nf6.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});


		vector<wstring> wsS;
		vector<wstring> wsD;
		wsS.push_back(nf1);
		wsS.push_back(nf2);
		wsD.push_back(nf5);
		wsD.push_back(nf6);

		EXPECT_TRUE(PathFileExists(nf1.c_str()));
		EXPECT_TRUE(PathFileExists(nf2.c_str()));
		EXPECT_TRUE(!PathFileExists(nf5.c_str()));
		EXPECT_TRUE(!PathFileExists(nf6.c_str()));
		EXPECT_TRUE(SHMoveFileEx( wsS, wsD, FOF_NO_UI) == 0);
		EXPECT_TRUE(!PathFileExists(nf1.c_str()));
		EXPECT_TRUE(!PathFileExists(nf2.c_str()));
		EXPECT_TRUE(PathFileExists(nf5.c_str()));
		EXPECT_TRUE(PathFileExists(nf6.c_str()));
	}
}

TEST_F(FileOperationTest, MoveFileTestCopyMultiToMulti)
{
	{
		const wchar_t* pF1 = L"New1.txt";
		const wchar_t* pF2 = L"New2.txt";

		// Create source file
		wstring nf1 = stdCombinePath(testRoot(), pF1);
		wstring nf2 = stdCombinePath(testRoot(), pF2);
		createSimpleFile(nf1);
		createSimpleFile(nf2);

		unique_ptr<const wchar_t, void(*)(const wchar_t*)> nf1remover(nf1.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> nf2remover(nf2.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});

		// create dest files
		const wchar_t* pF5 = L"New5.txt";
		const wchar_t* pF6 = L"New6.txt";


		wstring nf5 = stdCombinePath(testRoot(), pF5);
		wstring nf6 = stdCombinePath(testRoot(), pF6);

		unique_ptr<const wchar_t, void(*)(const wchar_t*)> nf5remover(nf5.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});
		unique_ptr<const wchar_t, void(*)(const wchar_t*)> nf6remover(nf6.c_str(),
			[](const wchar_t* p) {
			DeleteFile(p);
			EXPECT_FALSE(PathFileExists(p));
		});


		vector<wstring> wsS;
		vector<wstring> wsD;
		wsS.push_back(nf1);
		wsS.push_back(nf2);
		wsD.push_back(nf5);
		wsD.push_back(nf6);

		EXPECT_TRUE(PathFileExists(nf1.c_str()));
		EXPECT_TRUE(PathFileExists(nf2.c_str()));
		EXPECT_TRUE(!PathFileExists(nf5.c_str()));
		EXPECT_TRUE(!PathFileExists(nf6.c_str()));
		EXPECT_TRUE(SHCopyFileEx( wsS, wsD, FOF_NO_UI) == 0);
		EXPECT_TRUE(PathFileExists(nf1.c_str()));
		EXPECT_TRUE(PathFileExists(nf2.c_str()));
		EXPECT_TRUE(PathFileExists(nf5.c_str()));
		EXPECT_TRUE(PathFileExists(nf6.c_str()));
	}
}

