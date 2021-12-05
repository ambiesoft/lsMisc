#include "stdafx.h"
//#include <afx.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <cassert>
#include <thread>

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
	wchar_t* opXYZ[] = {
		L"-xyz",
		L"-bbb",
	};

	bool isHelp = false;
	bool isABC = false;
	bool isXYZ = false;
	wstring path;
	COption opMain(L"", ArgCount::ArgCount_OneToInfinite);
	CCommandLineParser clp;
	clp.AddOptionRange({ wstring(L"-h"), wstring(L"/?") }, 0, &isHelp);
	clp.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &isABC);
	clp.AddOptionRange({ L"-xyz", L"-bbb" }, 0, &isXYZ);
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
	bool isHelp = false;
	bool isABC = false;
	bool isXYZ = false;
	string path;
	COptionA opMain("", ArgCount::ArgCount_TwoToInfinite);
	CCommandLineParserA clp;
	clp.AddOptionRange({ string("-h"), string("/?") }, 0, &isHelp);
	clp.AddOptionRange({ "-a","-b","-c" }, 0, &isABC);
	clp.AddOptionRange({ "-xyz", "-bbb" }, 0, &isXYZ);
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
	const wchar_t* const argv[] = {
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


	// Default Ctor for Main arg and inifinite
	{
		CCommandLineParser parser;
		COption op1;
		parser.AddOption(&op1);

		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);
		int i = 0;
		for (size_t iArg = 4; iArg < argc; ++iArg, ++i)
		{
			EXPECT_STREQ(argv[iArg], op1.getValue(i).c_str());
		}
	}

	{
		CCommandLineParser parser;
		COption op1({ L"-h", L"-b" }, 0);
		parser.AddOption(&op1);

		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);
		EXPECT_TRUE(op1.hadOption());
	}
	{
		CCommandLineParser parser;
		COption op1({ L"-xxx", L"-yyy" }, 0);
		parser.AddOption(&op1);

		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);
		EXPECT_FALSE(op1.hadOption());
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
		// MyMessageBox(L"No page");
		EXPECT_TRUE(false);
	}

	if (!url.hadOption() || !url.hadValue())
	{
		// AfxMessageBox(L"No URL");
		EXPECT_TRUE(false);
		return ;
	}

	//if (!target.hadOption() || !target.hadValue())
	//{
	//	AfxMessageBox(L"No Target");
	//	return FALSE;
	//}

	if (!hosthwnd.hadOption() || !hosthwnd.hadValue())
	{
		// AfxMessageBox(L"No host");
		EXPECT_TRUE(false);
		return ;
	}
	if (!hostpid.hadOption() || !hostpid.hadValue())
	{
		// AfxMessageBox(L"No host");
		EXPECT_TRUE(false);
		return ;
	}

	if (!did.hadOption() || !did.hadValue())
	{
		// AfxMessageBox(L"No did");
		EXPECT_TRUE(false);
		return ;
	}

	if (!size.hadOption() || !size.hadValue())
	{
		// AfxMessageBox(L"No size");
		EXPECT_TRUE(false);
		return ;
	}


	EXPECT_EQ((HWND)(int64_t)_wtol(hosthwnd.getFirstValue().c_str()), (HWND)(int64_t)(12345));

	
	EXPECT_EQ(_wtoi64(size.getFirstValue().c_str()) , 333333333333333333LL);

	DWORD dwHostPID = _wtol(hostpid.getFirstValue().c_str());
	EXPECT_EQ(54321, dwHostPID);

	EXPECT_STREQ(UrlDecodeStd<wstring>(page.getFirstValue().c_str()).c_str(), L"http://example.com/");
	EXPECT_STREQ(UrlDecodeStd<wstring>(url.getFirstValue().c_str()).c_str(), L"http://example.com/my file.zip");
}

