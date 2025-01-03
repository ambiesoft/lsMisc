﻿// this should not be included
// because Qt project can not include MFC
// #include "stdafx.h"

#include "../DebugNew.h"

#if defined(_WIN32)
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#include <Windows.h>
#endif

#include <inttypes.h>
#include <limits>
#include <iostream>
#include <fstream>

#include "gtest/gtest.h"

#include "../stdosd/stdosd.h"
#include "../stdosd/CBool.h"
#include "../stdosd/CNativeValue.h"
#include "../stdosd/DetectVM.h"

using namespace Ambiesoft::stdosd;
// using namespace Ambiesoft::stdwin32;
using namespace std;

TEST(stdosd, stdIsDigitTest)
{
	// "１２３"
    const wchar_t* zenNum = L"１２３";
    // const char16_t* t16 = u"１２３";

    EXPECT_TRUE(stdIsAsciiDigitString(L"123"));
	EXPECT_TRUE(stdIsAsciiDigitString("0993"));
	EXPECT_TRUE(stdIsAsciiDigitString(wstring(L"123")));
	EXPECT_TRUE(stdIsAsciiDigitString(string("09")));
	EXPECT_FALSE(stdIsAsciiDigitString(zenNum));
	EXPECT_FALSE(stdIsAsciiDigitString(wstring(zenNum)));

	EXPECT_TRUE(stdIsDigitString(L"123"));
	EXPECT_TRUE(stdIsDigitString("0993"));
	EXPECT_TRUE(stdIsDigitString(wstring(L"123")));
	EXPECT_TRUE(stdIsDigitString(string("09")));
#if defined(_MSC_VER) || defined(__MINGW32__)
    EXPECT_TRUE(stdIsDigitString(zenNum));
    EXPECT_TRUE(stdIsDigitString(wstring(zenNum)));
#else
    EXPECT_TRUE(!stdIsDigitString(zenNum));
    EXPECT_TRUE(!stdIsDigitString(wstring(zenNum)));
#endif
    // EXPECT_TRUE(stdIsDigitString(t16));
}

TEST(stdosd, GetFileName)
{
	EXPECT_STREQ(stdGetFileName("aaa.exe"), "aaa.exe");
	EXPECT_STREQ(stdGetFileName(L"aaa.exe"), L"aaa.exe");
	EXPECT_EQ(stdGetFileName(string("aaa.exe")), string("aaa.exe"));
	EXPECT_EQ(stdGetFileName(wstring(L"aaa.exe")), wstring(L"aaa.exe"));

	EXPECT_STREQ(stdGetFileName("S:/zzz/eee/ff fee/iii/fff.txt"), "fff.txt");
	EXPECT_STREQ(stdGetFileName(L"S:/zzz/eee/ff fee/iii/fff.txt"), L"fff.txt");
	EXPECT_EQ(stdGetFileName(string("S:/zzz/eee/ff fee/iii/fff.txt")), string("fff.txt"));
	EXPECT_EQ(stdGetFileName(wstring(L"S:/zzz/eee/ff fee/iii/fff.txt")), wstring(L"fff.txt"));

#if _WIN32
	EXPECT_STREQ(stdGetFileName("S:/zzz/eee/ff fee/iii/\\fff.txt"), "fff.txt");
	EXPECT_STREQ(stdGetFileName(L"S:/zzz/eee/ff fee/iii/\\fff.txt"), L"fff.txt");
	EXPECT_EQ(stdGetFileName(string("S:/zzz/eee/ff fee/iii/\\fff.txt")), string("fff.txt"));
	EXPECT_EQ(stdGetFileName(wstring(L"S:/zzz/eee/ff fee/iii/\\fff.txt")), wstring(L"fff.txt"));
#endif

	EXPECT_STREQ(stdGetFileName("S:/zzz/eee/ff fee/iii/\\/fff.txt"), "fff.txt");
	EXPECT_STREQ(stdGetFileName(L"S:/zzz/eee/ff fee/iii/\\/fff.txt"), L"fff.txt");
	EXPECT_EQ(stdGetFileName(string("S:/zzz/eee/ff fee/iii/\\/fff.txt")), string("fff.txt"));
	EXPECT_EQ(stdGetFileName(wstring(L"S:/zzz/eee/ff fee/iii/\\/fff.txt")), wstring(L"fff.txt"));
}
TEST(stdosd, GetFileExtension)
{
	EXPECT_STREQ(stdGetFileExtension("aaa.exe"), ".exe");
	EXPECT_STREQ(stdGetFileExtension(L"aaa.exe"), L".exe");
	EXPECT_EQ(stdGetFileExtension(string("aaa.exe")), string(".exe"));
	EXPECT_EQ(stdGetFileExtension(wstring(L"aaa.exe")), wstring(L".exe"));

	EXPECT_STREQ(stdGetFileExtension("S:/zzz/eee/ff fee/iii/aaa.exe"), ".exe");
	EXPECT_STREQ(stdGetFileExtension(L"S:/zzz/eee/ff fee/iii/aaa.exe"), L".exe");
	EXPECT_EQ(stdGetFileExtension(string("S:/zzz/eee/ff fee/iii/aaa.exe")), string(".exe"));
	EXPECT_EQ(stdGetFileExtension(wstring(L"S:/zzz/eee/ff fee/iii/aaa.exe")), wstring(L".exe"));

	EXPECT_EQ(stdGetFileExtension((char*)nullptr), nullptr);
	EXPECT_STREQ(stdGetFileExtension("aaa"), "");
	EXPECT_STREQ(stdGetFileExtension(L"aaa"), L"");
	EXPECT_EQ(stdGetFileExtension(string("aaa")), string(""));
	EXPECT_EQ(stdGetFileExtension(wstring(L"aaa")), wstring(L""));


}

TEST(stdosd, GetFileNameWitoutExtension)
{
	EXPECT_STREQ(stdGetFileNameWithoutExtension((char*)nullptr).c_str(), "");
	EXPECT_STREQ(stdGetFileNameWithoutExtension("").c_str(), "");
	EXPECT_STREQ(stdGetFileNameWithoutExtension(L"").c_str(), L"");
	EXPECT_EQ(stdGetFileNameWithoutExtension(string("")), string(""));
	EXPECT_EQ(stdGetFileNameWithoutExtension(wstring(L"")), wstring(L""));

	EXPECT_STREQ(stdGetFileNameWithoutExtension("aaa.exe").c_str(), "aaa");
	EXPECT_STREQ(stdGetFileNameWithoutExtension(L"aaa.exe").c_str(), L"aaa");
	EXPECT_EQ(stdGetFileNameWithoutExtension(string("aaa.exe")), string("aaa"));
	EXPECT_EQ(stdGetFileNameWithoutExtension(wstring(L"aaa.exe")), wstring(L"aaa"));

	EXPECT_STREQ(stdGetFileNameWithoutExtension("S:/zzz/eee/ff fee/iii/\\/fff.txt").c_str(), "fff");
	EXPECT_STREQ(stdGetFileNameWithoutExtension(L"S:/zzz/eee/ff fee/iii/\\/fff.txt").c_str(), L"fff");
	EXPECT_EQ(stdGetFileNameWithoutExtension(string("S:/zzz/eee/ff fee/iii/\\/fff.txt")), string("fff"));
	EXPECT_EQ(stdGetFileNameWithoutExtension(wstring(L"S:/zzz/eee/ff fee/iii/\\/fff.txt")), wstring(L"fff"));
}

TEST(stdosd, stdRemoveExtension)
{
	EXPECT_STREQ(stdRemoveExtension((char*)nullptr).c_str(), "");
	EXPECT_STREQ(stdRemoveExtension("").c_str(), "");
	EXPECT_STREQ(stdRemoveExtension(L"").c_str(), L"");
	EXPECT_EQ(stdRemoveExtension(string("")), string(""));
	EXPECT_EQ(stdRemoveExtension(wstring(L"")), wstring(L""));

	EXPECT_STREQ(stdRemoveExtension("aaa.exe").c_str(), "aaa");
	EXPECT_STREQ(stdRemoveExtension(L"aaa.exe").c_str(), L"aaa");
	EXPECT_EQ(stdRemoveExtension(string("aaa.exe")), string("aaa"));
	EXPECT_EQ(stdRemoveExtension(wstring(L"aaa.exe")), wstring(L"aaa"));

	EXPECT_STREQ(stdRemoveExtension("S:/zzz/eee/ff fee/iii/\\/fff.txt").c_str(), "S:/zzz/eee/ff fee/iii/\\/fff");
	EXPECT_STREQ(stdRemoveExtension(L"S:/zzz/eee/ff fee/iii/\\/fff.txt").c_str(), L"S:/zzz/eee/ff fee/iii/\\/fff");
	EXPECT_EQ(stdRemoveExtension(string("S:/zzz/eee/ff fee/iii/\\/fff.txt")), string("S:/zzz/eee/ff fee/iii/\\/fff"));
	EXPECT_EQ(stdRemoveExtension(wstring(L"S:/zzz/eee/ff fee/iii/\\/fff.txt")), wstring(L"S:/zzz/eee/ff fee/iii/\\/fff"));
}

