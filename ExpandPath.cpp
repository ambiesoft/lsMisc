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


#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE


#include <windows.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <sstream>

#include "stdosd/stdosd.h"

#include "ExpandPath.h"

using namespace std;
using namespace Ambiesoft::stdosd;

namespace Ambiesoft{

	template<typename Out>
	static void split(const std::wstring &s, wchar_t delim, Out result) {
		std::wstringstream ss(s);
		std::wstring item;
		while (std::getline(ss, item, delim)) {
			*(result++) = item;
		}
	}

	static std::vector<std::wstring> split(const std::wstring &s, wchar_t delim) {
		std::vector<std::wstring> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	static bool ends_with(std::wstring const & value, std::wstring const & ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}

	bool IsSkipPath(LPCWSTR p)
	{
		if (!p || p[0] == 0)
			return false;

		if (p[0] != L'.')
			return false;

		if (p[1] == 0)
			return true;

		if (p[1] == L'.' && p[2] == 0)
			return true;

		return false;
	}
	static void ExpandDirectory(const wstring& path, bool fileanddir, vector<wstring>& rets, bool appendbs = false)
	{
		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile((path + L"\\*.*").c_str(), &ffd);

		if (INVALID_HANDLE_VALUE == hFind)
		{
			return;
		}

		// List all the files in the directory with some info about them.

		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!IsSkipPath(ffd.cFileName))
				{
					if (appendbs)
						lstrcat(ffd.cFileName, L"\\");
					rets.push_back(stdCombinePath(path, ffd.cFileName));
				}
			}
			else
			{
				if (fileanddir)
				{
					rets.push_back(stdCombinePath(path, ffd.cFileName));
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);
	}
	static bool ExpandPathWork(const wstring& path, vector<wstring>& results)
	{
		if (path.empty())
			return false;

		wstring tmp = stdStringReplace(path, L"/", L"\\");
		wstring orthPath;
		do
		{
			orthPath = stdStringReplace(tmp, L"\\\\", L"\\");
		} while (tmp != orthPath);

		bool endwithbs = ends_with(orthPath, L"\\");

		wstring comp;
		vector<wstring> parts = split(orthPath, L'\\');
		for (size_t i = 0; i < parts.size(); ++i)
		{
			wstring part = parts[i];
			if (part.empty())
			{
				return false; // something wrong
			}

			if (part == L"*")
			{
				if ((i + 1) == parts.size())
				{
					// last
					if (endwithbs)
					{
						// expand directory only
						ExpandDirectory(comp, false, results, endwithbs);
						return true;
					}
					else
					{
						// expand file and directory
						ExpandDirectory(comp, true, results);
						return true;
					}
				}
				else
				{
					// not last ( there is * inbetween )
					vector<wstring> rets;
					ExpandDirectory(comp, false, rets);

					wstring remnent;
					for (size_t j = i + 1; j < parts.size(); ++j)
					{
						remnent += parts[j];
						if ((j + 1) != parts.size())
						{
							// not last
							remnent += L"\\";
						}
					}
					if (endwithbs)
						remnent += L"\\";

					for each(wstring ret in rets)
					{
						wstring joined = stdCombinePath(ret, remnent);
						results.push_back(joined);
					}
					return true;
				}
			}
			else
			{
				comp += part;
				comp += L"\\";
			}
		}
		return true;
	}

	static bool IsNoAsta(const wstring& path)
	{
		std::size_t found = path.find(L'*');
		if (found != wstring::npos)
			return false;

		return true;
	}
	static bool IsNoAsta(const vector<wstring>& paths)
	{
		for each(wstring t in paths)
		{
			std::size_t found = t.find(L'*');
			if (found != wstring::npos)
				return false;
		}
		return true;
	}
	bool ExpandPath(const wstring& path, vector<wstring>& results)
	{
		vector<wstring> rets;
		if (!ExpandPathWork(path, rets))
			return false;

		if (IsNoAsta(rets))
		{
			results = rets;
			return true;
		}

		vector<wstring> rets2;
		for each(wstring t in rets)
		{
			if (IsNoAsta(t))
				rets2.push_back(t);
			else
			{
				vector<wstring> trets;
				if (!ExpandPathWork(t, trets))
					return false;
				rets2.insert(rets2.end(), trets.begin(), trets.end());
			}
		}
		results = rets2;
		return true;
	}
}