//BSD 2-Clause License
//
//Copyright (c) 2018, Ambiesoft
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//* Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//* Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <string>
#include <memory>

#include "UTF16toUTF8.h"

namespace Ambiesoft {

	char* UrlEncodeEx(const char *pstr, int size = -1);
	char* UrlDecodeEx(const char* penc, int* psize = NULL);


	inline std::string UrlEncodeStd(const char *pstr, int size = -1)
	{
		std::unique_ptr<char> pEncoded(UrlEncodeEx(pstr, size));
		if (!pEncoded)
			return std::string();
		return pEncoded.get();
	}
	inline std::wstring UrlEncodeStd(const wchar_t *pstr, int size = -1)
	{
		if (pstr == NULL || pstr[0] == 0)
			return std::wstring();

		int outsize = 0;
		std::unique_ptr<char[]> p8(UTF16toUTF8Ex(pstr, size, &outsize));
		std::unique_ptr<char[]> pRet8(UrlEncodeEx(p8.get(), outsize));
		return toStdWstringFromUtf8(pRet8.get());
	}

	template<typename RETTYPE> inline RETTYPE UrlDecodeStd(const char* penc);
	template<typename RETTYPE> inline RETTYPE UrlDecodeStd(const wchar_t* penc);

	// string
	template<> inline std::string UrlDecodeStd<std::string>(const char* penc)
	{
		std::unique_ptr<char> p8(UrlDecodeEx(penc));
		return p8.get();
	}
	template<> inline std::string UrlDecodeStd<std::string>(const wchar_t* penc)
	{
		std::unique_ptr<char> p8(UTF16toUTF8Ex(penc));
		std::unique_ptr<char> p8dec(UrlDecodeEx(p8.get()));

		return p8dec.get();
	}
	// wstring
	template<> inline std::wstring UrlDecodeStd<std::wstring>(const char* penc)
	{
		std::unique_ptr<char> p8(UrlDecodeEx(penc));
		return toStdWstringFromUtf8((const char*)p8.get());
	}
	template<> inline std::wstring UrlDecodeStd<std::wstring>(const wchar_t* penc)
	{
		std::unique_ptr<char> p8(UTF16toUTF8Ex(penc));
		std::unique_ptr<char> p8dec(UrlDecodeEx(p8.get()));

		return toStdWstringFromUtf8((const char*)p8dec.get());
	}

} // namespace Ambiesoft
