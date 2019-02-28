﻿#include "stdafx.h"

#include "../stdosd/stdosd.h"
#include "../stdosd/CBool.h"
#include "../stdosd/CNativeValue.h"
#include "../stdwin32/stdwin32.h"

using namespace Ambiesoft::stdosd;
using namespace Ambiesoft::stdwin32;
using namespace std;


TEST(stdosd, stdIsDigitTest)
{
	// "１２３"
    const wchar_t* zenNum = L"１２３";
    // const char16_t* t16 = u"１２３";

    EXPECT_TRUE(stdIsAsciiDigit(L"123"));
	EXPECT_TRUE(stdIsAsciiDigit("0993"));
	EXPECT_TRUE(stdIsAsciiDigit(wstring(L"123")));
	EXPECT_TRUE(stdIsAsciiDigit(string("09")));
	EXPECT_FALSE(stdIsAsciiDigit(zenNum));
	EXPECT_FALSE(stdIsAsciiDigit(wstring(zenNum)));

	EXPECT_TRUE(stdIsTdigit(L"123"));
	EXPECT_TRUE(stdIsTdigit("0993"));
	EXPECT_TRUE(stdIsTdigit(wstring(L"123")));
	EXPECT_TRUE(stdIsTdigit(string("09")));
#if defined(_MSC_VER) || defined(__MINGW32__)
    EXPECT_TRUE(stdIsTdigit(zenNum));
    EXPECT_TRUE(stdIsTdigit(wstring(zenNum)));
#else
    EXPECT_TRUE(!stdIsTdigit(zenNum));
    EXPECT_TRUE(!stdIsTdigit(wstring(zenNum)));
#endif
    // EXPECT_TRUE(stdIsTdigit(t16));
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
	EXPECT_STREQ(stdGetFileNameWitoutExtension((char*)nullptr).c_str(), "");
	EXPECT_STREQ(stdGetFileNameWitoutExtension("").c_str(), "");
	EXPECT_STREQ(stdGetFileNameWitoutExtension(L"").c_str(), L"");
	EXPECT_EQ(stdGetFileNameWitoutExtension(string("")), string(""));
	EXPECT_EQ(stdGetFileNameWitoutExtension(wstring(L"")), wstring(L""));

	EXPECT_STREQ(stdGetFileNameWitoutExtension("aaa.exe").c_str(), "aaa");
	EXPECT_STREQ(stdGetFileNameWitoutExtension(L"aaa.exe").c_str(), L"aaa");
	EXPECT_EQ(stdGetFileNameWitoutExtension(string("aaa.exe")), string("aaa"));
	EXPECT_EQ(stdGetFileNameWitoutExtension(wstring(L"aaa.exe")), wstring(L"aaa"));

	EXPECT_STREQ(stdGetFileNameWitoutExtension("S:/zzz/eee/ff fee/iii/\\/fff.txt").c_str(), "fff");
	EXPECT_STREQ(stdGetFileNameWitoutExtension(L"S:/zzz/eee/ff fee/iii/\\/fff.txt").c_str(), L"fff");
	EXPECT_EQ(stdGetFileNameWitoutExtension(string("S:/zzz/eee/ff fee/iii/\\/fff.txt")), string("fff"));
	EXPECT_EQ(stdGetFileNameWitoutExtension(wstring(L"S:/zzz/eee/ff fee/iii/\\/fff.txt")), wstring(L"fff"));
}

TEST(stdosd, SplitStringA)
{
	vector<string> v;

	v = stdSplitString("", ",");
	EXPECT_EQ(v.size(), 1);
	EXPECT_STREQ(v[0].c_str(), "");

	v = stdSplitString("aaa", ",");
	EXPECT_EQ(v.size(), 1);
	EXPECT_STREQ(v[0].c_str(), "aaa");

	v = stdSplitString("aaa,bbb,ccc", ",");
	EXPECT_EQ(v.size(), 3);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "bbb");
	EXPECT_STREQ(v[2].c_str(), "ccc");

	v = stdSplitString("aaa,,bbb,,ccc", ",");
	EXPECT_EQ(v.size(), 5);
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
	EXPECT_EQ(v.size(), 1);
	EXPECT_STREQ(v[0].c_str(), L"");
	
	v = stdSplitString(L"aaa", L",");
	EXPECT_EQ(v.size(), 1);
	EXPECT_STREQ(v[0].c_str(), L"aaa");

	v = stdSplitString(L"aaa,bbb,ccc", L",");
	EXPECT_EQ(v.size(), 3);
	EXPECT_STREQ(v[0].c_str(), L"aaa");
	EXPECT_STREQ(v[1].c_str(), L"bbb");
	EXPECT_STREQ(v[2].c_str(), L"ccc");
}

