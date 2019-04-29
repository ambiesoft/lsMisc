#include "stdafx.h"
#include <tchar.h>
#include <iostream>
#include <sstream>

#include <stdint.h>

#include <gtest/gtest.h>

#include "../UrlEncode.h"
#include "../CommandLineParser.h"



using namespace Ambiesoft;
using namespace std;

template <class InputIterator>
void outvalue(wstringstream& os, InputIterator first, InputIterator last)
{
	while (first != last) {
		os << *first;
		++first;
	}
}

TEST(CommandLineParser, IterateOur)
{
	vector<wstring> vs;
	vs.push_back(L"-a");
	vs.push_back(L"-b");
	vs.push_back(L"-c");

	wstringstream wss;
	outvalue(wss, vs.begin(), vs.end());
	EXPECT_TRUE(wss.str() == L"-a-b-c");
}
#define ACTULAPATHA "c:\\aaa\ttt bbb\aa"
#define ACTULAPATHW L"c:\\aaa\ttt bbb\aa"

TEST(CommandLineParser, BasicWchar)
{
	vector<wstring> vs;
	vs.push_back(L"-a");
	vs.push_back(L"-b");
	vs.push_back(L"-c");

	wchar_t* opXYZ[] = {
		L"-xyz",
		L"-bbb",
	};

	bool isHelp = false;
	bool isABC = false;
	bool isXYZ = false;
	wstring path;
	COption opMain(L"", ArgCount::ArgCount_Infinite);
	CCommandLineParser clp;
	clp.AddOption(L"-h", L"/?", 0, &isHelp);
	clp.AddOptionRange(vs.begin(), vs.end(), 0, &isABC);
	clp.AddOptionRange(opXYZ, opXYZ + _countof(opXYZ), 0, &isXYZ);
	clp.AddOption(L"-path", 1, &path);
	clp.AddOption(&opMain);
	wchar_t* argv[] = {
		L"exe.exe",
		L"-h",
		L"-b",
		L"-path",
		ACTULAPATHW,
		L"file1.txt",
		L"file2.txt",
		L"file3.txt",
		L"space file.txt",
		NULL
	};
	clp.Parse(_countof(argv)-1, argv);

	EXPECT_TRUE(isHelp);
	EXPECT_TRUE(isABC);
	EXPECT_STREQ(path.c_str(), ACTULAPATHW);
	
	EXPECT_EQ(opMain.getValueCount(), 4);
	EXPECT_STREQ(opMain.getFirstValue().c_str(), L"file1.txt");
	EXPECT_STREQ(opMain.getValue(0).c_str(), L"file1.txt");
	EXPECT_STREQ(opMain.getValue(1).c_str(), L"file2.txt");
	EXPECT_STREQ(opMain.getValue(2).c_str(), L"file3.txt");
	EXPECT_STREQ(opMain.getValue(3).c_str(), L"space file.txt");
	EXPECT_STREQ(opMain.getValueStrings().c_str(), L"file1.txt file2.txt file3.txt \"space file.txt\"");
}


TEST(CommandLineParser, BasicChar)
{
	vector<string> vs;
	vs.push_back("-a");
	vs.push_back("-b");
	vs.push_back("-c");

	char* opXYZ[] = {
		"-xyz",
		"-bbb",
	};

	bool isHelp = false;
	bool isABC = false;
	bool isXYZ = false;
	string path;
	COptionA opMain("", ArgCount::ArgCount_Infinite);
	CCommandLineParserA clp;
	clp.AddOption("-h", "/?", 0, &isHelp);
	clp.AddOptionRange(vs.begin(), vs.end(), 0, &isABC);
	clp.AddOptionRange(opXYZ, opXYZ + _countof(opXYZ), 0, &isXYZ);
	clp.AddOption("-path", 1, &path);
	clp.AddOption(&opMain);
	char* argv[] = {
		"exe.exe",
		"-h",
		"-b",
		"-path",
		ACTULAPATHA,
		"file1.txt",
		"file2.txt",
		"file3.txt",
		"space file.txt",
		NULL
	};
	clp.Parse(_countof(argv) - 1, argv);

	EXPECT_TRUE(isHelp);
	EXPECT_TRUE(isABC);
	EXPECT_STREQ(path.c_str(), ACTULAPATHA);

	EXPECT_EQ(opMain.getValueCount(), 4);
	EXPECT_STREQ(opMain.getFirstValue().c_str(), "file1.txt");
	EXPECT_STREQ(opMain.getValue(0).c_str(), "file1.txt");
	EXPECT_STREQ(opMain.getValue(1).c_str(), "file2.txt");
	EXPECT_STREQ(opMain.getValue(2).c_str(), "file3.txt");
	EXPECT_STREQ(opMain.getValue(3).c_str(), "space file.txt");
	EXPECT_STREQ(opMain.getValueStrings().c_str(), "file1.txt file2.txt file3.txt \"space file.txt\"");
}

TEST(CommandLineParser, OptionConstructorAll)
{
	// Default Ctor for Main arg and inifinite
	{
		CCommandLineParser parser;
		COption op1;
		parser.AddOption(&op1);

		wchar_t* argv[] = {
			L"exe.exe",
			L"-h",
			L"-b",
			L"-path",
			ACTULAPATHW,  // Main arg starts here
			L"file1.txt",
			L"file2.txt",
			L"file3.txt",
			L"space file.txt",
			NULL
		};

		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);
		int i = 0;
		for (size_t iArg = 4; iArg < argc; ++iArg, ++i)
		{
			EXPECT_STREQ(argv[iArg], op1.getValue(i).c_str());
		}
	}
}

