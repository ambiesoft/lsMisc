#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <cassert>

#include <gtest/gtest.h>

#include "../CommandLineString.h"

using namespace Ambiesoft;
using namespace std;

static void isSameResult(LPWSTR pCL, long line)
{
	int argc1 = 0;
	LPWSTR* argv1 = NULL;
	argv1 = CCommandLineString::getCommandLineArg(pCL,&argc1);

	int argc2 = 0;
	LPWSTR* argv2 = NULL;
	argv2 = CommandLineToArgvW(pCL, &argc2);

	wstringstream ws;
	ws << L"p=" << pCL << L"\nline=" << line;

	EXPECT_EQ(argc1, argc2) << ws.str();

	for (int i = 0; i < argc2; ++i)
	{
		LPWSTR CCommandLineString_ = argv1[i];
		LPWSTR ShellCommnadLine = argv2[i];
		EXPECT_STREQ(CCommandLineString_, ShellCommnadLine) << ws.str();
	}

	CCommandLineString::freeCommandLineArg(argv1);
	LocalFree(argv2);
}

TEST(CommandLineString, SameWithOtherMethods)
{
	LPWSTR pC1;
	pC1 = L"aaa.exe \"aaa bbb\"";
	isSameResult(pC1, __LINE__);

	pC1 = L"a";
	isSameResult(pC1, __LINE__);

	pC1 = nullptr;
	{
		int dummy;
		EXPECT_DEATH(CCommandLineString::getCommandLineArg(pC1, &dummy), "");
		EXPECT_DEATH(CommandLineToArgvW(pC1, &dummy), "");
	}
	

	pC1 = L"";
	isSameResult(pC1, __LINE__);

	pC1 = L"\"";
	isSameResult(pC1, __LINE__);
	
	pC1 = L"\"\"";
	isSameResult(pC1, __LINE__);
	
	pC1 = L"\"\" a";
	isSameResult(pC1, __LINE__);

	// a ""bb"""""
	pC1 = L"a \"\"bb\"\"\"\"\"";
	// not equal but crt is same as mine
	// isSameResult(pC1, __LINE__);
}
TEST(CommandLineString, subStringBasic)
{
	LPWSTR pC1;

	{
		pC1 = L"abc";
		CCommandLineString c(pC1);
		EXPECT_EQ(c.getCount(), 1);
		EXPECT_STREQ(c.getArg(0).c_str(), pC1);
	}

	{
		pC1 = L"aaa.exe file1.txt \"space file.txt\"";
		CCommandLineString cls(pC1);
		EXPECT_STREQ(cls.subString(0).c_str(), L"aaa.exe file1.txt \"space file.txt\"");
		EXPECT_STREQ(cls.subString(1).c_str(), L"file1.txt \"space file.txt\"");
		EXPECT_STREQ(cls.subString(2).c_str(), L"\"space file.txt\"");
	}

	{
		pC1 = L"\"a a\"";
		CCommandLineString c(pC1);
		EXPECT_EQ(c.getCount(), 1);
		EXPECT_STREQ(c.getArg(0).c_str(), L"a a");
	}

	{
		pC1 = L"\"a a\" bbb";
		CCommandLineString c(pC1);
		EXPECT_EQ(c.getCount(), 2);
		EXPECT_STREQ(c.getArg(0).c_str(), L"a a");
		EXPECT_STREQ(c.getArg(1).c_str(), L"bbb");
	}

	{
		pC1 = L"aa\"b b\"";
		CCommandLineString c(pC1);
		EXPECT_EQ(c.getCount(), 1);
		EXPECT_STREQ(c.getArg(0).c_str(), L"aab b");
	}

	{
		pC1 = L"aa\"b b\" ccc";
		CCommandLineString c(pC1);
		EXPECT_EQ(c.getCount(), 2);
		EXPECT_STREQ(c.getArg(0).c_str(), L"aab b");
		EXPECT_STREQ(c.getArg(1).c_str(), L"ccc");
	}
}

