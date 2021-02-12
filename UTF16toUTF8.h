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

#pragma once
#include <Windows.h>
#include <string>
namespace Ambiesoft {


char* UTF16toMultiByteEx(UINT cp, LPCWSTR pIN, int inLen = -1, int* pOutLen = nullptr);
char* UTF16toUTF8Ex(LPCWSTR pIN, int inLen = -1, int* pOutLen = nullptr);
LPWSTR UTF16ConvertEndianEx(LPCWSTR pIN);
LPWSTR MultiBytetoUTF16Ex(UINT cp, LPCSTR pIN, int inByteLen = -1, int* pOutLen = nullptr);

LPWSTR UTF8toUTF16Ex(const char* pIN, int inByteLen = -1, int* pOutLen = nullptr);


std::string toStdString(int acp, const std::wstring& w);
std::string toStdUtf8String(const std::wstring& w);
std::string toStdUtf8String(wchar_t c);
std::string toStdUtf8String(int acp, LPCSTR pstr, size_t size);
std::string toStdAcpString(const std::wstring& w);

std::wstring toStdWstring(int acp, const char* pString, int inByteLen);
std::wstring toStdWstringFromUtf8(const std::string& s);
std::wstring toStdWstringFromUtf8(const char* pUtf8, int inByteLen = -1);
std::wstring toStdWstringFromACP(const std::string& s);
std::wstring toStdWstringFromACP(const char* pApcString, int inByteLen = -1);


#ifdef __cplusplus_cli  
char* UTF16toUTF8Ex(System::String^ s);
#endif //__cplusplus_cli  


} // namespace
