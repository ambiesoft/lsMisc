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


// #include "StdAfx.h"
#include <regex>
#include <cassert>

#include "stdosd.h"

namespace Ambiesoft {
	namespace stdosd {
		using namespace std;

		namespace {
			std::wstring stdGetFullPathName(const wchar_t* pPath)
			{
				// not implemented
				assert(false);
				return std::wstring();
			}

			std::vector<std::wstring> splitdir(const std::wstring& input, const std::wstring& regex) {
				// passing -1 as the submatch index parameter performs splitting
				std::wregex re(regex);
				std::wsregex_token_iterator
					first{ input.begin(), input.end(), re, -1 },
					last;
				return{ first, last };
			}

			wstring resolveSingleDir(const wstring& input)
			{
				// not implemented
				assert(false);
				return std::wstring();
			}
		}
//		std::wstring stdGetFullPathName(const std::wstring& ws)
//		{
//			return stdGetFullPathName(ws.c_str());
//		}
		std::wstring resolveLink(const std::wstring& instring)
		{
			// not implemented
			assert(false);
			return std::wstring();
		}

		size_t stdGetCurrentDirectoryImpl(char* p, size_t size)
		{
			// not implemented
		}
		size_t stdGetCurrentDirectoryImpl(wchar_t* p, size_t size)
		{
			// not implemented
		}

		wstring StdGetDesktopDirectory()
		{
			return L"";
		}

		bool stdIsFileSystemCaseSensitive()
		{
			return true;
		}

	}
}
