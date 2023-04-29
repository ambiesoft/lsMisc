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
#include <memory>
#include <cassert>

#include "DebugNew.h"

#include "GetWindowString.h"

using namespace std;

wstring getWindowTitle(HWND h)
{
	wstring ret;
	int len = GetWindowTextLength(h);
	if(len <= 0)
		return ret;

	LPTSTR p = (LPTSTR)LocalAlloc(LMEM_FIXED, (len+2)*sizeof(TCHAR));
	if(!p)
		return ret;

	struct st_pfreer {
		void* p_;
		st_pfreer(void* p) { p_ = p; }
		~st_pfreer() { 
			LocalFree(p_); 
		}
	} pfreer(p);

	int nr = GetWindowText(h, p, len+1);
	if(nr==0)
		return ret;

	ret = p;
	return ret;
}

wstring getWindowClassName(HWND h)
{
	wstring ret;
	const size_t startSize = 64;
	size_t memlen = startSize;
	unique_ptr<wchar_t[]> p;
	for (; memlen < (startSize * 1024); )
	{
		p.reset(new wchar_t[memlen]);
		int retlen = GetClassName(h, p.get(), memlen);
		if (retlen <= 0)
			return ret;

		if ((retlen + 1) != memlen)
			return p.get();

		// not enough buffer
		memlen *= 2;
	}
	assert(false);
	return ret;
}