TEST(stdosd, SplitStringA)
{
	vector<string> v;

	v = stdSplitString("", ",");
    EXPECT_EQ(v.size(), 1u);
	EXPECT_STREQ(v[0].c_str(), "");

	v = stdSplitString("aaa", ",");
    EXPECT_EQ(v.size(), 1u);
	EXPECT_STREQ(v[0].c_str(), "aaa");

	v = stdSplitString("aaa,bbb,ccc", ",");
    EXPECT_EQ(v.size(), 3u);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "bbb");
	EXPECT_STREQ(v[2].c_str(), "ccc");

	v = stdSplitString("aaa,,bbb,,ccc", ",");
    EXPECT_EQ(v.size(), 5u);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "");
	EXPECT_STREQ(v[2].c_str(), "bbb");
	EXPECT_STREQ(v[3].c_str(), "");
	EXPECT_STREQ(v[4].c_str(), "ccc");
}
TEST(stdosd, SplitStringW)
{
	vector<wstring> v;
	
	v = stdSplitString(L"", L",");
    EXPECT_EQ(v.size(), 1u);
	EXPECT_STREQ(v[0].c_str(), L"");
	
	v = stdSplitString(L"aaa", L",");
    EXPECT_EQ(v.size(), 1u);
	EXPECT_STREQ(v[0].c_str(), L"aaa");

	v = stdSplitString(L"aaa,bbb,ccc", L",");
    EXPECT_EQ(v.size(), 3u);
	EXPECT_STREQ(v[0].c_str(), L"aaa");
	EXPECT_STREQ(v[1].c_str(), L"bbb");
	EXPECT_STREQ(v[2].c_str(), L"ccc");
}

TEST(stdosd, SplitStringToLineW)
{
	vector<wstring> v; 
	v = stdSplitStringToLine(L"");
    EXPECT_EQ(v.size(), 1u);
	EXPECT_STREQ(v[0].c_str(), L"");

	v = stdSplitStringToLine(L"aaa");
    EXPECT_EQ(v.size(), 1u);
	EXPECT_STREQ(v[0].c_str(), L"aaa");

	v = stdSplitStringToLine(L"aaa\nbbb");
    EXPECT_EQ(v.size(), 2u);
	EXPECT_STREQ(v[0].c_str(), L"aaa");
	EXPECT_STREQ(v[1].c_str(), L"bbb");

	v = stdSplitStringToLine(L"aaa\r\nbbb");
    EXPECT_EQ(v.size(), 2u);
	EXPECT_STREQ(v[0].c_str(), L"aaa");
	EXPECT_STREQ(v[1].c_str(), L"bbb");

	v = stdSplitStringToLine(L"aaa\r\n\nbbb");
    EXPECT_EQ(v.size(), 3u);
	EXPECT_STREQ(v[0].c_str(), L"aaa");
	EXPECT_STREQ(v[1].c_str(), L"");
	EXPECT_STREQ(v[2].c_str(), L"bbb");
}

TEST(stdosd, SplitStringToLineA)
{
	vector<string> v;
	v = stdSplitStringToLine("");
    EXPECT_EQ(v.size(), 1u);
	EXPECT_STREQ(v[0].c_str(), "");

	v = stdSplitStringToLine("aaa");
    EXPECT_EQ(v.size(), 1u);
	EXPECT_STREQ(v[0].c_str(), "aaa");

	v = stdSplitStringToLine("aaa\nbbb");
    EXPECT_EQ(v.size(), 2u);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "bbb");

	v = stdSplitStringToLine("aaa\r\nbbb");
    EXPECT_EQ(v.size(), 2u);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "bbb");

	v = stdSplitStringToLine("aaa\r\n\nbbb");
    EXPECT_EQ(v.size(), 3u);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "");
	EXPECT_STREQ(v[2].c_str(), "bbb");
}

static string getLongString()
{
	string ret;
	for (int i = 1; i < 1000; ++i)
		ret += to_string(i);
	return ret;
}

TEST(stdosd, FormatCompileError)
{
	const wstring cw(L"cw");
	const string ca("ca");
#define COMPILETEST true
#ifdef COMPILETEST
	  // stdFormat("%s", L"a");
	  // stdFormat(L"%s", "a");
	  // stdFormat("%s", string("a"));
	  // stdFormat("%s", ca);
	  // stdFormat(L"%s", ca.c_str());
	  // stdFormat(L"%s", wstring(L"a"));
	  // stdFormat(L"%s", cw);
	  // stdFormat("%s", cw.c_str());
#endif
#undef COMPILETEST

	stdFormat("%s", "a");
	stdFormat(L"%s", L"a");
	stdFormat("%s", ca.c_str());
	stdFormat(L"%s", cw.c_str());

	stdFormat("%d", L'a');
	stdFormat(L"%d", L'a');
	stdFormat("%d", 'a');
	stdFormat(L"%d", 'a');
}

TEST(stdosd, FormatA)
{
	string s;
	s = stdFormat("%d", 100);
	EXPECT_STREQ(s.c_str(), "100");

	s = stdFormat("%s%d%s", "---", 100, "---");
	EXPECT_STREQ(s.c_str(), "---100---");

	EXPECT_STREQ(stdFormat(getLongString()).c_str(), getLongString().c_str());
}


TEST(stdosd, FormatW)
{
    wstring s;
    s = stdFormat(L"%d", 100);
    EXPECT_STREQ(s.c_str(), L"100");

    s = stdFormat(L"%ls%d%ls", L"---",100,L"---");
    EXPECT_STREQ(s.c_str(), L"---100---");

	wstring ss = L"abc";
    s = stdFormat(L"%ls", ss.c_str());
	EXPECT_STREQ(s.c_str(), ss.c_str());
}



#if _MSC_VER > 1900

// https://stackoverflow.com/a/41985035
template<std::size_t... S>
string unpack_vector(const std::vector<int>& vec, string fmt, std::index_sequence<S...>) {
	return stdFormat(fmt, vec[S]...);
}

template<std::size_t size>
string unpack_vector(const std::vector<int>& vec, string fmt) {
	if (vec.size() != size)
		EXPECT_TRUE(false);
	return unpack_vector(vec, fmt, std::make_index_sequence<size>());
}


TEST(stdosd, FormatABig)
{
	string s;
	// 100 '%d's
	{
		string ppd100;
		vector<int> d100;
		string result100;
		for (int i = 0; i < 100; ++i)
		{
			ppd100 += "%d";
			d100.push_back(i);
			result100 += to_string(i);
		}
		s = unpack_vector<100>(d100, ppd100);
		EXPECT_STREQ(s.c_str(), result100.c_str());
	}

	// long %s
	s = getLongString();
	EXPECT_STREQ(stdFormat("XXX%sYYY", s.c_str()).c_str(), ("XXX" + s + "YYY").c_str());
}

#endif



TEST(stdosd, int64)
{
#ifndef PRId64
#define PRId64 "lld"
#endif
	{
		string s = stdFormat("%" PRId64, 0LL);
		EXPECT_STREQ(s.c_str(), "0");
	}
	{
#ifdef _WIN32
		using __int64_t = int64_t;
#endif
        string s = stdFormat("%" PRId64, numeric_limits<__int64_t>::max());
        __int64_t u = std::numeric_limits<__int64_t>::max();
		EXPECT_STREQ(s.c_str(), to_string(u).c_str());
	}
}
TEST(stdosd, hasEndingTest)
{
	EXPECT_FALSE(hasEnding((char*)nullptr, (char*)nullptr));
	EXPECT_FALSE(hasEnding((char*)nullptr, ""));
	EXPECT_TRUE(hasEnding("", (char*)nullptr));
	EXPECT_TRUE(hasEnding("", ""));
	EXPECT_FALSE(hasEnding("", "A"));

	EXPECT_TRUE(hasEnding("12345", "5"));
	EXPECT_TRUE(hasEnding("12345", "45"));
	EXPECT_TRUE(hasEnding("12345", "345"));
	EXPECT_TRUE(hasEnding("12345", "2345"));
	EXPECT_TRUE(hasEnding("12345", "12345"));

	EXPECT_TRUE(hasEnding(L"12345", L"5"));
	EXPECT_TRUE(hasEnding(L"12345", L"45"));
	EXPECT_TRUE(hasEnding(L"12345", L"345"));
	EXPECT_TRUE(hasEnding(L"12345", L"2345"));
	EXPECT_TRUE(hasEnding(L"12345", L"12345"));


	EXPECT_FALSE(hasEnding("12345", "1"));
	EXPECT_FALSE(hasEnding("12345", "12"));
	EXPECT_FALSE(hasEnding("12345", "123"));


	EXPECT_FALSE(hasEnding("abcdef", "F"));
	EXPECT_FALSE(hasEnding("abcdef", "eF"));
	EXPECT_FALSE(hasEnding("abcdef", "DEF"));
	EXPECT_FALSE(hasEnding("abcdef", "AbcDef"));

	EXPECT_TRUE(hasEndingI("abcdef", "F"));
	EXPECT_TRUE(hasEndingI("abcdef", "eF"));
	EXPECT_TRUE(hasEndingI("abcdef", "DEF"));
	EXPECT_TRUE(hasEndingI("abcdef", "AbcDef"));


	EXPECT_FALSE(hasEnding(L"abcdef", L"F"));
	EXPECT_FALSE(hasEnding(L"abcdef", L"eF"));
	EXPECT_FALSE(hasEnding(L"abcdef", L"DEF"));
	EXPECT_FALSE(hasEnding(L"abcdef", L"AbcDef"));

	EXPECT_TRUE(hasEndingI(L"abcdef", L"F"));
	EXPECT_TRUE(hasEndingI(L"abcdef", L"eF"));
	EXPECT_TRUE(hasEndingI(L"abcdef", L"DEF"));
	EXPECT_TRUE(hasEndingI(L"abcdef", L"AbcDef"));


	EXPECT_FALSE(hasEnding(string("abcdef"), string("deF")));
	EXPECT_TRUE(hasEndingI(string("abcdef"), string("deF")));

	EXPECT_FALSE(hasEnding(wstring(L"abcdef"), wstring(L"deF")));
	EXPECT_TRUE(hasEndingI(wstring(L"abcdef"), wstring(L"deF")));
}

