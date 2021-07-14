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



#ifndef _WINDOWS_
#pragma message("including windows.h")
#include <windows.h>
#endif

#include "tstring.h"

#include <malloc.h>
#include <shlwapi.h>
#include <tchar.h>

#include "GetDirFromPath.h"
#pragma comment(lib,"shlwapi.lib")


tstring GetDirFromPath(LPCTSTR pszPathName)
{
	LPTSTR p = (LPTSTR)_alloca( (lstrlen(pszPathName) + 1) * sizeof(TCHAR));
	lstrcpy(p, pszPathName);
	DWORD dwAttributes = ::GetFileAttributes(p);
	if(dwAttributes != 0xFFFFFFFF && (dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
	{// this is already dir.
		PathAddBackslash(p);
		return p;
	}

	LPTSTR pSep = _tcsrchr(p, _T('\\'));
	if( pSep==NULL )
	{// illegal path
		pSep = _tcsrchr(p, _T('/'));
		if(pSep==NULL)
			return _T("");
	}
	
	*(pSep+1) = 0;
	return p;
}
