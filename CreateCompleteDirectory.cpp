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

#ifndef _INC_TCHAR
#pragma message("including tchar.h")
#include <tchar.h>
#endif


#ifndef _INC_SHLWAPI
#pragma message("including shlwlapi")
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#endif


#include "CreateCompleteDirectory.h"

static BOOL GetCountedDir( LPTSTR szBuff, LPCTSTR lpszDir, size_t sizeBuf, int nCount )
{
	TCHAR buf[_MAX_PATH];
	TCHAR* p = buf;
	int nLengOrg = lstrlen(lpszDir);
	if( nLengOrg > sizeof(buf) )
		return FALSE;

	lstrcpy( buf, lpszDir );
	int nFindCount = 0;
	while(*p)
	{
		if (*p == _T('/') || *p == _T('\\'))
		{
			nFindCount ++;
			if( nFindCount == nCount )
			{
				*(p+1) = _T('\0');
				if( (size_t)lstrlen(buf) > sizeBuf )
					return FALSE;
				else
				{
					lstrcpy(szBuff, buf);
					return TRUE;
				}
			}
		}

//        if (IsDBCSLeadByte((BYTE)*p))
  //          p++;

	//	p++;
		p = _tcsinc(p);
	}
	return FALSE;
}


BOOL CreateCompleteDirectory( LPCTSTR lpszDir, LPSECURITY_ATTRIBUTES pSA )
{
	if( PathIsDirectory(lpszDir) )
		return TRUE;

	TCHAR szBuff[_MAX_PATH];
	int i;
	for( i = 0; GetCountedDir( szBuff, lpszDir, sizeof(szBuff), i+1 ); i++ )
	{
		if( !PathIsDirectory(szBuff) )
		{//Dir not exist
			if(!CreateDirectory(szBuff, pSA))
				return FALSE;
		}
	}
	
	return CreateDirectory(lpszDir, pSA);
}