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

#include "stlScopedClear.h"
#include "CompareFileTime.h"

namespace Ambiesoft
{
	int compareFileTime(LPCTSTR src, LPCTSTR dst)
	{
		// check time
		WIN32_FIND_DATA wfdsrc = { 0 };
		WIN32_FIND_DATA wfddst = { 0 };
		HANDLE hSrc = FindFirstFile(src, &wfdsrc);
		if (hSrc == INVALID_HANDLE_VALUE)
			return 0;

		//stlsoft::scoped_handle<HANDLE> srcclose(hSrc, FindClose);
		STLSCOPEDFREE(hSrc, HANDLE, FindClose);

		HANDLE hDst = FindFirstFile(dst, &wfddst);
		if (hDst == INVALID_HANDLE_VALUE)
			return 0;
		//stlsoft::scoped_handle<HANDLE> dstclose(hDst, FindClose);
		STLSCOPEDFREE(hDst, HANDLE, FindClose);

		return CompareFileTime(&wfdsrc.ftLastWriteTime, &wfddst.ftLastWriteTime);
	}
}