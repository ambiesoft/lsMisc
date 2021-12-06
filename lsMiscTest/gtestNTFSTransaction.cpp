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

#include "debug.h"

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
	if (!WriteFile(file, pText, size, &dwWritten, NULL) ||
		size != dwWritten)
	{
		return false;
	}
	return true;
}
TEST(NTFSTransaction, MoveFileAtomicTest)
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
}