TEST(stdosd, hasStartingTest)
{
	EXPECT_TRUE(hasStarting("abcdef", "a"));
	EXPECT_TRUE(hasStarting("abcdef", "ab"));
	EXPECT_FALSE(hasStarting("abcdef", "A"));
	EXPECT_FALSE(hasStarting("abcdef", "aB"));

	EXPECT_TRUE(hasStartingI("abcdef", "a"));
	EXPECT_TRUE(hasStartingI("abcdef", "ab"));
	EXPECT_TRUE(hasStartingI("abcdef", "A"));
	EXPECT_TRUE(hasStartingI("abcdef", "aB"));

	EXPECT_TRUE(hasStarting("abcdef", "abcdef"));
	EXPECT_FALSE(hasStarting("abcdef", "abcdefa"));


	EXPECT_TRUE(hasStarting(string("abcdef"), string("a")));
	EXPECT_TRUE(hasStarting(string("abcdef"), string("ab")));
	EXPECT_FALSE(hasStarting(string("abcdef"), string("A")));
	EXPECT_FALSE(hasStarting(string("abcdef"), string("aB")));

	EXPECT_TRUE(hasStartingI(string("abcdef"), string("a")));
	EXPECT_TRUE(hasStartingI(string("abcdef"), string("ab")));
	EXPECT_TRUE(hasStartingI(string("abcdef"), string("A")));
	EXPECT_TRUE(hasStartingI(string("abcdef"), string("aB")));

	EXPECT_TRUE(hasStarting(wstring(L"abcdef"), wstring(L"a")));
	EXPECT_TRUE(hasStarting(wstring(L"abcdef"), wstring(L"ab")));
	EXPECT_FALSE(hasStarting(wstring(L"abcdef"), wstring(L"A")));
	EXPECT_FALSE(hasStarting(wstring(L"abcdef"), wstring(L"aB")));

	EXPECT_TRUE(hasStartingI(wstring(L"abcdef"), wstring(L"a")));
	EXPECT_TRUE(hasStartingI(wstring(L"abcdef"), wstring(L"ab")));
	EXPECT_TRUE(hasStartingI(wstring(L"abcdef"), wstring(L"A")));
	EXPECT_TRUE(hasStartingI(wstring(L"abcdef"), wstring(L"aB")));


	EXPECT_TRUE(hasStarting(string("abcdef"), string("abcdef")));
	EXPECT_FALSE(hasStarting(string("abcdef"), string("abcdefa")));

	EXPECT_TRUE(hasStarting(wstring(L"abcdef"), wstring(L"abcdef")));
	EXPECT_FALSE(hasStarting(wstring(L"abcdef"), wstring(L"abcdefa")));

}

// https://code.i-harness.com/ja/q/c2eb3f
#ifdef _WIN32
#include <io.h> 
#define access    _access_s
#else
#include <unistd.h>
#endif
bool FileExists(const std::string &Filename)
{
	return access(Filename.c_str(), 0) == 0;
}

TEST(stdosd, resolveLinkTest)
{
#ifdef _WIN32
	if (FileExists("C:\\LegacyPrograms\\T\\aaa.rtf"))
	{
		EXPECT_TRUE(L"Z:\\From\\LegacyPrograms\\T\\aaa.rtf" ==
			resolveLink(L"C:\\LegacyPrograms\\T\\aaa.rtf"));

		// not existent
		EXPECT_TRUE(L"Z:\\From\\LegacyPrograms\\T\\aaabbb.rtf" ==
			resolveLink(L"C:\\LegacyPrograms\\T\\aaabbb.rtf"));

		EXPECT_TRUE(L"Z:\\From\\LegacyPrograms\\T\\aaabbb.rtf" ==
			resolveLink(L"Z:\\From\\LegacyPrograms\\T\\aaabbb.rtf"));

		EXPECT_TRUE(L"\\\\Thexp\\Share\\T\\aaa.pdf" ==
			resolveLink(L"\\\\Thexp\\Share\\T\\aaa.pdf"));
	}
#endif
}

TEST(stdosd, GetParentDirectoryText)
{
#ifdef _WIN32
	EXPECT_TRUE(
		stdGetParentDirectory(
			L"Z:\\From\\LegacyPrograms\\T\\aaa.rtf"
		) ==
			L"Z:\\From\\LegacyPrograms\\T");

	EXPECT_TRUE(
		stdGetParentDirectory(
			L"Z:\\From\\LegacyPrograms\\T\\aaa.rtf", true
		) ==
			L"Z:\\From\\LegacyPrograms\\T\\");
    EXPECT_TRUE(
        stdGetParentDirectory(
            L"Z:\\From\\LegacyPrograms\\T\\"
        ) ==
            L"Z:\\From\\LegacyPrograms");
#else
    EXPECT_TRUE(
        stdGetParentDirectory(
            L"/From/LegacyPrograms/T/"
        ) ==
            L"/From/LegacyPrograms");
    EXPECT_TRUE(
        stdGetParentDirectory(
            L"/From/LegacyPrograms/T"
        ) ==
            L"/From/LegacyPrograms");
#endif

	EXPECT_TRUE(
		stdGetParentDirectory(
			L"", true
		) ==
			L"");

	EXPECT_TRUE(
		stdGetParentDirectory(
			L"aaa", true
		) ==
			L"");



}

TEST(stdosd, IsFullPathTest)
{
	EXPECT_TRUE(stdIsFullPath(L"Z:\\From\\LegacyPrograms\\T\\aaa.rtf"));
	EXPECT_TRUE(stdIsFullPath(L"/a/b/c"));
	EXPECT_TRUE(stdIsFullPath(L"\\\\ttt\\aaa"));

	EXPECT_TRUE(!stdIsFullPath<char>((const char*)nullptr,false));
	EXPECT_TRUE(!stdIsFullPath(""));
	EXPECT_TRUE(!stdIsFullPath("."));
	EXPECT_TRUE(!stdIsFullPath("aaa"));

}


TEST(stdosd, CombinePathTest)
{
#if _WIN32
	EXPECT_TRUE(
		stdCombinePath(
			L"Z:\\From\\LegacyPrograms\\T\\", L"aaa.txt"
		) ==
			L"Z:\\From\\LegacyPrograms\\T\\aaa.txt");


	EXPECT_STREQ(stdCombinePath(L"aaa", L"bbb").c_str(), L"aaa\\bbb");
	EXPECT_STREQ(stdCombinePath(L"z:\\aaa", L"bbb").c_str(), L"z:\\aaa\\bbb");
#else
    EXPECT_STREQ(stdCombinePath(L"/aaa/bbb/ccc", L"ddd").c_str(), L"/aaa/bbb/ccc/ddd");
    EXPECT_STREQ(stdCombinePath(L"/aaa/bbb/ccc", L"/ddd").c_str(), L"/ddd");
    EXPECT_STREQ(stdCombinePath(L"aaa", L"bbb").c_str(), L"aaa/bbb");
    EXPECT_STREQ(stdCombinePath(L"z:\\aaa", L"bbb").c_str(), L"z:\\aaa/bbb");
#endif
}

TEST(stdosd, IsSpaceTest)
{
	EXPECT_TRUE(stdIsAsciiSpace(' '));
	EXPECT_TRUE(stdIsAsciiSpace(L' '));
	
	EXPECT_FALSE(stdIsAsciiSpace('e'));
	EXPECT_FALSE(stdIsAsciiSpace(L'a'));
	
#if defined(CHAR16T_AVAILABLE)
	EXPECT_TRUE(stdIsAsciiSpace(u' '));
	EXPECT_FALSE(stdIsAsciiSpace(u'e'));
#endif
}

TEST(stdosd, CBoolTest)
{
    Cbool a;
    EXPECT_FALSE(a);

    a = true;
    EXPECT_TRUE(a);

    a.toggle();
    EXPECT_FALSE(a);

    a = true;
    EXPECT_TRUE(a);
    a = false;
    EXPECT_FALSE(a);

    a=true;
    bool b=a;
    EXPECT_TRUE(b);
    a=false;
    b=a;
    EXPECT_FALSE(b);



    CBoolBase<unsigned int> bi;
    EXPECT_FALSE(bi);

    bi = 1;
    EXPECT_TRUE(bi);

    bi = 3;
    EXPECT_TRUE(bi);

    bi.toggle();
    EXPECT_FALSE(bi);
    bi.toggle();
    EXPECT_TRUE(bi);
    bi.toggle();
    EXPECT_FALSE(bi);

}

TEST(stdosd, CNativeValueTest)
{
	CInt ci;
	EXPECT_TRUE(ci == 0);

	ci = 100;
	EXPECT_TRUE(ci == 100);

	int t = ci;
	EXPECT_TRUE(t == 100);

	ci = ci + 100;
	EXPECT_TRUE(ci == 200);
}

TEST(stdosd, DoubleQuote)
{
	{
		EXPECT_TRUE(!stdIsDQNecessary(""));
		EXPECT_TRUE(!stdIsDQNecessary("a"));
		EXPECT_TRUE(!stdIsDQNecessary("aa"));
		EXPECT_TRUE(stdIsDQNecessary(" a"));
		EXPECT_TRUE(stdIsDQNecessary("a "));
		EXPECT_TRUE(stdIsDQNecessary("a b c"));
		EXPECT_TRUE(!stdIsDQNecessary("\""));
		EXPECT_TRUE(!stdIsDQNecessary("\"\""));
		EXPECT_TRUE(!stdIsDQNecessary("\"abc\""));
	}
	{
		EXPECT_STREQ(stdAddDQIfNecessary("").c_str(), "");
		EXPECT_STREQ(stdAddDQIfNecessary("a").c_str(), "a");
		EXPECT_STREQ(stdAddDQIfNecessary("aa").c_str(), "aa");
		EXPECT_STREQ(stdAddDQIfNecessary(" a").c_str(), "\" a\"");
		EXPECT_STREQ(stdAddDQIfNecessary("a ").c_str(), "\"a \"");
		EXPECT_STREQ(stdAddDQIfNecessary("a b c").c_str(), "\"a b c\"");
		EXPECT_STREQ(stdAddDQIfNecessary("\"").c_str(), "\"");
		EXPECT_STREQ(stdAddDQIfNecessary("\"\"").c_str(), "\"\"");
		EXPECT_STREQ(stdAddDQIfNecessary("\"abc\"").c_str(), "\"abc\"");
	}
}

