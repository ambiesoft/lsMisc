
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
	COption opMain({ L"" }, ArgCount::ArgCount_OneToInfinite);
	CCommandLineParser clp;
	clp.AddOption({ wstring(L"-h"), wstring(L"/?") }, ArgCount::ArgCount_Zero, &isHelp);
	clp.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &isABC);
	clp.AddOption({ L"-xyz", L"-bbb" }, ArgCount::ArgCount_Zero, &isXYZ);
	clp.AddOption({ L"-path" }, ArgCount::ArgCount_One, &path);
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
	COptionA opMain({ "" }, ArgCount::ArgCount_TwoToInfinite);
	CCommandLineParserA clp;
	clp.AddOption({ string("-h"), string("/?") }, ArgCount::ArgCount_Zero, &isHelp);
	clp.AddOption({ "-a","-b","-c" }, ArgCount::ArgCount_Zero, &isABC);
	clp.AddOption({ "-xyz", "-bbb" }, ArgCount::ArgCount_Zero, &isXYZ);
	clp.AddOption({ "-path" }, ArgCount::ArgCount_One, &path);
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
		COption op1({ L"-h", L"-b" }, ArgCount::ArgCount_Zero);
		parser.AddOption(&op1);

		size_t argc = _countof(argv) - 1;
		parser.Parse(argc, argv);
		EXPECT_TRUE(op1.hadOption());
	}
	{
		CCommandLineParser parser;
		COption op1({ L"-xxx", L"-yyy" }, ArgCount::ArgCount_Zero);
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
		parser.AddOption({ L"-b" }, ArgCount::ArgCount_Zero, &b);

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
		parser.AddOption({ L"-b" }, ArgCount::ArgCount_Zero, &b);

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
		parser.AddOption({ L"-i" }, ArgCount::ArgCount_One, &intval);

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
		parser.AddOption({ L"-i" }, ArgCount::ArgCount_One, &intval);

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
	COption page({ L"-page" }, ExactCount::Exact_1);
	parser.AddOption(&page);

	COption url({ L"-url" }, ExactCount::Exact_1);
	parser.AddOption(&url);

	COption hosthwnd({L"-hostwnd" }, ExactCount::Exact_1);
	parser.AddOption(&hosthwnd);

	COption hostpid({ L"-hostpid" }, ExactCount::Exact_1);
	parser.AddOption(&hostpid);

	COption did({ L"-did" }, ExactCount::Exact_1);
	parser.AddOption(&did);

	COption size({ L"-size" }, ExactCount::Exact_1);
	parser.AddOption(&size);

	COption lang({ L"-lang" }, ExactCount::Exact_1);
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
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
		parser.AddOption({ wstring(L"-a"),wstring(L"-b"),wstring(L"-c") }, ArgCount::ArgCount_Zero, &a);
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
		
		COption opMain({ L"" }, ArgCount::ArgCount_ZeroToInfinite);
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
			   "-op",
			   "delete",
			   NULL
		};
		CCommandLineParserA parser(CaseFlags_Default, "Rename or remove a folder", "lsMiscTest");

		COptionA optionDefault({ "" },
			ArgCount::ArgCount_ZeroToInfinite,
			ArgEncodingFlags_Default,
			("specify directory"));
		parser.AddOption(&optionDefault);

		bool bHelp = false;
		parser.AddOption({ "-h", "/h", "/?","--help","-H", "-hh"},
			ArgCount::ArgCount_Zero,
			&bHelp,
			ArgEncodingFlags_Default,
			("show help"));

		bool bVersion = false;
		parser.AddOption({ "-v", "/v", "--version" },
			ArgCount::ArgCount_Zero,
			&bVersion,
			ArgEncodingFlags_Default,
			("show version"));

		string operation;
		parser.AddOption({ "-op" },
			ArgCount::ArgCount_One,
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

		COption optionDefault({ L"" },
			ArgCount::ArgCount_OneToInfinite,
			ArgEncodingFlags_Default,
			(L"specify directory"));
		parser.AddOption(&optionDefault);

		bool bHelp = false;
		parser.AddOption({ L"-h", L"/h" },
			ArgCount::ArgCount_Zero,
			&bHelp,
			ArgEncodingFlags_Default,
			(L"show help"));

		bool bVersion = false;
		parser.AddOption({ L"-v", L"/v" },
			ArgCount::ArgCount_Zero,
			&bVersion,
			ArgEncodingFlags_Default,
			(L"show version"));

		wstring operation;
		parser.AddOption({ L"-op" },
			ArgCount::ArgCount_One,
			&operation,
			ArgEncodingFlags_Default,
			(L"Operation: One of 'rename', 'trash', 'delete'"));

		wstring renameto;
		parser.AddOption({ L"-to" },
			ArgCount::ArgCount_One,
			&renameto,
			ArgEncodingFlags_Default,
			(L"Specify new name"));

		int priority = -1;
		parser.AddOption({ L"-pri" },
			ArgCount::ArgCount_One,
			&priority,
			ArgEncodingFlags_Default,
			(L"Specify priority, 0=High 1=Normal 2=Low 3=Idle"));

		int dummy;
		parser.AddOption({ L"-except1" }, ArgCount::ArgCount_Zero, &dummy,
			ArgEncodingFlags::ArgEncodingFlags_Default,
			L"This must be removed");
		parser.AddOption({ L"--is-except2" }, ArgCount::ArgCount_Zero, &dummy,
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
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
		parser.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &a);
		parser.Parse(_countof(argv) - 1, argv);
		EXPECT_FALSE(a);
	}

	{
		bool isHelp = true;
		bool isABC = false;
		bool isXYZ = true;
		wstring path = L"aaa";
		int intval = 123;
		COption opMain({ L"" }, ArgCount::ArgCount_OneToInfinite);
		CCommandLineParser clp;
		clp.AddOption({ wstring(L"-h"), wstring(L"/?") }, ArgCount::ArgCount_Zero, &isHelp);
		clp.AddOption({ L"-a",L"-b",L"-c" }, ArgCount::ArgCount_Zero, &isABC);
		clp.AddOption({ L"-xyz", L"-bbb" }, ArgCount::ArgCount_Zero, &isXYZ);
		clp.AddOption({ L"-path" }, ArgCount::ArgCount_One, &path);
		clp.AddOption({ L"-intval" }, ArgCount::ArgCount_One, &intval);
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
TEST(CommandLineParser, MultipleArgWithSameOption)
{
	{
		//char* argv[] = {
		//	   "myapp.exe",
		//	   "-s aaa -s bbb -s ccc -s \"x x x\" -d ddd",
		//	   NULL
		//};
		char* arg = "myapp.exe -s aaa -s bbb -s ccc -s \"x x x\" -d ddd";
		CCommandLineParserA parser(CaseFlags_Default, "CommandLineParser, MultipleArgWithSameOption", "lsMiscTest");

		COptionA optionDefault({ "" },
			ArgCount::ArgCount_One,
			ArgEncodingFlags_Default,
			("specify directory"));
		parser.AddOption(&optionDefault);

		bool bHelp = false;
		parser.AddOption({ "-h", "/h", "/?","--help","-H", "-hh" },
			ArgCount::ArgCount_Zero,
			&bHelp,
			ArgEncodingFlags_Default,
			("show help"));

		vector<string> vs;
		parser.AddOption({ "-s" },
			ArgCount::ArgCount_One,
			&vs,
			ArgEncodingFlags_Default,
			("s"));

		parser.Parse(arg);

		EXPECT_EQ(vs.size(), 4u);
		EXPECT_STREQ(vs[0].c_str(), "aaa");
		EXPECT_STREQ(vs[1].c_str(), "bbb");
		EXPECT_STREQ(vs[2].c_str(), "ccc");
		EXPECT_STREQ(vs[3].c_str(), "x x x");
	}
}
TEST(CommandLineParser, WrongInt)
{
	// int
	{
		char* arg = "myapp.exe -n 2a";
		CCommandLineParserA parser;
		parser.setStrict();

		int n = -1;
		parser.AddOption({ "-n" },
			ArgCount::ArgCount_One,
			&n);

		try
		{
			parser.Parse(arg);
			EXPECT_TRUE(false);
		}
		catch (exception& ex)
		{
			EXPECT_STREQ(ex.what(), "2a is not int");
		}
		EXPECT_EQ(n, -1);
	}

	// long long
	{
		// char
		{
			char* arg = "myapp.exe -n 234234a";
			CCommandLineParserA parser;
			parser.setStrict();

			long long n = -1;
			parser.AddOption({ "-n" },
				ArgCount::ArgCount_One,
				&n);

			try
			{
				parser.Parse(arg);
				EXPECT_TRUE(false);
			}
			catch (exception& ex)
			{
				EXPECT_STREQ(ex.what(), (string("234234a is not ") + typeid(long long).name()).c_str());
			}
			EXPECT_EQ(n, -1);
		}
		// wchar
		{
			wchar_t* arg = L"myapp.exe -n 234234a";
			CCommandLineParserW parser;
			parser.setStrict();

			long long n = -1;
			parser.AddOption({ L"-n" },
				ArgCount::ArgCount_One,
				&n);

			try
			{
				parser.Parse(arg);
				EXPECT_TRUE(false);
			}
			catch(illegal_value_type_error<decltype(parser.GetStringType()), long long>& exIVTE)
			{ 
				std::function<std::wstring(const std::type_info&)> type_name_w =
					[](const std::type_info& type) {
					std::string name = type.name();
					return std::wstring(name.begin(), name.end());
					};
				EXPECT_STREQ(
					exIVTE.wwhat().c_str(), 
					(L"234234a is not " + type_name_w(typeid(long long))).c_str()
				);
			}
			catch (exception&)
			{
				EXPECT_TRUE(false);
			}
			EXPECT_EQ(n, -1);
		}
	}
}
TEST(CommandLineParser, BoolAsValue)
{
	// normal
	{
		wchar_t* arg = L"myapp.exe -b 1 -c 2 -d no";
		CCommandLineParserW parser;

		bool b = false;
		parser.AddOption({ L"-b" },
			ArgCount::ArgCount_One,
			&b);
		bool c = false;
		parser.AddOption({ L"-c" },
			ArgCount::ArgCount_One,
			&c);
		bool d = false;
		parser.AddOption({ L"-d" },
			ArgCount::ArgCount_One,
			&d);

		parser.Parse(arg);
		EXPECT_TRUE(b);
		EXPECT_TRUE(c);
		EXPECT_FALSE(d);
	}

	// not bool like command line but not strict
	{
		wchar_t* arg = L"myapp.exe -b 100 -c OK -d 0";
		CCommandLineParserW parser;

		bool b = false;
		parser.AddOption({ L"-b" },
			ArgCount::ArgCount_One,
			&b);
		bool c = false;
		parser.AddOption({ L"-c" },
			ArgCount::ArgCount_One,
			&c);
		bool d = false;
		parser.AddOption({ L"-d" },
			ArgCount::ArgCount_One,
			&d);

		parser.Parse(arg);
		EXPECT_TRUE(b);
		EXPECT_TRUE(c);
		EXPECT_FALSE(d);
	}
	// if argcount = 0
	{
		wchar_t* arg = L"myapp.exe -b 100 -c OK -d 0";
		CCommandLineParserW parser;

		bool b = false;
		parser.AddOption({ L"-b" },
			ArgCount::ArgCount_Zero,
			&b);
		bool c = false;
		parser.AddOption({ L"-c" },
			ArgCount::ArgCount_Zero,
			&c);
		bool d = false;
		parser.AddOption({ L"-d" },
			ArgCount::ArgCount_Zero,
			&d);
		bool e = false;
		parser.AddOption({ L"-e" },
			ArgCount::ArgCount_Zero,
			&e);

		parser.Parse(arg);
		EXPECT_TRUE(b);
		EXPECT_TRUE(c);
		EXPECT_TRUE(d); // because '-d' exists
		EXPECT_FALSE(e);
	}

	// normal (strict)
	{
		wchar_t* arg = L"myapp.exe -b 1 -c on -d no";
		CCommandLineParserW parser;
		parser.setStrict();

		bool b = false;
		parser.AddOption({ L"-b" },
			ArgCount::ArgCount_One,
			&b);
		bool c = false;
		parser.AddOption({ L"-c" },
			ArgCount::ArgCount_One,
			&c);
		bool d = false;
		parser.AddOption({ L"-d" },
			ArgCount::ArgCount_One,
			&d);

		parser.Parse(arg);
		EXPECT_TRUE(b);
		EXPECT_TRUE(c);
		EXPECT_FALSE(d);
	}

	// not bool like command line with strict
	{
		wchar_t* arg = L"myapp.exe -b 100 -c OK -d 0";
		CCommandLineParserW parser;
		parser.setStrict();

		bool b = false;
		parser.AddOption({ L"-b" },
			ArgCount::ArgCount_One,
			&b);
		bool c = false;
		parser.AddOption({ L"-c" },
			ArgCount::ArgCount_One,
			&c);
		bool d = false;
		parser.AddOption({ L"-d" },
			ArgCount::ArgCount_One,
			&d);

		try
		{
			try
			{
				parser.Parse(arg);
				EXPECT_TRUE(false);
			}
			catch (illegal_value_type_error<decltype(parser.GetStringType()), bool>& ex)
			{
				EXPECT_STREQ(ex.what(), "100 is not bool");
				EXPECT_STREQ(ex.wwhat().c_str(), L"100 is not bool");
				throw ex;
			}
		}
		catch (std::exception& ex)
		{
			EXPECT_STREQ(ex.what(), "100 is not bool");
		}
		EXPECT_FALSE(b);
		EXPECT_FALSE(c);
		EXPECT_FALSE(d);
	}

}

TEST(CommandLineParser, EndWithArgOne)
{
	CCommandLineParser parser;
	parser.setStrict();

	bool isDesktop = false;
	parser.AddOption({ L"-desktop" }, ArgCount::ArgCount_Zero, &isDesktop,
		ArgEncodingFlags::ArgEncodingFlags_Default,
		L"Monitor Desktop directory");

	bool isSound = false;
	parser.AddOption({ L"-s",L"--sound" },
		ArgCount::ArgCount_One,
		&isSound,
		ArgEncodingFlags::ArgEncodingFlags_Default,
		L"Plays sound on notification. '0', 'off' or 'no' to disable it.");

	// EXPECT_THROW(parser.Parse(L"aaa.exe -desktop -s"), no_value_error<wstring>);
	try
	{
		parser.Parse(L"aaa.exe -desktop -s");
		EXPECT_TRUE(false);
	}
	catch (no_value_error<wstring>& ex)
	{
		EXPECT_STREQ(ex.wwhat().c_str(), L"option '-s' must have value.");
	}
}

TEST(CommandLineParser, UnknowOptions)
{
	{
		const wchar_t* const argv[] = {
			L"exe.exe",
			L"-h",
			NULL
		};

		CCommandLineParser parser;

		bool bV = false;
		parser.AddOption({ L"-v" }, ArgCount::ArgCount_Zero, &bV);

		size_t argc = _countof(argv) - 1;

		parser.Parse(argc, argv);
		EXPECT_TRUE(parser.hadUnknownOption());
		EXPECT_STREQ(L"-h", parser.getUnknowOptionStrings().c_str());
	}
	{
		const wchar_t* const argv[] = {
			L"exe.exe",
			L"-v",
			L"-wait",
			L"3",
			NULL
		};

		CCommandLineParser parser;

		bool bV = false;
		parser.AddOption({ L"-v" }, ArgCount::ArgCount_Zero, &bV);

		int nWait = 0;
		parser.AddOption({ L"--wait" }, ArgCount::ArgCount_One, &nWait);

		size_t argc = _countof(argv) - 1;

		parser.Parse(argc, argv);
		EXPECT_TRUE(parser.hadUnknownOption());
		EXPECT_STREQ(L"-wait", parser.getFirstUnknowOptionString().c_str());
		EXPECT_STREQ(L"-wait 3", parser.getUnknowOptionStrings().c_str());
	}
}