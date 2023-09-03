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

#include <vcclr.h>
#include <string>

#include "clrString.h"

#ifdef __cplusplus_cli

using namespace System;
using namespace std;

namespace Ambiesoft {
	String^ toCLR(const wstring& s)
	{
		return gcnew String(s.c_str());
	}

	wstring toWstring(String^ s)
	{
		pin_ptr<const wchar_t> pS = PtrToStringChars(s);
		return pS;
	}



	String^ doubleQuote(String^ s)
	{
		if (String::IsNullOrEmpty(s))
			return s;
		if (s[0] == L'"')
			return s;

		bool needquote = false;
		for each(Char c in s)
		{
			if (Char::IsWhiteSpace(c))
			{
				needquote = true;
				break;
			}
		}

		return needquote ? L"\"" + s + L"\"" : s;
	}



} // namespace


#endif  // __cplusplus_cli
