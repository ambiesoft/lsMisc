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
#include <comdef.h>
#include <tchar.h>

#include "AnyCloser.h"
#include "Registory.h"

namespace Ambiesoft {
	BOOL TrxIsRegKeyExists(HKEY hRoot, LPCTSTR pSub)
	{
		BOOL bRet = FALSE;
		HKEY hRet = NULL;
		if (ERROR_SUCCESS == RegOpenKeyEx(hRoot, pSub, 0, KEY_QUERY_VALUE, &hRet))
		{
			bRet = TRUE;
			RegCloseKey(hRet);
		}

		return bRet;
	}

	BOOL TrxRegGetValue(HKEY hRoot, LPCTSTR pSub, LPCTSTR pName, tstring& value)
	{
		HKEY hKey;

		if (ERROR_SUCCESS != RegOpenKeyEx(hRoot,
			pSub,
			0,
			KEY_QUERY_VALUE,
			&hKey))
		{
			return FALSE;
		}

		RegCloser closer(hKey);

		DWORD dwType = REG_SZ;
		BYTE* szT[512] = { 0 };
		DWORD dwSize = sizeof(szT) - 1;
		if (ERROR_SUCCESS != RegQueryValueEx(
			hKey,
			pName,
			0,
			&dwType,
			(BYTE*)szT,
			&dwSize))
		{
			return FALSE;
		}

		if (dwType == REG_SZ)
		{
			value = (LPCTSTR)szT;
		}
		else if (dwType == REG_DWORD)
		{
			DWORD d = *((DWORD*)szT);
			if (d == 0)
				value = _T("0");
			else if (d == 1)
				value = _T("1");
		}
		else
		{
			return FALSE;
		}

		return TRUE;
	}


}