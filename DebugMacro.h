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
#include <cassert>

#ifdef __cplusplus_cli
inline bool isDerived(System::Object^ o, System::Type^ t)
{
	if (!o || !t)
		return false;

	System::Type^ ot = o->GetType();
	do
	{
		if (ot == t)
			return true;

		ot = ot->BaseType;
	} while (ot != System::Object::typeid);

	return false;
}
#endif

#if defined(_DEBUG)
#ifdef __cplusplus_cli
	#define DASSERT(s) System::Diagnostics::Debug::Assert(!!(s))
	inline void DTRACE(System::Object^ o) { System::Diagnostics::Debug::Print(o ? o->ToString() : L"<NULL>"); }
#else
	#define DASSERT(s) assert(s)
	inline void DTRACE(const char* pMessage) {
		OutputDebugStringA(pMessage);
		OutputDebugStringA("\r\n");
	}
	inline void DTRACE(const std::string& message) {
		DTRACE(message.c_str());
	}
	inline void DTRACE(const wchar_t* pMessage) {
		OutputDebugStringW(pMessage);
		OutputDebugStringW(L"\r\n");
	}
	inline void DTRACE(const std::wstring& message) {
		DTRACE(message.c_str());
	}
#endif

	void DTRACE_LASTERROR(DWORD dwLE);
	#define DTRACE_NUMBER(start) do { static int num = start; DTRACE(num.ToString()); ++num; } while(0);
	#define DTRACE_NUMBERTEXT(start,text) do { static int num = start; DTRACE(num.ToString() + ":" + text); ++num; } while(0);
	#define DVERIFY DASSERT
	#define DVERIFY_IS(a,b)  DASSERT( (a)==(b) )
	#define DVERIFY_NOT(a,b) DASSERT( (a)!=(b) )
	#define DVERIFY_GL(a) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=0 ) { DASSERT(gle || 0); } } while(0)
	#define DVERIFY_GL_NUM(a,n) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=n ) { DASSERT( gle || 0); } } while(0)
	#define DVERIFY_GL_NUM2(a,n1, n2) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=n1 && gle!= n2) { DTRACE(gle.ToString()); DASSERT( gle || 0); } } while(0)
	#define DASSERT_IS_CLASS(instance, clazz) DASSERT( ((instance)->GetType())==clazz::typeid ) 
	#define DASSERT_IS_CLASS_OR_NULL(instance, clazz) DASSERT( instance==nullptr || ((instance)->GetType())==clazz::typeid ) 
#ifdef __cplusplus_cli
	#define DASSERT_IS_DERIVED(instance, clazz) DASSERT(isDerived(instance, clazz::typeid))
#endif
#else // not _DEBUG
	#define DASSERT(s) ((void)0)
	#define DTRACE(s) ((void)0)
	#define DTRACE_LASTERROR(s) ((void)0)
	#define DTRACE_NUMBER(start) ((void)0)
	#define DTRACE_NUMBERTEXT(start,text) ((void)0)
	#define DVERIFY(a) (a)
	#define DVERIFY_IS(a,b)  (a)
	#define DVERIFY_NOT(a,b) (a)
	#define DVERIFY_GL(a) (a)
	#define DVERIFY_GL_NUM(a,n) (a)
	#define DVERIFY_GL_NUM2(a,n1, n2) (a)
	#define DASSERT_IS_CLASS(instance, clazz) ((void)0)
	#define DASSERT_IS_CLASS_OR_NULL(instance, clazz) ((void)0)
	#define DASSERT_IS_DERIVED(instance, clazz) ((void)0)
#endif


