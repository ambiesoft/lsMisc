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
#include <comdef.h>
#include <tchar.h>
#include "DebugNew.h"
#include "HtmlClipboard.h"
#include "HelpDefines.h"
#include "AnyCloser.h"

//#ifndef _UNICODE
//	#pragma comment(lib, "comsupp.lib")
//#else
//	#pragma comment(lib, "comsuppw.lib")
//#endif

bool CHtmlClipboard::getHtmlFromClipboard(HWND hWnd, _bstr_t* pHtml, _bstr_t* pTitle, _bstr_t* pURL)
{
	if ( pHtml==NULL )
		return false;

	if ( pTitle )
		*pTitle = L"";

	if ( pURL )
		*pURL = L"";

	LPSTR pcopy = NULL;  // clipboard data
	{
		if(!::OpenClipboard(hWnd))
			return false;

		// make sure clipboard will be closed.
		struct closer_t {
			~closer_t() {
				::CloseClipboard();
			}
		} closer;


		HANDLE h = ::GetClipboardData(getClipboardFormat());
		if ( h==NULL )
			return false;

		LPCSTR p = (LPCSTR)::GlobalLock(h);
		if ( p==NULL )
			return false;

		struct unlocker_t {
			HANDLE h;
			unlocker_t(HANDLE t) : h(t){}
			~unlocker_t() {
				::GlobalUnlock(h);
			}
		} unlocker(h);

		// "HTML Format" is UTF8 encoding.
		pcopy = _strdup(p);
	}

	if ( pcopy == NULL )
		return false;  // failed to obtain clipboard

	CRTFreer freer(pcopy);

	return getHtml(pcopy, pHtml, pTitle, pURL);
}

bool CHtmlClipboard::getHtml(LPSTR p, _bstr_t* pHtml, _bstr_t* pTitle, _bstr_t* pURL)
{
	int startbyte = 0;
	int endbyte = 0;
	bool suc = false;
	char* token = strtok(p, "\n");
	while( token != NULL )
	{
		if ( *token == '<' )
			return false;  // reached html section, maybe

		char* part = strchr(token, ':');
		if ( part==NULL )
		{
			return false;  // reached html section, maybe
		}
		else
		{
			*part = 0;
			part++;
		}

		if ( _stricmp(token, "StartHTML")==0 )
		{
			startbyte = atoi(part);
		}
		else if (_stricmp(token, "EndHTML") == 0)
		{
			endbyte = atoi(part);
		}
		else if (pURL && _stricmp(token, "SourceURL") == 0)
		{
			size_t s = strlen(part);
			if ( s > 0 && part[s-1]=='\r')
			{
				part[s-1] = 0;
				*pURL = part;
				part[s-1] = '\r';
			}
		}
		if ( startbyte != 0 && endbyte != 0 && (!pURL || pURL->length() != 0) )
		{// startbyte and endbyte found

			if ( startbyte < 0 || endbyte < 0 || startbyte > endbyte )
				return false;

			// memmove would smartly overwrite.
			memmove(p, p+startbyte, (endbyte-startbyte+1));
			suc = true;
			break;
		}
		token = strtok( NULL, "\n" ); // C4996
	}

	if ( !suc )
		return false;

	// obtain the required size;
	int req = ::MultiByteToWideChar(CP_UTF8,
		0,
		p,
		-1,
		NULL,
		0);

	if ( req == 0 )
		return false;

	// transfer from UTF8 to UTF16
//	WCHAR* pw = (WCHAR*)_alloca(req*sizeof(WCHAR));
	WCHAR* pw = (WCHAR*)malloc(req*sizeof(WCHAR));
	CRTFreer freer(pw);

	int r = ::MultiByteToWideChar(CP_UTF8,
		0,
		p,
		-1,
		pw,
		req);

	if ( r==0 || r!=req )
		return false;

	if ( pTitle )
	{
		LPCWSTR p = wcsstr(pw, L"<title");
		if ( p==NULL )
			p = wcsstr(pw, L"<TITLE");
		if ( p==NULL )
			p = wcsstr(pw, L"<Title");
		if ( p )
		{
			while(*p && *p != L'>')
				p++;
			p++;
			LPWSTR pe = (LPWSTR)wcsstr(p, L"</title");
			if ( pe==NULL )
				pe = (LPWSTR)wcsstr(p, L"</TITLE");
			if ( pe==NULL )
				pe = (LPWSTR)wcsstr(p, L"</Title");
			if ( pe==NULL )
				pe = (LPWSTR)wcsstr(p, L"</");

			*pe = 0;
			*pTitle = p;
			*pe = L'<';
			
		}
	}

	*pHtml = pw;
	return true;
}
UINT CHtmlClipboard::getClipboardFormat(void)
{
	static UINT cf = 0;
	if (cf==0)
		cf = ::RegisterClipboardFormat(_T("HTML Format"));

	return cf;
}