#ifndef _countof
#define _countof(A) (sizeof(A)/sizeof(A[0]))
#endif
TEST(stdosd, stdStringLowerTest)
{
	{
		char szT[] = "abc";
        EXPECT_STREQ(stdStringLower(szT, _countof(szT) - 1), szT);

		char szT1[] = "ABC";
		EXPECT_STREQ(stdStringLower(szT1, _countof(szT) - 1), "abc");

		string str = " xyz ";
		EXPECT_STREQ(stdStringLower(str).c_str(), str.c_str());

		str = " xyz Z";
		EXPECT_STREQ(stdStringLower(str).c_str(), " xyz z");
	}

	{
		wchar_t szT[] = L"abc";
		EXPECT_STREQ(stdStringLower(szT, _countof(szT) - 1), szT);

		wchar_t szT1[] = L"ABC";
		EXPECT_STREQ(stdStringLower(szT1, _countof(szT) - 1), L"abc");

		wstring str = L" xyz ";
		EXPECT_EQ(stdStringLower(str), str);

		str = L" xyz Z";
		EXPECT_STREQ(stdStringLower(str).c_str(), L" xyz z");
	}
}

TEST(stdosd, stdFileIteratorTest)
{
    {
        HFILEITERATOR hI = stdCreateFileIterator(
			stdGetParentDirectory(stdGetModuleFileName<char>()));
        EXPECT_NE(hI, nullptr);
        EXPECT_TRUE(stdCloseFileIterator(hI));
    }
    {
        HFILEITERATOR hI = stdCreateFileIterator(
			"nonexistantttttttttttttttttttttttttttttttttt");
        EXPECT_EQ(hI, nullptr);
        EXPECT_FALSE(stdCloseFileIterator(hI));
    }

	{
		{
			HFILEITERATOR hI = stdCreateFileIterator(
				stdGetParentDirectory(stdGetModuleFileName<char>()));
			EXPECT_NE(hI, nullptr);

			bool found = false;
			string targetName = stdGetFileName(stdGetModuleFileName<char>());
			FileDirectoryInfo<char> fi;
			while (stdFileNext(hI, &fi))
			{
				if (fi.name() == targetName)
				{
					found = true;
				}
			}
			EXPECT_TRUE(found);
			EXPECT_TRUE(stdCloseFileIterator(hI));
		}

		{
			EXPECT_FALSE(stdCloseFileIterator(nullptr));
		}

		{
			EXPECT_DEATH(stdCloseFileIterator((HFILEITERATOR)1), "");
		}
	}
#ifdef _WIN32
	{
		{
			HFILEITERATOR hI = stdCreateFileIterator(
				stdGetParentDirectory(stdGetModuleFileName()));
			EXPECT_NE(hI, nullptr);

			bool found = false;
			wstring targetName = stdGetFileName(stdGetModuleFileName());
			FileDirectoryInfo<wchar_t> fi;
			while (stdFileNext(hI, &fi))
			{
				if (fi.name() == targetName)
				{
					found = true;
				}
			}
			EXPECT_TRUE(found);
			EXPECT_TRUE(stdCloseFileIterator(hI));
		}

		{
			EXPECT_FALSE(stdCloseFileIterator(nullptr));
		}

		{
			EXPECT_DEATH(stdCloseFileIterator((HFILEITERATOR)1), "");
		}
	}
#endif
}

TEST(stdosd, stdTrimStartTest)
{
	EXPECT_STREQ("aaa", stdTrimStart(string("aaa")).c_str());
	EXPECT_STREQ("aaa ", stdTrimStart(string("aaa ")).c_str());
	EXPECT_STREQ("aaa", stdTrimStart(string(" aaa")).c_str());
	EXPECT_STREQ("aaa ", stdTrimStart(string(" aaa ")).c_str());

	EXPECT_STREQ("aaa aaa      ", stdTrimStart(string("          aaa aaa      ")).c_str());
	EXPECT_STREQ("aaa  aaa      ", stdTrimStart(string("          aaa  aaa      ")).c_str());
	EXPECT_STREQ("aaa\taaa      ", stdTrimStart(string("          aaa\taaa      ")).c_str());
	EXPECT_STREQ("aaa\taaa   \t   ", stdTrimStart(string(" \t         aaa\taaa   \t   ")).c_str());

	EXPECT_STREQ("bbbaaa", stdTrimStart(string("aaabbbaaa"), "a").c_str());
	EXPECT_STREQ("xxxabc", stdTrimStart(string("abcxxxabc"), "abc").c_str());

	//// count
	//size_t count;
	//stdTrimStart(string("aaa"), stdLiterals<char>::WHITESPACE(), &count);
	//EXPECT_EQ(count, 0);

	//stdTrimStart(string(" aaa"), stdLiterals<char>::WHITESPACE(), &count);
	//EXPECT_EQ(count, 1);

	//stdTrimStart(string(" aaa "), stdLiterals<char>::WHITESPACE(), &count);
	//EXPECT_EQ(count, 1);

	//stdTrimStart(string("\t \t\naaa "), stdLiterals<char>::WHITESPACE(), &count);
	//EXPECT_EQ(count, 4);
}
TEST(stdosd, stdTrimEndTest)
{
	EXPECT_STREQ("aaa", stdTrimEnd(string("aaa")).c_str());
	EXPECT_STREQ("aaa", stdTrimEnd(string("aaa ")).c_str());
	EXPECT_STREQ(" aaa", stdTrimEnd(string(" aaa")).c_str());
	EXPECT_STREQ(" aaa", stdTrimEnd(string(" aaa ")).c_str());

	EXPECT_STREQ("          aaa aaa", stdTrimEnd(string("          aaa aaa      ")).c_str());
	EXPECT_STREQ("          aaa  aaa", stdTrimEnd(string("          aaa  aaa      ")).c_str());
	EXPECT_STREQ("          aaa\taaa", stdTrimEnd(string("          aaa\taaa      ")).c_str());
	EXPECT_STREQ(" \t         aaa\taaa", stdTrimEnd(string(" \t         aaa\taaa   \t   ")).c_str());

	EXPECT_STREQ("aaabbb", stdTrimEnd(string("aaabbbaaa"), "a").c_str());
	EXPECT_STREQ("abcxxx", stdTrimEnd(string("abcxxxabc"), "abc").c_str());

	//// count
	//size_t count;
	//stdTrimEnd(string("aaa"), stdLiterals<char>::WHITESPACE(), &count);
	//EXPECT_EQ(count, 0);

	//stdTrimEnd(string("aaa "), stdLiterals<char>::WHITESPACE(), &count);
	//EXPECT_EQ(count, 1);
	//stdTrimEnd(string(" aaa "), stdLiterals<char>::WHITESPACE(), &count);
	//EXPECT_EQ(count, 1);
	//
	//stdTrimEnd(string(" aaa \t\t\n"), stdLiterals<char>::WHITESPACE(), &count);
	//EXPECT_EQ(count, 4);
	//
	//stdTrimEnd(string("aaa"), "a", &count);
	//EXPECT_EQ(count, 3);
}

TEST(stdosd, stdTrimTest)
{
	{
		EXPECT_STREQ("aaa", stdTrim(string("aaa")).c_str());
		EXPECT_STREQ("aaa", stdTrim(string("aaa ")).c_str());
		EXPECT_STREQ("aaa", stdTrim(string(" aaa")).c_str());
		EXPECT_STREQ("aaa", stdTrim(string(" aaa ")).c_str());

		EXPECT_STREQ("aaa aaa", stdTrim(string("          aaa aaa      ")).c_str());
		EXPECT_STREQ("aaa  aaa", stdTrim(string("          aaa  aaa      ")).c_str());
		EXPECT_STREQ("aaa\taaa", stdTrim(string("          aaa\taaa      ")).c_str());
		EXPECT_STREQ("aaa\taaa", stdTrim(string(" \t         aaa\taaa   \t   ")).c_str());

		EXPECT_STREQ("bbb", stdTrim(string("aaabbbaaa"), "a").c_str());
		EXPECT_STREQ("xxx", stdTrim(string("abcxxxabc"), "abc").c_str());
	}
	{
		EXPECT_STREQ(L"aaa", stdTrim(wstring(L"aaa")).c_str());
		EXPECT_STREQ(L"aaa", stdTrim(wstring(L"aaa ")).c_str());
		EXPECT_STREQ(L"aaa", stdTrim(wstring(L" aaa")).c_str());
		EXPECT_STREQ(L"aaa", stdTrim(wstring(L" aaa ")).c_str());

		EXPECT_STREQ(L"aaa aaa", stdTrim(wstring(L"          aaa aaa      ")).c_str());
		EXPECT_STREQ(L"aaa  aaa", stdTrim(wstring(L"          aaa  aaa      ")).c_str());
		EXPECT_STREQ(L"aaa\taaa", stdTrim(wstring(L"          aaa\taaa      ")).c_str());
		EXPECT_STREQ(L"aaa\taaa", stdTrim(wstring(L" \t         aaa\taaa   \t   ")).c_str());

		EXPECT_STREQ(L"bbb", stdTrim(wstring(L"aaabbbaaa"), L"a").c_str());
		EXPECT_STREQ(L"xxx", stdTrim(wstring(L"abcxxxabc"), L"abc").c_str());
	}
}

