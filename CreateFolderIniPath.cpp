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

// warning C4091: 'typedef ': ignored on left of 'tagGPFIDL_FLAGS' when no variable is declared
#pragma warning(push)
#pragma warning(disable: 4091)
#include <ShlObj.h>
#pragma warning(pop)

#include "GetSpecialFolderPath.h"
#include "CreateFolderIniPath.h"
#include "CreateCompleteDirectory.h"

#include "tstring.h"

#ifndef CSIDL_LOCAL_APPDATA
#define CSIDL_LOCAL_APPDATA 0x001c
#endif

namespace Ambiesoft {

bool GetFolderIniDirEx(HINSTANCE hInst, LPTSTR szFolder, DWORD nfSize,
	LPCTSTR pCreator, LPCTSTR pApp)
{
	TCHAR szT[MAX_PATH];
	if(!GetModuleFileName(hInst, szT, sizeof(szT)/sizeof(TCHAR)))
	{
		throw tstring(_T("Fatal Error"));
	}

	*_tcsrchr(szT, _T('\\'))=0;

	TCHAR szI[MAX_PATH];
	wsprintf(szI, _T("%s\\folder.ini"), szT);

	tstring tmpS;
	int pathtype = GetPrivateProfileInt(_T("Main"),
		_T("pathType"),
		-1,
		szI);
	switch (pathtype)
	{
	case -1:
	case 0:
	{
		if (!GetModuleFileName(NULL, szFolder, nfSize))
		{
			throw tstring(_T("Fatal Error"));
		}
		*_tcsrchr(szFolder, _T('\\')) = 0;
		return true;
	}

	case 1:
	case 2:
	{
		int csidl = pathtype == 1 ? CSIDL_LOCAL_APPDATA : CSIDL_APPDATA;
		if (!GetSpecialFolderPath(NULL, csidl, szFolder))
			return false;

		PathAddBackslash(szFolder);
		lstrcat(szFolder, pCreator);
		lstrcat(szFolder, _T("\\"));
		lstrcat(szFolder, pApp);
		CreateCompleteDirectory(szFolder, NULL);
		return true;
	}
	break;

	case 3:
	{
		if (GetPrivateProfileString(_T("Main"),
			_T("folder"),
			_T(""),
			szFolder,
			nfSize,
			szI))
		{
			return true;
		}
	}
	break;
	}
	
	return false;
}

//void CreateFolderIniPath_obsoletes(HINSTANCE hInst,
//	LPCTSTR pIniFileName, 
//	LPTSTR pOut, 
//	LPCTSTR pCreator,
//	LPCTSTR pApp,
//	LPCTSTR pErrorTemplate)
//{
//	TCHAR szT[MAX_PATH];
//	TCHAR szFolder[MAX_PATH];
//	
//	if(GetFolderIniDir(hInst, 
//		szFolder,
//		(sizeof(szFolder)/sizeof(TCHAR))-sizeof(TCHAR),
//		pCreator,
//		pApp)
//	   && (szFolder[0] != 0 ))
//	{
//		wsprintf(szT, _T("%s\\%s"), szFolder, pIniFileName);
//		if(!PathFileExists(szT))
//		{
//			TCHAR message[MAX_PATH + 128];
//			wsprintf(message, (pErrorTemplate ? pErrorTemplate : _T("%s is not found. Exiting.")), szT);
//			throw tstring(message);
//		}
//		lstrcpy(pOut, szT);
//	}
//	else
//	{
//		if(!GetModuleFileName(NULL, szT, sizeof(szT)/sizeof(TCHAR)))
//		{
//			throw tstring(_T("Fatal Error"));
//		}
//
//		*_tcsrchr(szT, _T('\\'))=0;
//		wsprintf(pOut, _T("%s\\%s"), szT, pIniFileName);
//	}
//}

} // namespace