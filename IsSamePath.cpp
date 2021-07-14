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


#include <windows.h>

#include "IsSamePath.h"

static BOOL isnullpath(LPCTSTR p)
{
	return p==NULL || *p==0;
}

static LPCTSTR getLongPath(LPCTSTR p)
{
	DWORD size = GetLongPathName(p, NULL, 0);
	if(size==0)
		return NULL;

	LPTSTR pBuff = (LPTSTR)malloc( (1+size) *sizeof(TCHAR));
	DWORD retsize = GetLongPathName(p, pBuff, 1+size);
	if(size >= retsize)
		return pBuff;

	free((void*)pBuff);
	return NULL;
}

BOOL IsSamePath(LPCTSTR p1, LPCTSTR p2)
{
	if(!isnullpath(p1) && !isnullpath(p2))
	{
		LPCTSTR p1L = getLongPath(p1);
		LPCTSTR p2L = getLongPath(p2);

		BOOL ret = lstrcmpi(p1L,p2L)==0;
		free((void*)p1L);
		free((void*)p2L);

		return ret;
	}
	return FALSE;
}