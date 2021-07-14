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


#include <vector>
#include <string>
#include <algorithm>

#include "stdosd/stdosd.h"

#include "GetAllFile.h"

using namespace std;
using namespace Ambiesoft::stdosd;

namespace Ambiesoft {
	namespace {
		bool CompareByFileTime(const WIN32_FIND_DATA& left, const WIN32_FIND_DATA& right)
		{
			return CompareFileTime(&left.ftLastWriteTime, &right.ftLastWriteTime) > 0;
		}
	}

	vector<wstring> GetAllFiles(const wstring& dir, GETALLFILES_SORT sortMethod)
	{
		vector<WIN32_FIND_DATA> files;
		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile(stdCombinePath(dir, L"*.*").c_str(), &wfd);
		if (hFind == INVALID_HANDLE_VALUE)
			return vector<wstring>();
		do
		{
			if (wfd.cFileName[0] == L'.' && wfd.cFileName[1] == 0)
				continue;
			if (wfd.cFileName[0] == L'.' && wfd.cFileName[1] == L'.' && wfd.cFileName[2] == 0)
				continue;
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;
			files.push_back(wfd);
		} while (FindNextFile(hFind, &wfd));
		FindClose(hFind);

		// sort by FileTime
		sort(files.begin(), files.end(), CompareByFileTime);

		vector<wstring> rets;
		for (auto&& w : files)
			rets.push_back(stdCombinePath(dir, w.cFileName));
		return rets;
	}

}