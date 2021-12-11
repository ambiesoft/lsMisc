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
#include <string>
#include "DebugNew.h"
#include "IsFilenamable.h"

using std::wstring;

namespace Ambiesoft {
	wstring GetFileInamableChars()
	{
		return L"/\\:*?\"<>|";
	}
#ifdef __cplusplus_cli
	System::String^ GetFileInamableCharsCLR()
	{
		return gcnew System::String(GetFileInamableChars().c_str());
	}
#endif
	wstring GetRelativePathInamableChars()
	{
		return L":*?\"<>|";
	}
	wstring GetFullPathInamableChars()
	{
		return L"*?\"<>|";
	}
#ifdef __cplusplus_cli
	System::String^ GetFullPathInamableCharsCLR()
	{
		return gcnew System::String(GetFullPathInamableChars().c_str());
	}
#endif

	BOOL makeFileNamable(LPTSTR pch)
	{
		TCHAR* pTemp = pch;
		while( *pch != '\0' )
		{
			TCHAR* pOrgch = pch;
			if( *pch==_T('\\')
				||  *pch==_T(':')
				||  *pch==_T('*')
				||  *pch==_T('?')
				||  *pch==_T('\"')
				||  *pch==_T('<')
				||  *pch==_T('>')
				||  *pch==_T('|') )
			{
				do
				{
					*pOrgch = *(pOrgch+1);
					++pOrgch;
				}
				while( *pOrgch );
			}
			else
			{
				pch = (TCHAR*)_tcsinc(pch);
			}
		}

		pch = pTemp;
		while( *pch != _T('\0') )
		{
			if( *pch == _T('/') )
				*pch = _T('-');

			pch = (TCHAR*)_tcsinc(pch);
		}

		return TRUE;
	}

	BOOL IsFileNamble(LPCTSTR p)
	{
		if(!p || !*p)
			return FALSE;

		return wcspbrk(p, GetFileInamableChars().c_str())==NULL;
	}
	BOOL IsRelativePathNamble(LPCTSTR p)
	{
		if(!p || !*p)
			return FALSE;

		return wcspbrk(p, GetRelativePathInamableChars().c_str())==NULL;
	}
	BOOL IsFullPathNamble(LPCTSTR p)
	{
		if(!p || !*p)
			return FALSE;

		return wcspbrk(p, GetFullPathInamableChars().c_str())==NULL;
	}




} // namespace