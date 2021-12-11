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
#include "DebugNew.h"
#include "ReplaceFileHelper.h"

typedef BOOL (WINAPI *fnReplaceFile)(
									 LPCTSTR lpReplacedFileName,
									 LPCTSTR lpReplacementFileName,
									 LPCTSTR lpBackupFileName,
									 DWORD   dwReplaceFlags,
									 LPVOID  lpExclude,
									 LPVOID  lpReserved
									 );

#ifdef UNICODE
#define AW(name) name "W"
#else
#define AW(name) name "A"
#endif

BOOL ReplaceFileHelper(
					   LPCTSTR lpReplacedFileName,
					   LPCTSTR lpReplacementFileName,
					   LPCTSTR lpBackupFileName,
					   DWORD   dwReplaceFlags,
					   DWORD* pdwLastError)
{
	BOOL bRet = FALSE;
	BOOL bProcessed = FALSE;
	BOOL bReplacedExists = (GetFileAttributes(lpReplacedFileName) != 0xFFFFFFFF);
	if(bReplacedExists)
	{
		HMODULE h = LoadLibrary(_T("Kernel32.dll"));
		if(h)
		{
			LPCSTR pFuncName = AW("ReplaceFile");
			fnReplaceFile func = (fnReplaceFile)GetProcAddress(h,pFuncName);
			if(func)
			{
				bRet = func(
					lpReplacedFileName,
					lpReplacementFileName,
					lpBackupFileName,
					dwReplaceFlags,
					NULL,NULL);
				if(!bRet && pdwLastError)
					*pdwLastError = GetLastError();
				bProcessed = TRUE;
			}
			FreeLibrary(h);
		}
	}

	if(bProcessed)
		return bRet;

	if(bReplacedExists && lpBackupFileName && *lpBackupFileName)
	{
		DeleteFile(lpBackupFileName);
		if(!MoveFile(lpReplacedFileName, lpBackupFileName))
		{
			if(pdwLastError)
				*pdwLastError = GetLastError();
			return FALSE;
		}
	}

	if(!MoveFile(lpReplacementFileName, lpReplacedFileName))
	{
		if(pdwLastError)
			*pdwLastError = GetLastError();
		return FALSE;
	}

	if(pdwLastError)
		*pdwLastError = NO_ERROR;
	return TRUE;
}