#if defined(_WIN32)
TEST(stdosd, GetModuleFileNameTest)
{
	char szT[MAX_PATH];
	GetModuleFileNameA(nullptr, szT, _countof(szT));
	string s = stdGetModuleFileName<char>();
	EXPECT_STREQ(szT, s.c_str());

	wchar_t szTW[MAX_PATH];
	GetModuleFileNameW(nullptr, szTW, _countof(szTW));
	wstring ws = stdGetModuleFileName<wchar_t>();
	EXPECT_STREQ(szTW, ws.c_str());
}
TEST(stdosd, stdExpandEnvironmentStringsTest)
{
	EXPECT_STREQ("AAABBBCCCDDDEEEXXX", stdExpandEnvironmentStrings("AAABBBCCCDDDEEEXXX").c_str());

	wstring envkey = L"MYENV";
	wstring envval = L"fjowjefowjefojwefjowej2482384-skfjlsj=jjwerrwfjowjefowjefojwefjowej2482384-skfjlsj=jjwerrwfjowjefowjefojwefjowej2482384-skfjlsj=jjwerrwfjowjefowjefojwefjowej2482384-skfjlsj=jjwerrwfjowjefowjefojwefjowej2482384-skfjlsj=jjwerrwfjowjefowjefojwefjowej2482384-skfjlsj=jjwerrwfjowjefowjefojwefjowej2482384-skfjlsj=jjwerrwfjowjefowjefojwefjowej2482384-skfjlsj=jjwerrwfjowjefowjefojwefjowej2482384-skfjlsj=jjwerrw";
	EXPECT_TRUE(!!SetEnvironmentVariable(envkey.c_str(), envval.c_str()));
	wstring expect = L"111" + envval + L"222";
	EXPECT_EQ(expect, stdExpandEnvironmentStrings(L"111%MYENV%222"));
}
#endif // _WIN32

TEST(stdosd, stdGetComputerNameTest)
{
#ifdef _WIN32
	wstring w = stdGetComputerName();
	EXPECT_FALSE(w.empty());
#endif

	string a = stdGetComputerName<char>();
	EXPECT_FALSE(a.empty());

}


TEST(stdosd, stdXmlEncodeTest)
{
	EXPECT_STREQ(stdXmlEncode("").c_str(), "");
	EXPECT_STREQ(stdXmlEncode("abc").c_str(), "abc");
	EXPECT_STREQ(stdXmlEncode("a&b&c").c_str(), "a&amp;b&amp;c");
	EXPECT_STREQ(stdXmlEncode("<.*>").c_str(), "&lt;.*&gt;");

	EXPECT_STREQ(stdXmlEncode(L"").c_str(), L"");
	EXPECT_STREQ(stdXmlEncode(L"abc").c_str(), L"abc");
	EXPECT_STREQ(stdXmlEncode(L"a&b&c").c_str(), L"a&amp;b&amp;c");
	EXPECT_STREQ(stdXmlEncode(L"<.*>").c_str(), L"&lt;.*&gt;");
}

TEST(stdosd, stdRegexReplace)
{
    function<string(const smatch &)> nonefunc = [](const smatch &) {
		return string();
	};
	EXPECT_STREQ(stdRegexReplace(string(""), regex(), nonefunc).c_str(), "");
	EXPECT_STREQ(stdRegexReplace(string("aaa"), regex(), nonefunc).c_str(), "aaa");
	EXPECT_STREQ(stdRegexReplace(string("aaa"), regex("a"), nonefunc).c_str(), "");

    function<string(const smatch &)> zfunc = [](const smatch &) {
		return string("z");
	};
	EXPECT_STREQ(stdRegexReplace(string(""), regex(), zfunc).c_str(), "");
	EXPECT_STREQ(stdRegexReplace(string("aaa"), regex(), zfunc).c_str(), "aaa");
	EXPECT_STREQ(stdRegexReplace(string("aaa"), regex("a"), zfunc).c_str(), "zzz");


	wstring input = L"aaa[2]bbb[1]ccc";
	int writeIndex = 1;
    function<wstring(const wsmatch &)> kfunc = [&](const wsmatch &) {
		return wstring(L"[") + to_wstring(writeIndex++) + L"]";
	};
	wstring output = stdRegexReplace(input, wregex(L"\\[\\d+\\]"), kfunc);
	EXPECT_STREQ(output.c_str(), L"aaa[1]bbb[2]ccc");
}

TEST(stdosd, stdRemoveFirstLine)
{
	EXPECT_STREQ(stdRemoveFirstLine(string("")).c_str(), "");
	EXPECT_STREQ(stdRemoveFirstLine(string("a")).c_str(), "");
	EXPECT_STREQ(stdRemoveFirstLine(string("a\nb")).c_str(), "b");
	EXPECT_STREQ(stdRemoveFirstLine(string("a\rb\nc")).c_str(), "b\nc");
}

TEST(stdosd, stdGetFirstLine)
{
	EXPECT_STREQ(stdGetFirstLine(string("")).c_str(), "");
	EXPECT_STREQ(stdGetFirstLine(string("a")).c_str(), "a");
	EXPECT_STREQ(stdGetFirstLine(string("abc")).c_str(), "abc");
	EXPECT_STREQ(stdGetFirstLine(string("abc\nxyz")).c_str(), "abc");
	EXPECT_STREQ(stdGetFirstLine(string("abc\r\nxyz")).c_str(), "abc");
	EXPECT_STREQ(stdGetFirstLine(string("\nabc\r\nxyz")).c_str(), "");

	EXPECT_STREQ(stdGetFirstLine(string("\nabc\r\nxyz"), true).c_str(), "abc");
	EXPECT_STREQ(stdGetFirstLine(string("\r    \nabc\r\nxyz"), true).c_str(), "abc");
	EXPECT_STREQ(stdGetFirstLine(string("\r  \t  \nabc\r\nxyz"), true).c_str(), "abc");
}

TEST(stdosd, stdJoinStrings)
{
	{
		vector<string> nullv;
		string out = stdJoinStrings(nullv);
		EXPECT_TRUE(out.empty());
	}
	{
		vector<string> vs{ "aaa","bbb","ccc" };
		string out = stdJoinStrings(vs);
		EXPECT_STREQ(out.c_str(), "'aaa', 'bbb', 'ccc'");
	}
	{
		vector<wstring> vs{ L"aaa",L"bbb",L"ccc" };
		wstring out = stdJoinStrings(vs);
		EXPECT_STREQ(out.c_str(), L"'aaa', 'bbb', 'ccc'");
	}
	{
		vector<wstring> vs{ L"aaa",L"bbb",L"ccc" };
		wstring out = stdJoinStrings(vs, L":", L"(", L")");
		EXPECT_STREQ(out.c_str(), L"(aaa):(bbb):(ccc)");
	}

}

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

//#ifndef _WIN32
//static void GetSystemDirectoryA(char* p, int count)
//{
//    STDOSD_UNUSED(count);
//    strcpy(p, "/somesystem");
//}
//#endif
TEST(stdosd, stdGetFullPathExecutable)
{
#if defined(_MSC_VER) || defined(__MINGW32__)
	{
		string fullnote1 = stdGetFullPathExecutable("notepad.exe");
		char szT[MAX_PATH];
		GetSystemDirectoryA(szT, _countof(szT));
		string fullnote2 = stdCombinePath(szT, "notepad.exe");
		EXPECT_EQ(fullnote1, fullnote2);
	}
#endif
#if defined(_MSC_VER)
	{
		wstring fullnote1 = stdGetFullPathExecutable(L"notepad.exe");
		wchar_t szT[MAX_PATH];
		GetSystemDirectoryW(szT, _countof(szT));
		wstring fullnote2 = stdCombinePath(szT, L"notepad.exe");
		EXPECT_EQ(fullnote1, fullnote2);
	}
#endif
#if defined(__GNUC__) && !defined(__MINGW32__)
    {
        string fullls = stdGetFullPathExecutable("ls");
        EXPECT_TRUE(stdIsFullPath(fullls));
    }
#endif
}

TEST(stdosd, stdUniqueVector)
{
	{
		vector<int> v = { 1,2,3,1 };
		vector<int> vExpect = { 1,2,3 };
		v = stdUniqueVector(v);
		EXPECT_EQ(v, vExpect);
	}
	{
		vector<string> v = { "yyy", "aaa", "yyy", "bbb", "yyy", "bbb" };
		vector<string> vExpect = { "yyy", "aaa", "bbb" };
		v = stdUniqueVector(v);
		EXPECT_EQ(v, vExpect);
	}
}

