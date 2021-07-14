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


BOOL stringEndwith(LPCTSTR pString, LPCTSTR pEnd)
{
	if(!pString)
		return FALSE;

	if(!pEnd || pEnd[0]==0)
		return TRUE;

	size_t psize = lstrlen(pString);
	size_t endlen = lstrlen(pEnd);
	if(psize < endlen)
		return FALSE;

	LPCTSTR pS = pString;
	LPCTSTR pSS = pS + psize - endlen;
	while(*pEnd)
	{
		if(*pSS != *pEnd)
			return FALSE;

		++pSS;
		++pEnd;
	}
	return TRUE;
}

BOOL stringEndwithI(LPCTSTR pString, LPCTSTR pEnd)
{
	if(!pString)
		return FALSE;

	if(!pEnd || pEnd[0]==0)
		return TRUE;

	LPTSTR pdup = _tcsdup(pString);
	_tcslwr(pdup);

	LPTSTR endup = _tcsdup(pEnd);
	_tcslwr(endup);

	BOOL ret = stringEndwith(pdup, endup);
	free(pdup);
	free(endup);
	return ret;
}
