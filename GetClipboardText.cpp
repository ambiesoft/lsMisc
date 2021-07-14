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

#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif


#include <windows.h>
#include "tstring.h"

#include "UTF16toUTF8.h"

#include "GetClipboardText.h"

using namespace std;

namespace Ambiesoft {
	static BOOL getClipboardTextData(HWND hWnd, int cf, string& strData)
	{
		if (!IsClipboardFormatAvailable(cf))
			return FALSE;
		if (!OpenClipboard(hWnd))
			return FALSE;

		struct ClipboardCloser {
			~ClipboardCloser() { ::CloseClipboard(); }
		} closer;

		HANDLE hData = ::GetClipboardData(cf);
		if (NULL == hData)
			return FALSE;

		LPCSTR p = (LPCSTR)GlobalLock(hData);
		if (!p)
			return FALSE;

		strData = p;
		GlobalUnlock(hData);


		return TRUE;
	}
	BOOL GetClipboardText(HWND hWnd, string& strData)
	{
		return getClipboardTextData(hWnd, CF_TEXT, strData);
	}

	static BOOL getHtmlPosition(LPCSTR pStr, LPCSTR pKey, int& val)
	{
		if (!pStr || pStr[0] == 0)
			return FALSE;

		if (!pKey || pKey[0] == 0)
			return FALSE;

		LPCSTR pS = strstr(pStr, pKey);
		if (!pS)
			return FALSE;

		pS += strlen(pKey);
		BOOL zeroFilled = false;
		string result;
		for (; *pS != 0 && ('0' <= *pS && *pS <= '9'); ++pS)
		{
			if (!zeroFilled)
			{
				if ('0' == *pS)
					continue;
				zeroFilled = true;
				result += *pS;
			}
			else
			{
				result += *pS;
			}
		}

		val = atoi(result.c_str());
		return TRUE;
	}

	BOOL GetClipboardHtml(HWND hWnd, string& strData)
	{
		static int cf = RegisterClipboardFormatA("HTML Format");

		string strT;
		if (!getClipboardTextData(hWnd, cf, strT))
			return FALSE;

		int start = -1;
		if (!getHtmlPosition(strT.c_str(), "StartHTML:", start))
			return FALSE;
		if (start < 0)
			return FALSE;

		int end = -1;
		if (!getHtmlPosition(strT.c_str(), "EndHTML:", end))
			return FALSE;
		if (end < 0)
			return FALSE;

		if (start >= end)
			return FALSE;

		strData = strT.substr(start, end - start);
		return TRUE;
	}









	static BOOL getClipboardTextData(HWND hWnd, int cf, wstring& strData)
	{
		if (!IsClipboardFormatAvailable(cf))
			return FALSE;
		if (!OpenClipboard(hWnd))
			return FALSE;

		struct ClipboardCloser {
			~ClipboardCloser() { ::CloseClipboard(); }
		} closer;

		HANDLE hData = ::GetClipboardData(cf);
		if (NULL == hData)
			return FALSE;

		LPCWSTR p = (LPCWSTR)GlobalLock(hData);
		if (!p)
			return FALSE;

		strData = p;
		GlobalUnlock(hData);


		return TRUE;
	}
	BOOL GetClipboardText(HWND hWnd, wstring& strData)
	{
		return getClipboardTextData(hWnd, CF_UNICODETEXT, strData);
	}
	BOOL GetClipboardHtml(HWND hWnd, wstring& strData)
	{
		string strU8;
		if (!GetClipboardHtml(hWnd, strU8))
			return FALSE;

		strData = toStdWstringFromUtf8(strU8);
		//if (!UTF8toUTF16((const LPBYTE)strU8.c_str(), strData))
		//	return FALSE;

		return TRUE;
	}

}