TEST(CommandLineParser, SameOption)
{
	{
		wchar_t* argv[] = {
		L"exe.exe",
		NULL
		};
		CCommandLineParser parser;
		bool a=false;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_FALSE(a);
	}
	{
		wchar_t* argv[] = {
		L"exe.exe",
		L"-a",
		L"-b",
		L"-c",
		NULL
		};
		CCommandLineParser parser;
		bool a;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_TRUE(a);
	}
	{
		wchar_t* argv[] = {
		L"exe.exe",
		L"-a",
		NULL
		};
		CCommandLineParser parser;
		bool a;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_TRUE(a);
	}
	{
		wchar_t* argv[] = {
		L"exe.exe",
		L"-b",
		NULL
		};
		CCommandLineParser parser;
		bool a;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_TRUE(a);
	}
	{
		wchar_t* argv[] = {
		L"exe.exe",
		L"-c",
		NULL
		};
		CCommandLineParser parser;
		bool a;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_TRUE(a);
	}


	// wstring initializer
	{
		wchar_t* argv[] = {
		L"exe.exe",
		L"-c",
		NULL
		};
		CCommandLineParser parser;
		bool a;
		parser.AddOptionRange({ wstring(L"-a"),wstring(L"-b"),wstring(L"-c") }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_TRUE(a);
	}
}

TEST(CommandLineParser, EndWithDQComma)
{
	{
		wchar_t* argv[] = {
		L"exe.exe",
		L"\"aaa\",",
		NULL
		};
		CCommandLineParser parser;
		
		COption opMain(L"", ArgCount::ArgCount_ZeroToInfinite);
		parser.AddOption(&opMain);

		parser.Parse(_countof(argv) - 1, argv);

		EXPECT_EQ(opMain.getValueCount(), 1);
		EXPECT_STREQ(opMain.getValue(0).c_str(), L"\"aaa\",");
	}
}

TEST(CommandLineParser, Help)
{
	{
		char* argv[] = {
			   "myapp.exe",
			   "-op delete",
			   NULL
		};
		CCommandLineParserA parser(CaseFlags_Default, "Rename or remove a folder", "lsMiscTest");

		COptionA optionDefault("",
			ArgCount::ArgCount_ZeroToInfinite,
			ArgEncodingFlags_Default,
			("specify directory"));
		parser.AddOption(&optionDefault);

		bool bHelp = false;
		parser.AddOptionRange({ "-h", "/h", "/?","--help","-H", "-hh"},
			0,
			&bHelp,
			ArgEncodingFlags_Default,
			("show help"));

		bool bVersion = false;
		parser.AddOptionRange({ "-v", "/v", "--version" },
			0,
			&bVersion,
			ArgEncodingFlags_Default,
			("show version"));

		string operation;
		parser.AddOption("-op",
			1,
			&operation,
			ArgEncodingFlags_Default,
			("Operation: One of 'rename', 'trash', 'delete'"));

		parser.Parse(_countof(argv) - 1, argv);

		EXPECT_EQ(IDOK, MessageBoxA(nullptr, parser.getHelpMessage().c_str(), "", MB_OKCANCEL));
	}
	{
		wchar_t* argv[] = {
			   L"myapp.exe",
			   L"-op delete -pri 3 \"C:\\aaa bbb\\cc dd\\xxx.txt\"",
			   NULL
		};
		CCommandLineParser parser(CaseFlags_Default, L"Rename or remove a folder", L"lsMiscTestWchar");

		COption optionDefault(L"",
			ArgCount::ArgCount_OneToInfinite,
			ArgEncodingFlags_Default,
			(L"specify directory"));
		parser.AddOption(&optionDefault);

		bool bHelp = false;
		parser.AddOptionRange({ L"-h", L"/h" },
			0,
			&bHelp,
			ArgEncodingFlags_Default,
			(L"show help"));

		bool bVersion = false;
		parser.AddOptionRange({ L"-v", L"/v" },
			0,
			&bVersion,
			ArgEncodingFlags_Default,
			(L"show version"));

		wstring operation;
		parser.AddOption(L"-op",
			1,
			&operation,
			ArgEncodingFlags_Default,
			(L"Operation: One of 'rename', 'trash', 'delete'"));

		wstring renameto;
		parser.AddOption(L"-to",
			1,
			&renameto,
			ArgEncodingFlags_Default,
			(L"Specify new name"));

		int priority = -1;
		parser.AddOption(L"-pri",
			1,
			&priority,
			ArgEncodingFlags_Default,
			(L"Specify priority, 0=High 1=Normal 2=Low 3=Idle"));

		int dummy;
		parser.AddOption(L"-except1", 0, &dummy,
			ArgEncodingFlags::ArgEncodingFlags_Default,
			L"This must be removed");
		parser.AddOption(L"--is-except2", 0, &dummy,
			ArgEncodingFlags::ArgEncodingFlags_Default,
			L"This must be removed 2");

		parser.Parse(_countof(argv) - 1, argv);

		std::thread t1([&] {
			MessageBox(nullptr, parser.getHelpMessage().c_str(), L"FULL", MB_OKCANCEL);
			});
		std::thread t2([&] {
			MessageBox(nullptr, parser.getHelpMessage(L"-except1").c_str(), L"except1", MB_OKCANCEL);
			});
		std::thread t3([&] {
			MessageBox(nullptr, parser.getHelpMessage({ L"-except1",L"--is-except2" }).c_str(), L"except1 and 2", MB_OKCANCEL);
			});
		t1.join();
		t2.join();
		t3.join();
	}
	{

	}
}

TEST(CommandLineParser, KeepDefaultValue)
{
	{
		wchar_t* argv[] = {
		L"exe.exe",
		NULL
		};
		CCommandLineParser parser;
		bool a = false;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_FALSE(a);
	}
	{
		wchar_t* argv[] = {
		L"exe.exe",
		NULL
		};
		CCommandLineParser parser;
		bool a = true;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_TRUE(a);
	} 
	{
		wchar_t* argv[] = {
		L"exe.exe",
		L"-a",
		NULL
		};
		CCommandLineParser parser;
		bool a = false;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_TRUE(a);
	}
	{
		wchar_t* argv[] = {
		L"exe.exe",
		L"-d",
		NULL
		};
		CCommandLineParser parser;
		bool a = false;
		parser.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_FALSE(a);
	}

	{
		bool isHelp = true;
		bool isABC = false;
		bool isXYZ = true;
		wstring path = L"aaa";
		int intval = 123;
		COption opMain(L"", ArgCount::ArgCount_OneToInfinite);
		CCommandLineParser clp;
		clp.AddOptionRange({ wstring(L"-h"), wstring(L"/?") }, 0, &isHelp);
		clp.AddOptionRange({ L"-a",L"-b",L"-c" }, 0, &isABC);
		clp.AddOptionRange({ L"-xyz", L"-bbb" }, 0, &isXYZ);
		clp.AddOption(L"-path", 1, &path);
		clp.AddOption(L"-intval", 1, &intval);
		clp.AddOption(&opMain);
		wchar_t* argv[] = {
			L"exe.exe",
			NULL
		};
		clp.Parse(_countof(argv) - 1, argv);

		EXPECT_TRUE(isHelp);
		EXPECT_FALSE(isABC);
		EXPECT_TRUE(isXYZ);
		EXPECT_STREQ(path.c_str(), L"aaa");
		EXPECT_EQ(intval, 123);
	}
}