TEST(stdosd, SplitStringToLineW)
{
	vector<wstring> v; 
	v = stdSplitStringToLine(L"");
	EXPECT_EQ(v.size(), 1);
	EXPECT_STREQ(v[0].c_str(), L"");

	v = stdSplitStringToLine(L"aaa");
	EXPECT_EQ(v.size(), 1);
	EXPECT_STREQ(v[0].c_str(), L"aaa");

	v = stdSplitStringToLine(L"aaa\nbbb");
	EXPECT_EQ(v.size(), 2);
	EXPECT_STREQ(v[0].c_str(), L"aaa");
	EXPECT_STREQ(v[1].c_str(), L"bbb");

	v = stdSplitStringToLine(L"aaa\r\nbbb");
	EXPECT_EQ(v.size(), 2);
	EXPECT_STREQ(v[0].c_str(), L"aaa");
	EXPECT_STREQ(v[1].c_str(), L"bbb");

	v = stdSplitStringToLine(L"aaa\r\n\nbbb");
	EXPECT_EQ(v.size(), 3);
	EXPECT_STREQ(v[0].c_str(), L"aaa");
	EXPECT_STREQ(v[1].c_str(), L"");
	EXPECT_STREQ(v[2].c_str(), L"bbb");
}

TEST(stdosd, SplitStringToLineA)
{
	vector<string> v;
	v = stdSplitStringToLine("");
	EXPECT_EQ(v.size(), 1);
	EXPECT_STREQ(v[0].c_str(), "");

	v = stdSplitStringToLine("aaa");
	EXPECT_EQ(v.size(), 1);
	EXPECT_STREQ(v[0].c_str(), "aaa");

	v = stdSplitStringToLine("aaa\nbbb");
	EXPECT_EQ(v.size(), 2);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "bbb");

	v = stdSplitStringToLine("aaa\r\nbbb");
	EXPECT_EQ(v.size(), 2);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "bbb");

	v = stdSplitStringToLine("aaa\r\n\nbbb");
	EXPECT_EQ(v.size(), 3);
	EXPECT_STREQ(v[0].c_str(), "aaa");
	EXPECT_STREQ(v[1].c_str(), "");
	EXPECT_STREQ(v[2].c_str(), "bbb");
}

TEST(stdosd, FormatA)
{
    string s;
    s = stdFormat("%d", 100);
    EXPECT_STREQ(s.c_str(), "100");

    s = stdFormat("%s%d%s", "---",100,"---");
    EXPECT_STREQ(s.c_str(), "---100---");
}

TEST(stdosd, FormatW)
{
    wstring s;
    s = stdFormat(L"%d", 100);
    EXPECT_STREQ(s.c_str(), L"100");

    s = stdFormat(L"%ls%d%ls", L"---",100,L"---");
    EXPECT_STREQ(s.c_str(), L"---100---");
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
}

TEST(stdosd, GetParentDirectoryText)
{
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
			L"", true
		) ==
			L"");

	EXPECT_TRUE(
		stdGetParentDirectory(
			L"aaa", true
		) ==
			L"");

	EXPECT_TRUE(
		stdGetParentDirectory(
			L"Z:\\From\\LegacyPrograms\\T\\"
		) ==
			L"Z:\\From\\LegacyPrograms");

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
	EXPECT_TRUE(
		stdCombinePath(
			L"Z:\\From\\LegacyPrograms\\T\\", L"aaa.txt"
		) ==
			L"Z:\\From\\LegacyPrograms\\T\\aaa.txt");
#if _WIN32
	EXPECT_STREQ(stdCombinePath(L"aaa", L"bbb").c_str(), L"aaa\\bbb");
	EXPECT_STREQ(stdCombinePath(L"z:\\aaa", L"bbb").c_str(), L"z:\\aaa\\bbb");
#else
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

TEST(stdosd, stdAddDQIfNecessaryTest)
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
		HFILEITERATOR hI = stdCreateFileIterator(stdGetParentDirectory(stdGetModuleFileNameA()));
		EXPECT_NE(hI, nullptr);
		EXPECT_TRUE(stdCloseFileIterator(hI));
	}

	{
		HFILEITERATOR hI = stdCreateFileIterator(stdGetParentDirectory(stdGetModuleFileNameA()));
		EXPECT_NE(hI, nullptr);

		bool found = false;
		string targetName = stdGetFileName(stdGetModuleFileNameA());
		FileInfo fi;
		while (stdFileNext(hI, &fi))
		{
			if (fi.name() == targetName)
			{
				found = true;
				break;
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