TEST(CommandLineParser, Bool)
{
	// Specified
	{
		CCommandLineParser parser;
		bool b = false;
		parser.AddOption(L"-b", 0, &b);

		wchar_t* argv[] = {
			L"exe.exe",
			L"-h",
			L"-b",
			L"-path",
			ACTULAPATHW,  // Main arg starts here
			L"file1.txt",
			L"file2.txt",
			L"file3.txt",
			L"space file.txt",
			NULL
		};
		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);

		EXPECT_TRUE(b);
	}
	// UnSpecified
	{
		CCommandLineParser parser;
		bool b = false;
		parser.AddOption(L"-b", 0, &b);

		wchar_t* argv[] = {
			L"exe.exe",
			L"-h",
			L"-cb",
			L"-path",
			ACTULAPATHW,  // Main arg starts here
			L"file1.txt",
			L"file2.txt",
			L"file3.txt",
			L"space file.txt",
			NULL
		};
		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);

		EXPECT_FALSE(b);
	}
}

TEST(CommandLineParser, Int)
{
	// Specified
	{
		CCommandLineParser parser;
		int intval = 0;
		parser.AddOption(L"-i", 1, &intval);

		wchar_t* argv[] = {
			L"exe.exe",
			L"-h",
			L"-i",
			L"1001",
			L"-path",
			ACTULAPATHW,  // Main arg starts here
			L"file1.txt",
			L"file2.txt",
			L"file3.txt",
			L"space file.txt",
			NULL
		};
		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);

		EXPECT_EQ(intval, 1001);
	}
	// Unspecified
	{
		CCommandLineParser parser;
		int intval = -1;
		parser.AddOption(L"-i", 1, &intval);

		wchar_t* argv[] = {
			L"exe.exe",
			L"-h",
			L"-fi",
			L"1001",
			L"-path",
			ACTULAPATHW,  // Main arg starts here
			L"file1.txt",
			L"file2.txt",
			L"file3.txt",
			L"space file.txt",
			NULL
		};
		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);

		EXPECT_EQ(intval, -1);
	}
}

TEST(CommandLineParser, Dicregate)
{
	wchar_t* argv[] = {
		L"exe.exe",
		L"-page",
		L"http://example.com/",
		L"-url",
		L"http://example.com/my%20file.zip",
		L"-hostwnd",
		L"12345",
		L"-hostpid",
		L"54321",
		L"-did",
		L"333",
		L"-size",
		L"333333333333333333",
		L"-lng",
		L"ja",
		NULL
	};
	size_t argc = _countof(argv) - 1;

	CCommandLineParser parser;
	COption page(L"-page", ExactCount::Exact_1);
	parser.AddOption(&page);

	COption url(L"-url", ExactCount::Exact_1);
	parser.AddOption(&url);

	//COption target(L"-target",1);
	//parser.AddOption(&target);

	COption hosthwnd(L"-hostwnd", ExactCount::Exact_1);
	parser.AddOption(&hosthwnd);

	COption hostpid(L"-hostpid", ExactCount::Exact_1);
	parser.AddOption(&hostpid);

	COption did(L"-did", ExactCount::Exact_1);
	parser.AddOption(&did);

	COption size(L"-size", ExactCount::Exact_1);
	parser.AddOption(&size);

	COption lang(L"-lang", ExactCount::Exact_1);
	parser.AddOption(&lang);

	parser.Parse(argc, argv);


	if (!page.hadOption() || !page.hadValue())
	{
		AfxMessageBox(L"No page");
		return ;
	}

	if (!url.hadOption() || !url.hadValue())
	{
		AfxMessageBox(L"No URL");
		return ;
	}

	//if (!target.hadOption() || !target.hadValue())
	//{
	//	AfxMessageBox(L"No Target");
	//	return FALSE;
	//}

	if (!hosthwnd.hadOption() || !hosthwnd.hadValue())
	{
		AfxMessageBox(L"No host");
		return ;
	}
	if (!hostpid.hadOption() || !hostpid.hadValue())
	{
		AfxMessageBox(L"No host");
		return ;
	}

	if (!did.hadOption() || !did.hadValue())
	{
		AfxMessageBox(L"No did");
		return ;
	}

	if (!size.hadOption() || !size.hadValue())
	{
		AfxMessageBox(L"No size");
		return ;
	}


	EXPECT_EQ((HWND)(int64_t)_wtol(hosthwnd.getFirstValue().c_str()), (HWND)(int64_t)(12345));

	
	EXPECT_EQ(_wtoi64(size.getFirstValue().c_str()) , 333333333333333333LL);

	DWORD dwHostPID = _wtol(hostpid.getFirstValue().c_str());
	EXPECT_EQ(54321, dwHostPID);

	EXPECT_STREQ(UrlDecodeStd<wstring>(page.getFirstValue().c_str()).c_str(), L"http://example.com/");
	EXPECT_STREQ(UrlDecodeStd<wstring>(url.getFirstValue().c_str()).c_str(), L"http://example.com/my file.zip");
}