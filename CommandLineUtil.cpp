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

#include <Windows.h>
#include <string>
#include "DebugNew.h"
using namespace std;

static LPCWSTR SkipSpace(LPCWSTR p)
{
	for( ; *p ; ++p)
	{
		if(*p==L'\t' || *p==L' ' || *p==L'\r' || *p==L'\n')
		{
			// skip
		}
		else
		{
			break;
		}
	}
	return p;
}

std::wstring ReplaceFirstArg(LPCWSTR pCommandLine, LPCWSTR pReplace)
{
	if(!pCommandLine || pCommandLine[0]==0)
	{
		return pReplace;
	}
	
	LPCWSTR pFirstEndPos=NULL;
	TCHAR cSep=0;
	
	for(LPCWSTR p = SkipSpace(pCommandLine) ; *p ; ++p)
	{
		if(cSep)
		{
			// in the quote
		}
		else
		{
			pFirstEndPos = p;
			break;
		}

		if(*p==L'"' || *p==L'\'')
		{
			if(cSep==*p)
			{
				// quote ends
				cSep=0;
			}
			else 
			{
				// quote starts
				cSep=*p;
			}
		}
	}
	wstring ret;
	return ret;
}