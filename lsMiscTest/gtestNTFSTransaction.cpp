#include "stdafx.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "gtest/gtest.h"

#include "../stdosd/stdosd.h"
#include "../CHandle.h"
#include "../GetBackupFile.h"
#include "../NTFSTransaction.h"

#include "../DebugNew.h"

using namespace std;
using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

vector<BYTE> ReadAllText(const wchar_t* filename)
{
	CFileHandle file(CreateFile(filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL));
	if (!file)
		return vector<BYTE>();
	DWORD dwRead;
	BYTE buff;
	vector<BYTE> v;
	do
	{
		if (!ReadFile(file, &buff, sizeof(buff), &dwRead, NULL))
		{
			return vector<BYTE>();
		}
		if (dwRead == 0)
			break;
		if (dwRead != 1)
			return vector<BYTE>();
		v.push_back(buff);
	} while (true);
	return v;
}
bool WriteAllText(const wchar_t* filename, const char* pText, size_t size)
{
	if (size == -1)
		size = strlen(pText);
	CFileHandle file(CreateFile(filename,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL));
	if (!file)
		return false;
	DWORD dwWritten;
	if (!WriteFile(file, pText, (DWORD)size, &dwWritten, NULL) ||
		size != dwWritten)
	{
		return false;
	}
	return !!SetEndOfFile(file);
}
TEST(NTFSTransaction, MoveFileAtomicNormal)
{
	const char* t1 = "aaaaaaaaaaaaa";
	const char* t2 = "bbbbbbbbbbbbbbbbbbbbbbb";

	const wstring filename1 = stdCombinePath(
		stdGetParentDirectory(stdGetModuleFileName()),
		L"file1");
	const wstring filename2 = stdCombinePath(
		stdGetParentDirectory(stdGetModuleFileName()),
		L"file2");

	EXPECT_TRUE(WriteAllText(filename1.c_str(), t1, -1));
	EXPECT_TRUE(WriteAllText(filename2.c_str(), t2, -1));

	const wstring filenamebk = GetBackupFile(filename1.c_str());

	// swap 1 2
	// 1 and 2 exists
	// 1->bk (bk and 2 exists)
	// 2->1 (bk and 1 exists)
	// bk->2 (1 and 2 exists)
	SRCDESTVECTOR sdv;
	sdv.push_back(SRCDEST(filename1, filenamebk));
	sdv.push_back(SRCDEST(filename2, filename1));
	sdv.push_back(SRCDEST(filenamebk, filename2));

	EXPECT_TRUE(MoveFileAtomic(sdv));

	auto r1 = ReadAllText(filename1.c_str());
	auto r2 = ReadAllText(filename2.c_str());

	EXPECT_EQ(strlen(t1), r2.size());
	EXPECT_EQ(0, memcmp(t1, r2.data(), r2.size()));

	EXPECT_EQ(strlen(t2), r1.size());
	EXPECT_EQ(0, memcmp(t2, r1.data(), r1.size()));
}

TEST(NTFSTransaction, MoveFileAtomicAbort)
{
	const char* t1 = "111";
	const char* t2 = "2222";

	const wstring filename1 = stdCombinePath(
		stdGetParentDirectory(stdGetModuleFileName()),
		L"file1");
	const wstring filename2 = stdCombinePath(
		stdGetParentDirectory(stdGetModuleFileName()),
		L"file2");

	EXPECT_TRUE(WriteAllText(filename1.c_str(), t1, -1));
	EXPECT_TRUE(WriteAllText(filename2.c_str(), t2, -1));

	const wstring filenamebk = GetBackupFile(filename1.c_str());

	const wstring filelockingname = filename1 + L"lock";
	
	// this prevents file move
	CFileHandle filelock(CreateFile(filelockingname.c_str(),
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL));
	EXPECT_TRUE(filelock);
	


	SRCDESTVECTOR sdv;
	sdv.push_back(SRCDEST(filename1, filenamebk));
	sdv.push_back(SRCDEST(filename2, filelockingname)); // this must fail

	EXPECT_FALSE(MoveFileAtomic(sdv));

	// first move must be rollbacked
	// filename1 must be intace
	EXPECT_TRUE(!PathFileExists(filenamebk.c_str()));

	EXPECT_TRUE(PathFileExists(filename1.c_str()));
	auto r1 = ReadAllText(filename1.c_str());
	EXPECT_EQ(strlen(t1), r1.size());
	EXPECT_EQ(0, memcmp(t1, r1.data(), r1.size()));

	EXPECT_TRUE(PathFileExists(filename2.c_str()));
	auto r2 = ReadAllText(filename2.c_str());
	EXPECT_EQ(strlen(t2), r2.size());
	EXPECT_EQ(0, memcmp(t2, r2.data(), r2.size()));
}
