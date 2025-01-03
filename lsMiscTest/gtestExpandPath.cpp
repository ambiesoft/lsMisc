﻿
#include <fstream>

#include "gtest/gtest.h"

#include "../ExpandPath.h"
#include "../stdosd/stdosd.h"

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

class ExpandPathTest : public ::testing::Test {
private:
	wstring baseDir_;
	vector<wstring> allDirs_;
	vector<wstring> allFiles_;
	void SetUp() override {
		/*
		./secondDir/
		./secondDir/secfile1
		./secondDir/secfile2
		./secondDir/thirdDir/
		./secondDir/thirdDir/thirdfile1
		./secondDir/thirdDir/thirdfile2
		./file1
		./file2
		*/
		baseDir_ = stdCombinePath(
			stdGetParentDirectory(stdGetModuleFileName()),
			L"TestExpandPath");
		CreateDirectory(baseDir_.c_str(), NULL);
		allDirs_.push_back(baseDir_);
		EXPECT_TRUE(stdDirectoryExists(baseDir_.c_str()));
		{
			wstring firstfile1 = stdCombinePath(baseDir_, L"file1");
			wofstream ofs1(firstfile1);
			allFiles_.push_back(firstfile1);

			wstring firstfile2 = stdCombinePath(baseDir_, L"file2");
			wofstream ofs2(firstfile2);
			allFiles_.push_back(firstfile2);
		}

		wstring secondDir = stdCombinePath(baseDir_, L"secondDir");
		CreateDirectory(secondDir.c_str(), NULL);
		allDirs_.push_back(secondDir);
		EXPECT_TRUE(stdDirectoryExists(secondDir.c_str()));
		{
			wstring secondfile1 = stdCombinePath(secondDir, L"secondfile1");
			wofstream ofs21(secondfile1);
			allFiles_.push_back(secondfile1);

			wstring secondfile2 = stdCombinePath(secondDir, L"secondfile2");
			wofstream ofs222(secondfile2);
			allFiles_.push_back(secondfile2);
		}

		wstring thirdDir = stdCombinePath(secondDir, L"thirdDir");
		CreateDirectory(thirdDir.c_str(), NULL);
		allDirs_.push_back(thirdDir);
		EXPECT_TRUE(stdDirectoryExists(thirdDir.c_str()));
		{
			wstring thirdfile1 = stdCombinePath(thirdDir, L"thirdfile1");
			wofstream ofs31(thirdfile1);
			allFiles_.push_back(thirdfile1);

			wstring thirdfile2 = stdCombinePath(thirdDir, L"thirdfile2");
			wofstream ofs32(thirdfile2);
			allFiles_.push_back(thirdfile2);
		}
	}

	void TearDown() override {
		for (auto&& file : allFiles_)
		{
			EXPECT_TRUE(!!DeleteFile(file.c_str()));
		}

		size_t count = 0;
		while (count != allDirs_.size())
		{
			count = allDirs_.size();
			vector<vector<wstring>::iterator> removedIts;
			for (vector<wstring>::iterator it = allDirs_.begin();
				it != allDirs_.end() ; ++it)
			{
				if (RemoveDirectory(it->c_str()))
					removedIts.push_back(it);
			}
			for (auto&& it : removedIts)
				allDirs_.erase(it);
		}
		EXPECT_EQ(count, 0);
	}

public:
	wstring baseDir() const {
		return baseDir_;
	}
};

TEST_F(ExpandPathTest,Basic)
{
	{
		vector<wstring> results;

		results.clear();
		ExpandPath(baseDir() + L"/*", results);
		EXPECT_EQ(results.size(), 3);
	}
}