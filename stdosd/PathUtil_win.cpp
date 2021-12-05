//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

// #include "stdafx.h"


#include <windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#include <string>
#include <algorithm>

#include <memory>

#include "PathUtil.h"

using namespace std;

namespace Ambiesoft { namespace stdosd {
    static inline const wchar_t* toWc(const char16_t* p) {
        return (const wchar_t*)p;
    }
//    static inline const char16_t* to16c(const wchar_t* p) {
//        return (const char16_t*)p;
//    }

    //server/			should be treated as root
	//server/folder		should not be treated as root
	//server/folder/aaa	should not be treated as root
    bool IsPathRoot(const wchar_t* pPathorg)
    {
		if (!pPathorg || pPathorg[0] == 0)
            return false;

        if (PathIsRelative(pPathorg))
		{
            TCHAR szT[MAX_PATH * 2];
            LPTSTR pT;
            if (GetFullPathName(pPathorg, _countof(szT), szT, &pT) > _countof(szT))
                return false;
            return IsPathRoot(szT);
		}
        LPWSTR pPath = nullptr;
        LPWSTR pPathFree = nullptr;
        bool ret = false;
		do
		{
            if (pPathorg[wcslen(pPathorg) - 1] != L'\\')
			{
				size_t wordsize = (wcslen(pPathorg) + 1 + 1);
				size_t bytesize = wordsize * sizeof(WCHAR);

				pPathFree = pPath = (LPWSTR)malloc(bytesize);
				wcscpy_s(pPath, wordsize, pPathorg);

				PathAddBackslash(pPath);
			}
			else
			{
                pPath = (LPWSTR)pPathorg;
			}


			if (!PathIsUNC(pPath))
			{
				ret = !!PathIsRoot(pPath);
				break;
			}
			if (PathIsUNCServer(pPath))
			{
                ret = true;
				break;
			}

			if (PathIsUNCServerShare(pPath))
			{
                ret = true;
				break;
			}

            ret = false;
        } while (false);

		free(pPathFree);
		return ret;
	}
    bool IsPathRoot(const char16_t* pPathorg)
    {
        return IsPathRoot(toWc(pPathorg));
    }


    bool IsBothEmptyOrNULL(const char16_t* p1, const char16_t* p2)
	{
		if (p1 == NULL || p1[0] == 0)
			return p2 == NULL || p2[0] == 0;
        return false;
	}

    int myPathGetComponentCount(const wchar_t* p)
	{
		LPCTSTR pT = p;
		int i = 0;
        for (; (pT = PathFindNextComponent(pT)) != nullptr; ++i)
		{

		}
		return i;
	}
    int myPathGetComponentCount(const char16_t* p)
    {
        return myPathGetComponentCount(toWc(p));
    }

    bool IsSamePathDepth(const wchar_t* p1org, const wchar_t* p2org)
    {
        return myPathGetComponentCount(p1org) == myPathGetComponentCount(p2org);
    }
    bool IsSamePathDepth(const char16_t* p1org, const char16_t* p2org)
    {
        return IsSamePathDepth(toWc(p1org), toWc(p2org));
    }

    bool IsPathSame(const wchar_t* p1, const wchar_t* p2)
    {
        return lstrcmpi(p1, p2) == 0;
    }
    bool IsPathSame(const char16_t* p1, const char16_t* p2)
    {
        return IsPathSame(toWc(p1), toWc(p2));
    }
    bool IsPathChildIncluded(const wchar_t* p1org, const wchar_t* p2org, std::wstring* pDupPath)
	{
		// http://d.hatena.ne.jp/s-kita/20101206/1291651401#PathCommonPrefix
		TCHAR lpCommonPrefix[MAX_PATH]; lpCommonPrefix[0] = 0;
		int nCommonPrefixCharcters;

        unique_ptr<wchar_t> p1(_tcsdup(p1org));
        unique_ptr<wchar_t> p2(_tcsdup(p2org));

		PathRemoveBackslash(p1.get());
		PathRemoveBackslash(p2.get());

        wstring w1 = p1.get();
        replace(w1.begin(), w1.end(), L'/', L'\\');
        wstring w2 = p2.get();
        replace(w2.begin(), w2.end(), L'/', L'\\');

		TCHAR szT1[MAX_PATH * 2];
		TCHAR szT2[MAX_PATH * 2];
		if (ExpandEnvironmentStrings(w1.c_str(), szT1, _countof(szT1)) > _countof(szT1))
            return false;
		if (ExpandEnvironmentStrings(w2.c_str(), szT2, _countof(szT2)) > _countof(szT2))
            return false;

		w1 = szT1;
		w2 = szT2;
		if (!PathCanonicalize(szT1, w1.c_str()))
            return false;
		if (!PathCanonicalize(szT2, w2.c_str()))
            return false;

		nCommonPrefixCharcters = PathCommonPrefix(szT1, szT2, lpCommonPrefix);
		if (nCommonPrefixCharcters < 1)
		{
            return false;
		}

		if (!IsPathSame(szT1, lpCommonPrefix) && !IsPathSame(szT2, lpCommonPrefix))
		{
            return false;
		}

		if (IsSamePathDepth(szT1, szT2))
		{
            return false;
		}

		if (pDupPath)
			*pDupPath = lpCommonPrefix;

        return true;
	}
    bool IsPathChildIncluded(const char16_t* p1org, const char16_t* p2org, std::u16string* pDupPath)
    {
        return IsPathChildIncluded(toWc(p1org), toWc(p2org), (wstring*)pDupPath);
    }


	static bool HasDupPaths(
		const u16string& left,
		const vector<u16string>& saPaths,
		size_t startindex,
		size_t& hitindex,
		u16string& common)
	{
		for (size_t i = startindex; i < saPaths.size(); ++i)
		{
			u16string dupParent;
			if (IsPathChildIncluded(left.c_str(), saPaths[i].c_str(), &dupParent))
			{
				common = dupParent;
				hitindex = i;
				return true;
			}
		}
		return false;
	}
	static bool HasDupPaths(
		const wstring& left,
		const vector<wstring>& saPaths,
		size_t startindex,
		size_t& hitindex,
		wstring& common)
	{
		for (size_t i = startindex; i < saPaths.size(); ++i)
		{
			wstring dupParent;
			if (IsPathChildIncluded(left.c_str(), saPaths[i].c_str(), &dupParent))
			{
				common = dupParent;
				hitindex = i;
				return true;
			}
		}
		return false;
	}

	bool checkDupPaths(const vector<u16string>& saPaths,
		u16string& left,
		u16string& right,
		u16string& common)
	{
		for (size_t i = 0; i < saPaths.size(); ++i)
		{
			u16string tmpleft = saPaths[i];
			size_t hitindex = 0;
			if (HasDupPaths(tmpleft, saPaths, i + 1, hitindex, common))
			{
				left = tmpleft;
				right = saPaths[hitindex];
				return false;
			}
		}
		return true;
	}
	bool checkDupPaths(const vector<wstring>& saPaths,
		wstring& left,
		wstring& right,
		wstring& common)
	{
		for (size_t i = 0; i < saPaths.size(); ++i)
		{
			wstring tmpleft = saPaths[i];
			size_t hitindex = 0;
			if (HasDupPaths(tmpleft, saPaths, i + 1, hitindex, common))
			{
				left = tmpleft;
				right = saPaths[hitindex];
				return false;
			}
		}
		return true;
	}
}}
