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








#include <Windows.h>
#include <shlwapi.h>

#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include "DebugNew.h"
#include "tstring.h"

#include "GetFilesInfo.h"

namespace Ambiesoft {

	BOOL GetFilesInfoW(LPCWSTR pDirectory, FILESINFOW& ret)
	{
		wchar_t szBuff[MAX_PATH];
		if (pDirectory == nullptr || pDirectory[0] == 0)
			return FALSE;
		if (lstrlen(pDirectory) > (MAX_PATH - 6))
			return FALSE;

		lstrcpy(szBuff, pDirectory);
		PathAddBackslash(szBuff);
		lstrcat(szBuff, L"*.*");

		myWIN32_FIND_DATA wfd;
		std::unique_ptr<void, void(*)(void*)> hFind(
			FindFirstFile(szBuff, &wfd),
			[](void* h) 
			{ 
				if(h != INVALID_HANDLE_VALUE)
					FindClose(h); 
			});
		if (hFind.get() == INVALID_HANDLE_VALUE)
			return FALSE;

		ret.directry_ = pDirectory;
		do
		{
			if (wfd.cFileName[0] != L'.')
			{
				ret.wfds_.push_back(wfd);
			}
		} while (FindNextFile(hFind.get(), &wfd));
		
		return TRUE;
	}

}