TEST(stdosd, stdIsSamePath)
{
#ifdef _WIN32
    EXPECT_TRUE(stdIsSamePath(L"", L""));
    EXPECT_TRUE(stdIsSamePath(nullptr, L""));
    EXPECT_TRUE(stdIsSamePath(L"", nullptr));
    EXPECT_FALSE(stdIsSamePath(L"2", nullptr));
    EXPECT_FALSE(stdIsSamePath(L"abcz", L"abc"));
    EXPECT_TRUE(stdIsSamePath(L"abc", L"abc/"));
    EXPECT_TRUE(stdIsSamePath(L"abc\\", L"abc"));

    EXPECT_TRUE(stdIsSamePath(L"./abc", L"abc"));
    EXPECT_TRUE(stdIsSamePath(L"./abc", L"././abc"));
    EXPECT_TRUE(stdIsSamePath(L"./abc", L"././abc/xyz/.."));

    EXPECT_FALSE(stdIsSamePath(L"X:\\aaa\\bbb\\ccc", L"Y:\\aaa\\bbb\\ccc"));
    EXPECT_TRUE(stdIsSamePath(L"X:\\aaa\\bbb\\ccc", L"X:\\aaa\\bbb\\ccc"));
    EXPECT_TRUE(stdIsSamePath(L"X:\\aaa\\bbb\\ccc", L"X:\\aaa\\bbb\\ccc\\ddd\\..\\"));
    EXPECT_FALSE(stdIsSamePath(nullptr, L"1"));


    EXPECT_TRUE(stdIsSamePath(L"abc", L"abc"));
#else
    EXPECT_TRUE(stdIsSamePath(nullptr, nullptr));
#endif

}

TEST(stdosd, stdToCRLFString)
{
	EXPECT_EQ(stdToCRLFString(string("aaa\r\nbbb")), string("aaa\r\nbbb"));
	EXPECT_EQ(stdToCRLFString(string("aaa\rbbb")), string("aaa\r\nbbb"));
	EXPECT_EQ(stdToCRLFString(string("aaa\n\nbbb")), string("aaa\r\n\r\nbbb"));
	EXPECT_EQ(stdToCRLFString(string("aaa\n\r\nbbb")), string("aaa\r\n\r\nbbb"));
}

TEST(stdosd, stdRemoveDoubleQuote)
{
	EXPECT_EQ(stdRemoveDoubleQuote(string("")), string(""));
	EXPECT_EQ(stdRemoveDoubleQuote(string("\"")), string("\""));
	EXPECT_EQ(stdRemoveDoubleQuote(string("\"\"")), string(""));
	EXPECT_EQ(stdRemoveDoubleQuote(string("a")), string("a"));
	EXPECT_EQ(stdRemoveDoubleQuote(string("abc")), string("abc"));
	EXPECT_EQ(stdRemoveDoubleQuote(string("a\"bc")), string("a\"bc"));
	EXPECT_EQ(stdRemoveDoubleQuote(string("abc\"")), string("abc\""));
	EXPECT_EQ(stdRemoveDoubleQuote(string("\"abc")), string("\"abc"));
	
	EXPECT_EQ(stdRemoveDoubleQuote(string("\"abc\"")), string("abc"));
}


TEST(stdosd, stdFileExists)
{
#ifdef _WIN32
    {
        wstring thisFile = stdGetModuleFileName();
        EXPECT_TRUE(stdFileExists(thisFile.c_str()));
        EXPECT_FALSE(stdDirectoryExists(thisFile.c_str()));

        wstring thisFolder = stdGetParentDirectory(thisFile);
        EXPECT_FALSE(stdFileExists(thisFolder.c_str()));
        EXPECT_TRUE(stdDirectoryExists(thisFolder.c_str()));

        wstring nonExistantFile = thisFile + L"fwjolkaj3vaRR";
        EXPECT_FALSE(stdFileExists(nonExistantFile.c_str()));
        EXPECT_FALSE(stdDirectoryExists(nonExistantFile.c_str()));
    }
#endif
    {
        string thisFile = stdGetModuleFileName<char>();
        EXPECT_TRUE(stdFileExists(thisFile.c_str()));
        EXPECT_FALSE(stdDirectoryExists(thisFile.c_str()));

        string thisFolder = stdGetParentDirectory(thisFile);
        EXPECT_FALSE(stdFileExists(thisFolder.c_str()));
        EXPECT_TRUE(stdDirectoryExists(thisFolder.c_str()));

        string nonExistantFile = thisFile + "fwjolkaj3vaRR";
        EXPECT_FALSE(stdFileExists(nonExistantFile.c_str()));
        EXPECT_FALSE(stdDirectoryExists(nonExistantFile.c_str()));
    }
}
TEST(stdosd, stdGetCurrentDirectory)
{
    basic_string<SYSTEM_CHAR_TYPE> curdir = stdGetCurrentDirectory<SYSTEM_CHAR_TYPE>();
    EXPECT_FALSE(curdir.empty());
}
TEST(stdosd, stdGetProgramName)
{
    basic_string<SYSTEM_CHAR_TYPE> prog = stdGetProgramName();
    EXPECT_FALSE(prog.empty());
}

TEST(stdosd, stdToString)
{
#ifdef _WIN32
	EXPECT_STREQ(L"1", stdToString(1).c_str());
#else
	EXPECT_STREQ("1", stdToString(1).c_str());
#endif

	EXPECT_STREQ("0", stdToString<char>(0).c_str());
	EXPECT_STREQ("-1", stdToString<char>(-1).c_str());
	EXPECT_STREQ(L"0", stdToString<wchar_t>(0).c_str());
	EXPECT_STREQ(L"-1", stdToString<wchar_t>(-1).c_str());

	EXPECT_STREQ("true", stdToString<char>(true).c_str());
	EXPECT_STREQ("false", stdToString<char>(!true).c_str());
	EXPECT_STREQ(L"true", stdToString<wchar_t>(true).c_str());
	EXPECT_STREQ(L"false", stdToString<wchar_t>(!true).c_str());
}

TEST(stdosd, stdGetUnittedSize)
{
	int nSign = 0;
    int64_t val = 0;
	EXPECT_FALSE(stdGetUnittedSize((const char*)NULL, 0, &nSign, &val));
	EXPECT_FALSE(stdGetUnittedSize("", 0, &nSign, &val));
#ifdef _WIN32
	EXPECT_FALSE(stdGetUnittedSize(L"", 0, &nSign, &val));
#endif
	EXPECT_TRUE(stdGetUnittedSize("100", -1, &nSign, &val));
	EXPECT_EQ(nSign, 0);
	EXPECT_EQ(val, 100);

	EXPECT_TRUE(stdGetUnittedSize("100k", -1, &nSign, &val));
	EXPECT_EQ(nSign, 0);
	EXPECT_EQ(val, 100 * 1000);

	EXPECT_TRUE(stdGetUnittedSize("-100K", -1, &nSign, &val));
	EXPECT_EQ(nSign, -1);
	EXPECT_EQ(val, -100 * 1024);

#ifdef _WIN32
	EXPECT_TRUE(stdGetUnittedSize(L"-100K", -1, &nSign, &val));
	EXPECT_EQ(nSign, -1);
	EXPECT_EQ(val, -100 * 1024);
#endif
	EXPECT_TRUE(stdGetUnittedSize(string("-100K"), &nSign, &val));
	EXPECT_EQ(nSign, -1);
	EXPECT_EQ(val, -100 * 1024);

#ifdef _WIN32
	EXPECT_TRUE(stdGetUnittedSize(wstring(L"-100K"), &nSign, &val));
	EXPECT_EQ(nSign, -1);
	EXPECT_EQ(val, -100 * 1024);
#endif
}

TEST(stdosd, stdGetenv)
{
#ifdef _WIN32
	char szT[32];
#endif

	EXPECT_TRUE(stdGetenv("PATH").size() != 0);

#ifdef _WIN32
    _putenv("AAA=XXX");
    EXPECT_STREQ(stdGetenv("AAA").c_str(), "XXX");
	GetEnvironmentVariableA("AAA", szT, sizeof(szT));
	EXPECT_STREQ(szT, "XXX");
#endif

#ifdef _WIN32
	SetEnvironmentVariableA("BBB", "YYY");
	GetEnvironmentVariableA("BBB", szT, sizeof(szT));
	EXPECT_STREQ(szT, "YYY");

	EXPECT_STREQ(stdGetenv("BBB").c_str(), "YYY");
	EXPECT_STREQ(stdGetenv(L"BBB").c_str(), L"YYY");

	SetEnvironmentVariableW(L"CCC", L"ZZZ");
	EXPECT_STREQ(stdGetenv("CCC").c_str(), "ZZZ");
	EXPECT_STREQ(stdGetenv(L"CCC").c_str(), L"ZZZ");

	EXPECT_STREQ(stdGetenv("fewfjoweajfoiewjaofwe").c_str(), "");
	EXPECT_STREQ(stdGetenv(L"fewfjoweajfoiewjaofwe").c_str(), L"");
#endif
}

TEST(stdosd, stdIsSubDirectoryTest)
{
	{
#ifdef _WIN32
		EXPECT_TRUE(stdIsSubDirectory(L"C:\\A\\", L"C:\\A\\B"));
		EXPECT_TRUE(stdIsSubDirectory(L"C:\\A\\", L"C:\\A\\B\\"));
		EXPECT_TRUE(stdIsSubDirectory(L"C:\\a\\", L"C:\\A\\B\\"));
		EXPECT_TRUE(stdIsSubDirectory(L"C:\\A\\", L"C:\\A\\b\\"));

		EXPECT_FALSE(stdIsSubDirectory(L"C:\\A\\", L"C:\\A\\"));
		EXPECT_FALSE(stdIsSubDirectory(L"C:\\c\\", L"C:\\A\\B\\"));

		EXPECT_TRUE(stdIsSubDirectory(L"..", L"."));
#endif
	}
}

