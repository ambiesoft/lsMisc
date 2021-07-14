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

#include <tchar.h>
#include <windows.h>
//#include <objidl.h>
#include <shlobj.h>
//#include <shlwapi.h>

#include <shobjidl.h>
#include <shlguid.h>

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Ole32.lib")

#include "ResolveLink.h"

namespace Ambiesoft {
namespace stdosd {

bool ResolveLink(LPCTSTR lpszFileIn, HWND hWnd, LPTSTR target)
{
	bool retval = FALSE;
	if( SUCCEEDED(CoInitialize(NULL)) )
	{
		IShellLink* psl = NULL;
		if( SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_SERVER, IID_IShellLink,
			(LPVOID*)&psl)) )
		{
			IPersistFile *ppf = NULL;
			if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf)))
			{
#ifdef UNICODE
				WCHAR* pIn = _wcsdup(lpszFileIn);
#else
				int nAllocLeng = lstrlen(lpszFileIn)+sizeof(WCHAR);
				WCHAR* pIn = new WCHAR[nAllocLeng];
				if( MultiByteToWideChar(CP_ACP, 0, lpszFileIn, -1, pIn, nAllocLeng) == 0 )
				{
					delete[] pIn;
					return false;
				}
#endif
				{
					if (SUCCEEDED(ppf->Load(pIn, STGM_READ)))
					{
						if (SUCCEEDED(psl->Resolve(hWnd, SLR_ANY_MATCH)))
						{
							if( SUCCEEDED(psl->GetPath(target, MAX_PATH, NULL, 0)) )
							{
								retval = true;
							}
						}
					}
				}
				delete [] pIn;
				ppf->Release();
			}
			psl->Release();
		}
		CoUninitialize();
	}
	return retval;
}

}}
