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







//#include <AFX.h>
#include "tstring.h"
#include <windows.h>
#include <tchar.h>
#include "DebugNew.h"
#include "LCMapCommon.h"

static tstring LCMapCommon(LPCTSTR p, DWORD flag)
{
	int len = lstrlen(p);

	int outlen = (len+1)*2;
	TCHAR* q = (TCHAR*)malloc(outlen*sizeof(TCHAR));
	struct Qfree {
		TCHAR* q_;
		Qfree(TCHAR* q) {q_=q;}
		~Qfree(){free(q_);}
	} qfree(q);

	int resultlen=LCMapString(
				LOCALE_SYSTEM_DEFAULT,
				flag,
				p,
				len,
				q,
				outlen);
	if(resultlen==0)
	{
		return _T("");
	}
	q[resultlen]=0;
	return tstring(q,resultlen);
}

tstring Han2Zen(LPCTSTR p)
{
	return LCMapCommon(p, LCMAP_FULLWIDTH);
}
tstring Zen2Han(LPCTSTR p)
{
	return LCMapCommon(p, LCMAP_HALFWIDTH);
}

tstring Kata2Hira(LPCTSTR p)
{
	return LCMapCommon(p, LCMAP_HIRAGANA);
}
tstring Hira2Kata(LPCTSTR p)
{
	return LCMapCommon(p, LCMAP_KATAKANA);
}