TEST(stdosd, stdCreateCompleteDirectoryText)
{

	{
		const SYSTEM_CHAR_TYPE* pDir = STDOSD_SYSTEM_CHAR_LITERAL("./mytestdir/aaa/bb/c/d/e/f/g");
		EXPECT_TRUE(stdCreateCompleteDirectory(pDir));
		EXPECT_TRUE(stdDirectoryExists(pDir));
		EXPECT_TRUE(stdRemoveCompleteDirectory(pDir));
	}
//	{
//		const SYSTEM_CHAR_TYPE* pDir = STDOSD_SYSTEM_CHAR_LITERAL("/kowa");
//		EXPECT_TRUE(stdCreateCompleteDirectory(pDir));
//		EXPECT_TRUE(stdDirectoryExists(pDir));
//		EXPECT_TRUE(stdRemoveCompleteDirectory(pDir));
//	}
}
TEST(stdosd, stdDirectoryEmptyTest)
{
	const SYSTEM_CHAR_TYPE* pDir = STDOSD_SYSTEM_CHAR_LITERAL("./mytestdir");
	const SYSTEM_CHAR_TYPE* pFile = STDOSD_SYSTEM_CHAR_LITERAL("file.txt");
	using S = std::basic_string<SYSTEM_CHAR_TYPE>;

	{
		EXPECT_TRUE(stdRemoveCompleteDirectory(pDir));
		S dir = pDir;
		EXPECT_EQ(0, stdMkDir(dir));
		EXPECT_TRUE(stdDirectoryExists(dir));
		EXPECT_TRUE(stdDirectoryEmpty(dir));

		EXPECT_EQ(0, stdRmDir(dir));
	}

	{
		EXPECT_TRUE(stdRemoveCompleteDirectory(pDir));
		S dir = pDir;
		// check
		EXPECT_FALSE(stdDirectoryExists(dir));
		EXPECT_TRUE(stdDirectoryEmpty(dir));
		
		// create file
		EXPECT_EQ(0, stdMkDir(dir));
		S file = stdCombinePath(dir, pFile);
		EXPECT_TRUE(stdWriteAllText(file, (unsigned char*)"aaa", 3));

		// check file creation
		EXPECT_TRUE(stdFileExists(file));

		// check it is not empty
		EXPECT_FALSE(stdDirectoryEmpty(dir));

		// delete file
		EXPECT_EQ(0, stdUnlink(file));

		// check
		EXPECT_FALSE(stdFileExists(file));
		EXPECT_TRUE(stdDirectoryEmpty(dir));

		EXPECT_EQ(0, stdRmDir(dir)); 
	}
}
TEST(stdosd, stdGetFileCountTest)
{
	const SYSTEM_CHAR_TYPE* pFile1 = STDOSD_SYSTEM_CHAR_LITERAL("./mytestdir/file1.txt");
	const SYSTEM_CHAR_TYPE* pFile2 = STDOSD_SYSTEM_CHAR_LITERAL("./mytestdir/dir2/file2.txt");
	const SYSTEM_CHAR_TYPE* pFile3 = STDOSD_SYSTEM_CHAR_LITERAL("./mytestdir/dir2/file3.txt");

	// depth = 1
	{
		EXPECT_TRUE(stdRemoveCompleteDirectory(stdGetParentDirectory(pFile1)));

		EXPECT_TRUE(stdWriteAllText(pFile1, (unsigned char*)"aaa", 3, true));

		// check file creation
		EXPECT_TRUE(stdFileExists(pFile1));
        EXPECT_EQ(1U, stdGetFileCount(stdGetParentDirectory(pFile1)));

		// delete file
		EXPECT_EQ(0, stdUnlink(pFile1));

		// check
        EXPECT_EQ(0U, stdGetFileCount(stdGetParentDirectory(pFile1)));
        EXPECT_EQ(0, stdRmDir(stdGetParentDirectory(pFile1)));
	}
	// depth = 2
	{
		EXPECT_TRUE(stdRemoveCompleteDirectory(stdGetParentDirectory(pFile1)));

		// check
		EXPECT_EQ(0U, stdGetFileCount(pFile1));

		// create file
		EXPECT_TRUE(stdWriteAllText(pFile1, (unsigned char*)"aaa", 3, true));
		EXPECT_TRUE(stdWriteAllText(pFile2, (unsigned char*)"aaa", 3, true));
		EXPECT_TRUE(stdWriteAllText(pFile3, (unsigned char*)"aaa", 3, true));

		// check file creation
		EXPECT_TRUE(stdFileExists(pFile1));
		EXPECT_TRUE(stdFileExists(pFile2));
		EXPECT_TRUE(stdFileExists(pFile3));
		
        EXPECT_EQ(3U, stdGetFileCount(stdGetParentDirectory(pFile1)));

		// delete file
		EXPECT_EQ(0, stdUnlink(pFile1));
		EXPECT_EQ(0, stdUnlink(pFile2));
		EXPECT_EQ(0, stdUnlink(pFile3));
		
		// check
        EXPECT_EQ(0U, stdGetFileCount(stdGetParentDirectory(pFile1)));
		EXPECT_TRUE(stdRemoveCompleteDirectory(stdGetParentDirectory(pFile1)));
	}
}

TEST(stdosd, stdHasVideoFileExtension)
{
	EXPECT_FALSE(stdHasVideoFileExtension("mp4"));
	EXPECT_TRUE(stdHasVideoFileExtension(".mp4"));
	EXPECT_TRUE(stdHasVideoFileExtension("aaa.mp4"));
	EXPECT_FALSE(stdHasVideoFileExtension(".txt"));
	EXPECT_FALSE(stdHasVideoFileExtension("aaa.txt"));
	EXPECT_TRUE(stdHasVideoFileExtension(".\\xxx\\aaa.mp4"));

	EXPECT_FALSE(stdHasVideoFileExtension(L"mp4"));
	EXPECT_TRUE(stdHasVideoFileExtension(L".mp4"));
	EXPECT_TRUE(stdHasVideoFileExtension(L"aaa.mp4"));
	EXPECT_FALSE(stdHasVideoFileExtension(L".txt"));
	EXPECT_FALSE(stdHasVideoFileExtension(L"aaa.txt"));
	EXPECT_TRUE(stdHasVideoFileExtension(L".\\xxx\\aaa.mp4"));
}

TEST(stdosd, stdSplitEnvPath)
{
    {
        const char* p = "C:\\aaa\\bbb\\ccc;S:\\xxx\\yyy\\zzz";
        vector<string> ret = stdSplitEnvPath(p, SplitEnvSeparator::SEPARATOR_SEMICOLON);
        EXPECT_EQ(ret.size(), 2U);
        EXPECT_STREQ(ret[0].c_str(), "C:\\aaa\\bbb\\ccc");
        EXPECT_STREQ(ret[1].c_str(), "S:\\xxx\\yyy\\zzz");
    }

    // double quoted
    {
        const char* p = "C:\\aaa\\bbb\\ccc;\"D:\\a a a\\b b b\\ c c c\";S:\\xxx\\yyy\\zzz";
        vector<string> ret = stdSplitEnvPath(p, SplitEnvSeparator::SEPARATOR_SEMICOLON);
        EXPECT_EQ(ret.size(), 3U);
        EXPECT_STREQ(ret[0].c_str(), "C:\\aaa\\bbb\\ccc");
        EXPECT_STREQ(ret[1].c_str(), "D:\\a a a\\b b b\\ c c c");
        EXPECT_STREQ(ret[2].c_str(), "S:\\xxx\\yyy\\zzz");
    }
    {
        const char* p = ".:/bin:/usr/bin";
        vector<string> ret = stdSplitEnvPath(p, SplitEnvSeparator::SEPARATOR_COLON);
        EXPECT_EQ(ret.size(), 3U);
        EXPECT_STREQ(ret[0].c_str(), ".");
        EXPECT_STREQ(ret[1].c_str(), "/bin");
        EXPECT_STREQ(ret[2].c_str(), "/usr/bin");
    }

#ifdef _WIN32
	if (GetACP() == 932)
	{
		const char* p = "C:\\" "\x82\xA0\x82\xA0" "bbb\\ccc;S:\\xxx\\yyy\\zzz";
		vector<string> ret = stdSplitEnvPath(p);
        EXPECT_EQ(ret.size(), 2U);
		EXPECT_STREQ(ret[0].c_str(), "C:\\" "\x82\xA0\x82\xA0" "bbb\\ccc");
		EXPECT_STREQ(ret[1].c_str(), "S:\\xxx\\yyy\\zzz");
	}
#endif
	{
		const wchar_t* p = L"C:\\aaa\\bbb\\ccc;S:\\xxx\\yyy\\zzz";
		vector<wstring> ret = stdSplitEnvPath(p, SplitEnvSeparator::SEPARATOR_SEMICOLON);
        EXPECT_EQ(ret.size(), 2U);
		EXPECT_STREQ(ret[0].c_str(), L"C:\\aaa\\bbb\\ccc");
		EXPECT_STREQ(ret[1].c_str(), L"S:\\xxx\\yyy\\zzz");
	}
}

TEST(stdosd, DetectVM)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    bool isVMWare = IsInsideVMWare();
    bool isVPC = IsInsideVPC();

    EXPECT_FALSE(isVMWare && isVPC);
#endif
}

TEST(stdosd, stdGetHexString)
{
#ifdef _WIN32
	EXPECT_STREQ(stdGetHexString(0).c_str(), L"0");
	EXPECT_STREQ(stdGetHexString(1).c_str(), L"1");
	EXPECT_STREQ(stdGetHexString(10).c_str(), L"a");
	EXPECT_STREQ(stdGetHexString(0x12345).c_str(), L"12345");
	EXPECT_STREQ(stdGetHexString(0x12345abcdef).c_str(), L"12345abcdef");
#endif

	EXPECT_STREQ(stdGetHexString<char>(0).c_str(), "0");
	EXPECT_STREQ(stdGetHexString<char>(1).c_str(), "1");
	EXPECT_STREQ(stdGetHexString<char>(10).c_str(), "a");
	EXPECT_STREQ(stdGetHexString<char>(0x12345).c_str(), "12345");
	EXPECT_STREQ(stdGetHexString<char>(0x12345abcdef).c_str(), "12345abcdef");
}

