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

#include "tstring.h"

#include "WritePrivateProfileWString.h"

#ifdef min
#undef min
#endif

BOOL WritePrivateProfileWString(const TCHAR *lpszSection, const TCHAR *lpszKey, const TCHAR *lpszIn, const TCHAR *lpszFile)
{
	if(lpszIn==NULL)
	{
		return WritePrivateProfileString(lpszSection, lpszKey, NULL, lpszFile);
	}
	
	char szBuff[4096] = {0};
	size_t cpylen = std::min(sizeof(szBuff), (lstrlen(lpszIn)+1)*sizeof(TCHAR));
	memcpy(szBuff, lpszIn, cpylen);
	szBuff[ sizeof(szBuff)-1 ] = 0;
	
	return WritePrivateProfileStruct(lpszSection,
		lpszKey,
		(LPVOID)szBuff,
		sizeof(szBuff),
		lpszFile);
}


BOOL GetPrivateProfileWString(const TCHAR *lpszSection, const TCHAR *lpszKey, const TCHAR *lpszDefault, tstring& tout, const TCHAR *lpszFile)
{
	if(lpszDefault==NULL)
	{
		tout = _T("");
	}
	else
	{
		tout = lpszDefault;
	}

	char szBuff[4096];
	szBuff[0]=0;
	
	if(!GetPrivateProfileStruct(lpszSection,
		lpszKey,
		(LPVOID)szBuff,
		sizeof(szBuff),
		lpszFile))
	{
		DWORD ddd = GetLastError();
		return FALSE;
	}

	tout = (TCHAR*)szBuff;
	return TRUE;
}