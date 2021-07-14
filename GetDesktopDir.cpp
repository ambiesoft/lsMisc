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








#include <windows.h>
#include <tchar.h>
#include <Shlobj.h>
#include <assert.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

#include "GetDesktopDir.h"

BOOL GetDesktopDir(LPTSTR lpBuf, DWORD dwSize)
{
	if(lpBuf==NULL || dwSize < MAX_PATH)
		return FALSE;

	LPITEMIDLIST lpDeskTop = NULL;
	if( SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP,&lpDeskTop)) )
	{
		assert(lpDeskTop);

		if( !SHGetPathFromIDList(lpDeskTop, lpBuf) )
			lpBuf[0] = _T('\0');
		else
			PathAddBackslash(lpBuf);
	}
	LPMALLOC pMalloc = NULL;
	if( SUCCEEDED(SHGetMalloc(&pMalloc)) )
	{
		pMalloc->Free(lpDeskTop);
		pMalloc->Release();
	}

	if( lpBuf[0] == _T('\0') )
	{
		assert(0);
		lstrcpy( lpBuf, _T("") );
	}

	return TRUE;
}