TEST(stdosd, stdAddPathSeparator)
{
	string s;
	wstring ws;

	s = stdAddPathSeparator("");
	EXPECT_TRUE(s == "/" || s == "\\");

	s = stdAddPathSeparator("C:\\aaa");
	EXPECT_TRUE(s == "C:\\aaa/" || s == "C:\\aaa\\");

	s = stdAddPathSeparator("C:\\aaa\\");
    EXPECT_TRUE(s == "C:\\aaa/" || s == "C:\\aaa\\" || s == "C:\\aaa\\/");

	ws = L"C:\\aaa";
	ws = stdAddPathSeparator(ws);
	EXPECT_TRUE(ws == L"C:\\aaa/" || ws == L"C:\\aaa\\");
}

TEST(stdosd, stdStartWith)
{
	{
		EXPECT_TRUE(stdStartWith("", string("")));

		string s = "abc123";
		EXPECT_TRUE(stdStartWith(s, string("abc")));
		EXPECT_TRUE(stdStartWith(s, "abc"));
		EXPECT_TRUE(stdStartWith("abc", "abc"));

		EXPECT_FALSE(stdStartWith("", "abc"));
		EXPECT_FALSE(stdStartWith((const char*)nullptr, "abc"));
	}
}

TEST(stdosd, stdEndWith)
{
	{
		EXPECT_TRUE(stdEndWith("", string("")));

		string s = "abc123";
		EXPECT_TRUE(stdEndWith(s, string("123")));
		EXPECT_TRUE(stdEndWith(s, "123"));
		EXPECT_TRUE(stdEndWith("123", "123"));

		EXPECT_FALSE(stdEndWith("", "abc"));
		EXPECT_FALSE(stdEndWith((const char*)nullptr, "abc"));
	}
}




TEST(stdosd, stdGetFileSize)
{
#define STR "test_stdGetFileSize"
	string filename = STR;
	wstring wfilename = STDOSD_WCHARLITERAL(STR);
	{
		ofstream ofs(filename, std::ios::binary);
		ofs << "aaa";
	}
	
	EXPECT_EQ(stdGetFileSize(filename.c_str()), 3);

#ifdef _WIN32
	EXPECT_EQ(stdGetFileSize(wfilename.c_str()), 3);
#endif

#undef STR
}


TEST(stdosd, stdIsFileSameWithMemory)
{
#define STR "test_stdIsFileSameWithMemory"
	string filename = STR;
	wstring wfilename = STDOSD_WCHARLITERAL(STR);
#undef STR
	{
		const char* data = "abc";
		{
			ofstream ofs(filename, std::ios::binary);
			ofs << data;
		}

		EXPECT_TRUE(stdIsFileSameWithMemory(filename, data, stdStringLength(data)));
		EXPECT_FALSE(stdIsFileSameWithMemory(filename, nullptr, 0));
		EXPECT_FALSE(stdIsFileSameWithMemory("notexistantfile", data, stdStringLength(data)));
	}
	{
		string data;
		for (int i = 0; i < 256; ++i)
			data += "d";
		{
			ofstream ofs(filename, std::ios::binary);
			ofs << data;
		}

		EXPECT_TRUE(stdIsFileSameWithMemory(filename, data.c_str(), stdStringLength(data)));
		EXPECT_FALSE(stdIsFileSameWithMemory(filename, nullptr, 0));
		EXPECT_FALSE(stdIsFileSameWithMemory("notexistantfile", data.c_str(), stdStringLength(data)));
	}
	{
		string data;
		for (int i = 0; i < 257; ++i)
			data += "d";
		{
			ofstream ofs(filename, std::ios::binary);
			ofs << data;
		}

		EXPECT_TRUE(stdIsFileSameWithMemory(filename, data.c_str(), stdStringLength(data)));
		EXPECT_FALSE(stdIsFileSameWithMemory(filename, nullptr, 0));
		EXPECT_FALSE(stdIsFileSameWithMemory("notexistantfile", data.c_str(), stdStringLength(data)));
	}
	{
		const char* data = R"(abc
fewwwwwwwwwwwwwwwwwwwwwwwww fjweo fw efjwaoj fow fwejfoiewajf
fjwoej  jwoejfwe
jfojaweofjeowjfoijewaoifjawjf


fjewojafawoijfoiewajfowjeaofweij
fjffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffwejfoewjfoiewior23487320497237423740237402387fuaufeuwafuewaajdfjajfewa:fewa3904832947238974203978fajsv,,c,ajlkfjaenvnvakjerjaw34983725892375043759afhasfhdsahfdsafsad
efwefweafhewa
)";
		{
			ofstream ofs(filename, std::ios::binary);
			ofs << data;
		}

		EXPECT_TRUE(stdIsFileSameWithMemory(filename, data, stdStringLength(data)));
		EXPECT_FALSE(stdIsFileSameWithMemory(filename, nullptr, 0));
		EXPECT_FALSE(stdIsFileSameWithMemory("notexistantfile", data, stdStringLength(data)));
	}
}

TEST(stdosd, stdCountOf)
{
	int ar[10];
	EXPECT_EQ(stdCountOf(ar), 10);

	char cr[] = { 1,2,3 };
	EXPECT_EQ(stdCountOf(cr), 3);
}

TEST(stdosd, stdIsAllEqual)
{
	EXPECT_TRUE(stdIsAllEqual(1, 1));
	EXPECT_TRUE(stdIsAllEqual(1, 1,1,1));
	EXPECT_TRUE(stdIsAllEqual(1, 1, 1, 1, 1));

	EXPECT_FALSE(stdIsAllEqual(1, 1, 1, 1, 0));
	EXPECT_FALSE(stdIsAllEqual(0, 1, 1, 1, 0));
	EXPECT_FALSE(stdIsAllEqual(0, 2));
}

TEST(stdosd, stdGetLocalAppDirectory)
{
    basic_string<SYSTEM_CHAR_TYPE> s;
	
	s = stdGetLocalAppDirectory(
		STDOSD_SYSTEM_CHAR_LITERAL("Ambiesoft"),
		STDOSD_SYSTEM_CHAR_LITERAL("lsMiscTest"),
		false);
    EXPECT_NE(s.length(), 0);

	s = stdGetLocalAppDirectory(
		STDOSD_SYSTEM_CHAR_LITERAL("Ambiesoft"),
		STDOSD_SYSTEM_CHAR_LITERAL("lsMiscTest"),
		true);
	EXPECT_TRUE(stdIsSubDirectory(stdGetHomeDirectory(), s));
}

TEST(stdosd, stdGetHomeDirectory)
{
	EXPECT_TRUE(stdDirectoryExists(stdGetHomeDirectory()));
}

TEST(stdosd, stdIsAscii)
{
	EXPECT_TRUE(stdIsAscii('a'));
	EXPECT_TRUE(stdIsAscii(L'a'));
	EXPECT_TRUE(stdIsAscii(1));
	EXPECT_TRUE(stdIsAscii(' '));

	EXPECT_FALSE(stdIsAscii(-1));
	EXPECT_FALSE(stdIsAscii(128));

	// Must be Compile-error
	// EXPECT_FALSE(stdIsAscii(""));
	// EXPECT_FALSE(stdIsAscii(nullptr));
	// struct ST {} st;
	// EXPECT_FALSE(stdIsAscii(st));
	// EXPECT_FALSE(stdIsAscii(0.0));
	// EXPECT_FALSE(stdIsAscii(0.1));
	// EXPECT_FALSE(stdIsAscii(true));

	EXPECT_TRUE(stdIsAsciiString("123"));
	EXPECT_TRUE(stdIsAsciiString(L"123"));
	EXPECT_TRUE(stdIsAsciiString(L" "));
	EXPECT_TRUE(stdIsAsciiString(L"-"));

	const char* pA = u8"あいうえお";
	const wchar_t* pW = L"あいうえお";
	EXPECT_FALSE(stdIsAsciiString(pA));
	EXPECT_FALSE(stdIsAsciiString(pW));
}


TEST(stdosd, stdAddToPATHEnvString)
{
	const char* dir = "C:\\vjoewjrjkaljfwejrjewkljrklajrerar\\bin";
	{
		string newPATH = stdAddToPATHEnvString(dir);
		EXPECT_TRUE(stdGetenv("PATH").size() < newPATH.size());
	}
	{
		string newPATH = stdAddToPATHEnvString(dir, TO_TOP);
		EXPECT_TRUE(stdGetenv("PATH").size() < newPATH.size());
		EXPECT_STREQ(dir, newPATH.substr(0, stdStringLength(dir)).c_str());
	}
}

TEST(stdosd, stdGetAllProcesses)
{
    vector<STDOSD_PID> pids = stdGetAllProcesses();
	EXPECT_TRUE(!pids.empty());

	pids = stdGetAllProcesses(stdGetModuleFileName());
	EXPECT_TRUE(!pids.empty());
	EXPECT_NE(pids.end(), std::find(pids.begin(), pids.end(), stdGetCurrentProcessId()));

	pids = stdGetAllProcesses(stdGetFileName(stdGetModuleFileName()));
	EXPECT_TRUE(!pids.empty());
	EXPECT_NE(pids.end(), std::find(pids.begin(), pids.end(), stdGetCurrentProcessId()));
}

TEST(stdosd, stdSuspendProcess)
{
	// stdSuspendProcess(stdGetCurrentProcessId());
	EXPECT_FALSE(stdSuspendProcess(4));

	// stdSuspendProcess(23696);
	// stdResumeProcess(23696);
}