TEST(CommandLineString, subStringComplex)
{
	LPWSTR pC1;
	{
		pC1 = L"aaa.exe \"aaa bbb\" aa fff feee aaa\"bbb\" ";
		CCommandLineString cls(pC1);
		EXPECT_STREQ(cls.subString(0).c_str(), L"aaa.exe \"aaa bbb\" aa fff feee aaa\"bbb\" ");
		EXPECT_STREQ(cls.subString(1).c_str(), L"\"aaa bbb\" aa fff feee aaa\"bbb\" ");
		EXPECT_STREQ(cls.subString(2).c_str(), L"aa fff feee aaa\"bbb\" ");
		EXPECT_STREQ(cls.subString(3).c_str(), L"fff feee aaa\"bbb\" ");
		EXPECT_STREQ(cls.subString(4).c_str(), L"feee aaa\"bbb\" ");
		EXPECT_STREQ(cls.subString(5).c_str(), L"aaa\"bbb\" ");
		EXPECT_STREQ(cls.subString(6).c_str(), L"");
	}

	{
		pC1 = L"\"\"\"";
		CCommandLineString cls(pC1);
		EXPECT_STREQ(cls.subString(0).c_str(), pC1);
		EXPECT_EQ(cls.getCount(), 1);
		EXPECT_STREQ(cls.getArg(0).c_str(), L"\"");
	}
	{
		// a ""bb"""
		pC1 = L"a \"\"bb\"\"\"";
		CCommandLineString cls(pC1);
		EXPECT_EQ(cls.getCount(), 2);
		EXPECT_STREQ(cls.subString(0).c_str(), pC1);
		EXPECT_STREQ(cls.getArg(0).c_str(), L"a");
		EXPECT_STREQ(cls.getArg(1).c_str(), L"bb\"");
	}
	{
		pC1 = L"\"\"\"a\"\"\"";
		CCommandLineString cls(pC1);
		EXPECT_STREQ(cls.subString(0).c_str(), pC1);
		EXPECT_STREQ(cls.getArg(0).c_str(), L"\"a\"");
	}
	{
		pC1 = L"\"\"\"a b\"\"\"";
		CCommandLineString cls(pC1);
		EXPECT_STREQ(cls.subString(0).c_str(), pC1);
		EXPECT_STREQ(cls.getArg(0).c_str(), L"\"a b\"");
	}
	{
		pC1 = L"aaa.exe \"\"\"aaa bbb ccc\"\"\"";
		CCommandLineString cls(pC1);
		EXPECT_STREQ(cls.subString(0).c_str(), L"aaa.exe \"\"\"aaa bbb ccc\"\"\"");
		EXPECT_STREQ(cls.subString(1).c_str(), L"\"\"\"aaa bbb ccc\"\"\"");
		EXPECT_STREQ(cls.getArg(1).c_str(), L"\"aaa bbb ccc\"");
	}
	{
		pC1 = L"aaa.exe hello\"to the  entire\"world";
		CCommandLineString cls(pC1);
		EXPECT_STREQ(cls.subString(0).c_str(), L"aaa.exe hello\"to the  entire\"world");
		EXPECT_STREQ(cls.subString(1).c_str(), L"hello\"to the  entire\"world");
		EXPECT_STREQ(cls.getArg(1).c_str(), L"helloto the  entireworld");
	}
	{
		// "T ""Hi!"""
		pC1 = L"\"T \"\"Hi!\"\"\"";
		CCommandLineString cls(pC1);
		EXPECT_STREQ(cls.getArg(0).c_str(), L"T \"Hi!\"");
	}
}

TEST(CommandLineString, subStringRemove)
{
	LPWSTR pC1;

	{
		pC1 = L"aaa.exe \"aaa bbb\" aa fff feee 12345";
		CCommandLineString cls(pC1);
		cls.remove(2, 3);  // remove 3 items from index 2
		EXPECT_STREQ(L"aaa.exe \"aaa bbb\" 12345", cls.toString().c_str());
	}
	{
		pC1 = L"aaa.exe \"aaa bbb\" aa fff feee '123 45'";
		CCommandLineString cls(pC1);
		cls.remove(2, 3);  // remove 3 items from index 2
		EXPECT_STREQ(L"aaa.exe \"aaa bbb\" '123 45'", cls.toString().c_str());
	}
}

