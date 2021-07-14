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







#include <comdef.h>
#include "AnyCloser.h"

#include "addBaseElement.h"

#include "DebugNew.h"

namespace Ambiesoft {
	bool addBaseElement(_bstr_t& bstrHtml, LPCWSTR pSourceURL)
	{
		if (pSourceURL == NULL || pSourceURL[0] == 0)
			return false;

		WCHAR* pw = _wcsdup(bstrHtml);
		if (!pw)
			return false;

		//	struct strfreer_t {
		//		WCHAR* p;
		//		strfreer_t(WCHAR* q) : p(q) {}
		//		~strfreer_t() {
		//			free((void*)p);
		//		}
		//	} strfreer(pw);

		CRTFreer freer(pw);

		LPWSTR p = (LPWSTR)wcsstr(pw, L"<head");
		if (p == NULL)
			p = wcsstr(pw, L"<HEAD");
		if (p == NULL)
			p = wcsstr(pw, L"<Head");
		if (p)
		{
			while (*p && *p != L'>')
				p++;
			p++;

			_bstr_t ret;

			WCHAR c = *p;
			*p = 0;
			ret += pw;

			ret += L"<base href=\"";
			ret += pSourceURL;
			ret += L"\" />";

			*p = c;
			ret += p;
			bstrHtml = ret;
			return true;
		}
		else
		{
			LPWSTR p = (LPWSTR)wcsstr(pw, L"<html");
			if (p == NULL)
				p = wcsstr(pw, L"<HTML");
			if (p == NULL)
				p = wcsstr(pw, L"<Html");

			if (p)
			{
				while (*p && *p != L'>')
					p++;
				p++;

				_bstr_t ret;

				WCHAR c = *p;
				*p = 0;
				ret += pw;

				ret += L"<head><base href=\"";
				ret += pSourceURL;
				ret += L"\" /></head>";

				*p = c;
				ret += p;
				bstrHtml = ret;
				return true;
			}
		}
		return false;
	}





















	//bool addBaseElement(_bstr_t& bstrHtml, LPCWSTR pSourceURL)
	//{
	//	if ( !pSourceURL )
	//		return false;
	//
	//	WCHAR* pW = wcsdup(bstrHtml);
	//	if(!pW)
	//		return false;
	//
	//	struct strfreer_t {
	//		WCHAR* p;
	//		strfreer_t(WCHAR* q) : p(q) {}
	//		~strfreer_t() {
	//			free((void*)p);
	//		}
	//	} strfreer(pW);
	//
	//	WCHAR* p = wcsstr(pW, L"</head");
	//	if ( p==NULL )
	//		p = wcsstr(pW, L"</HEAD");
	//	if ( p==NULL )
	//		p = wcsstr(pW, L"</Head");
	//	if ( p==NULL )
	//	{ // could not find head tag, try to find body tag
	//		p = wcsstr(pW, L"<body");
	//		if ( p==NULL )
	//			p = wcsstr(pW, L"<BODY");
	//		if ( p==NULL )
	//			p = wcsstr(pW, L"<Body");
	//		if ( p==NULL )
	//		{
	//			ASSERT(false);
	//			return false;
	//		}
	//
	//		// body tag found.
	//		_bstr_t ret;
	//		*p = 0;
	//		ret = pW;
	//		*p = L'<';
	//
	//		ret += L"<head><base href=\"";
	//		ret += pSourceURL;
	//		ret += L"\" /></head>";
	//
	//		ret += p;
	//		bstrHtml = ret;
	//		return true;
	//	}
	//	else
	//	{ // head tag found.
	//		_bstr_t ret;
	//		*p = 0;
	//		ret = pW;
	//		*p = L'<';
	//
	//		ret += L"<base href=\"";
	//		ret += pSourceURL;
	//		ret += L"\" />";
	//
	//		ret += p;
	//
	//		bstrHtml = ret;
	//		return true;
	//	}
	//	ASSERT(false);
	//	return false;
	//}
}