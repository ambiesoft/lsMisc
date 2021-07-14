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

#include "GetURLFromClipboard.h"
#include "RemoveLastInValidURLChar.h"

BOOL GetURLFromClipboard(HWND hWnd,LPTSTR pURL, size_t dwSize)
{
	if( pURL==NULL )
		return FALSE;

	if( !IsClipboardFormatAvailable(CF_TEXT) )
		return FALSE;

	if( !OpenClipboard(hWnd) )
		return FALSE;

	LPCTSTR pData = (LPCTSTR)GetClipboardData(CF_TEXT);
	size_t nDataLen = _tcslen(pData);
	if( nDataLen >= dwSize ) {
		CloseClipboard();
		return FALSE;
	}

	LPTSTR pBuf = _tcsdup(pData);
	CloseClipboard();

	pURL[0] = 0;

	if(pBuf==NULL)
		return FALSE;

	LPTSTR pEnd = _tcsstr(pBuf, _T("\n"));
	if(pEnd) {
		*pEnd = 0;
		pEnd--;
		if(*pEnd==_T('\r') && pBuf <= pEnd) {
			*pEnd = 0;
		}
	}

	LPTSTR pHttpStart = _tcsstr(pBuf, _T("http://"));
	if(pHttpStart!=NULL) 
	{
		RemoveLastInValidURLChar(pHttpStart);
		_tcscpy(pURL, pHttpStart);
	} 
	else 
	{
		LPTSTR pFtpStart = _tcsstr(pBuf, _T("ftp://"));
		if(pFtpStart!=NULL) 
		{
			RemoveLastInValidURLChar(pFtpStart);
			_tcscpy(pURL, pFtpStart);
		} 
		else if( !( (_tcslen(pBuf)+sizeof(_T("http://"))) >= dwSize) ) 
		{
			RemoveLastInValidURLChar(pBuf);
			_tcscpy(pURL, _T("http://"));
			_tcscat(pURL, pBuf);
		}
	}
	free(pBuf);
	return TRUE;
}