TEST(CommandLineString, ArgcArgv)
{
	LPWSTR pC1;
	pC1 = L"aaa.exe \"aaa bbb\" aa fff feee aaa\"bbb\" ";

	{
		CCommandLineString cms1(pC1);
		CCommandLineString cms2(pC1);
		EXPECT_EQ(cms1, cms2);
	}
	
	//{
	//	CCommandLineString cms1(pC1);

	//	int nArg;
	//	LPWSTR* pArgv = CommandLineToArgvW(pC1, &nArg);
	//	CCommandLineString cms2(nArg, pArgv);

	//	EXPECT_NE(cms1, cms2);
	//	EXPECT_TRUE(cms1.SyntaxEqual(cms2));
	//}

	{
		wchar_t* argv[] = {
			L"exe.exe",
			L"-h",
			L"-b",
			L"-path",
			L"c:\\aaa\ttt bbb\aa",
			L"file1.txt",
			L"file2.txt",
			L"file3.txt",
			L"space file.txt",
			NULL
		};

		int argc = _countof(argv) - 1;
		CCommandLineString cms(argc, argv);
		EXPECT_EQ(cms.getCount(), argc);
		for (int i = 0; i < argc; ++i)
		{
			EXPECT_STREQ(cms.getArg(i).c_str(), argv[i]);
		}
	}
}

TEST(CommandLineString, DQinsideDQ)
{
	LPWSTR pC1;
	pC1 = L"aaa.exe \"-DCR_CLANG_REVISION=\\\"336424-1\\\"\"";
	isSameResult(pC1, __LINE__);

	pC1 = L"aaa.exe \"-DCR_CLANG_REVISION=\\\"336424-1\\\"";
	isSameResult(pC1, __LINE__);

	pC1 = L"aaa.exe \"-DCR_CLANG_REVISION=\\\"336424-1\\";
	isSameResult(pC1, __LINE__);
}

TEST(CommandLineString, GoogleTypeCommandLineParts)
{
	LPWSTR pC1;

	{
		pC1 = L"\"C:\\aaa bbb\\fff.exe\" --log-file=\"C:\\aaa bbb ccc\\aaa.log\" --enable-log --some-thing=100";
		CCommandLineString cmd(pC1);
		EXPECT_EQ(cmd.getCount(), 4);
		EXPECT_STREQ(cmd.toString().c_str(), pC1);

		EXPECT_STREQ(cmd.getArg(1).c_str(), L"--log-file=C:\\aaa bbb ccc\\aaa.log");
		EXPECT_STREQ(cmd.getArg(2).c_str(), L"--enable-log");
		EXPECT_STREQ(cmd.getArg(3).c_str(), L"--some-thing=100");
	}

	{
		pC1 = L"\"C:\\aaa bbb\\fff.exe\" --log-file=\"C:\\aaa bbb ccc\\aaa.log\" --limit-render-process=5 --enable-log --some-thing=100";
		CCommandLineString cmd(pC1);
		EXPECT_EQ(cmd.getCount(), 5);
		EXPECT_STREQ(cmd.toString().c_str(), pC1);

		EXPECT_STREQ(cmd.getArg(1).c_str(), L"--log-file=C:\\aaa bbb ccc\\aaa.log");
		EXPECT_STREQ(cmd.getArg(2).c_str(), L"--limit-render-process=5");
		EXPECT_STREQ(cmd.getArg(3).c_str(), L"--enable-log");
		EXPECT_STREQ(cmd.getArg(4).c_str(), L"--some-thing=100");

		cmd.remove(2);
		EXPECT_STREQ(cmd.getArg(1).c_str(), L"--log-file=C:\\aaa bbb ccc\\aaa.log");
		EXPECT_STREQ(cmd.getArg(2).c_str(), L"--enable-log");
		EXPECT_STREQ(cmd.getArg(3).c_str(), L"--some-thing=100");

		EXPECT_STREQ(cmd.toString().c_str(),
			L"\"C:\\aaa bbb\\fff.exe\" --log-file=\"C:\\aaa bbb ccc\\aaa.log\" --enable-log --some-thing=100");
	}
}

TEST(CommandLineString, MultiByte)
{
	// TODO: test with shiftjs multibyte
}