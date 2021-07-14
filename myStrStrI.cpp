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
#include <string.h>

#include "myStrStrI.h"

static TCHAR myToLowSimple(TCHAR t)
{
	if ( TEXT('A') <= t && t <= TEXT('Z') )
		return t - TEXT('A') + TEXT('a');

	return t;
}


static LPTSTR myStrChrI(
	LPCTSTR lpStart,
	TCHAR wMatch
)
{
	if ( lpStart==NULL || *lpStart == 0 )
		return NULL;

	const size_t length = _tcslen(lpStart);
	for ( UINT i=0 ; i < length; ++i )
	{
		TCHAR t = myToLowSimple(lpStart[i]);
		TCHAR w = myToLowSimple(wMatch);

		if ( t == w )
			return (LPTSTR)&lpStart[i];
	}
	return NULL;
}

LPTSTR myStrStrI(
	LPCTSTR lpFirst,
	LPCTSTR lpSrch
)
{
	if ( lpFirst==NULL || *lpFirst==0 || lpSrch==NULL || *lpSrch==0 )
	{
		return (LPTSTR)lpFirst;
	}

	const size_t length = _tcslen(lpSrch);

	if ( length > _tcslen(lpFirst) )
		return NULL;

	for( LPCTSTR p = lpFirst; NULL != (p = myStrChrI(p, lpSrch[0])); p++ )
	{
		if ( !_tcsncicmp( p, lpSrch, length ) )
		{
			return (LPTSTR)p;
		}
	}

	return NULL;
}
