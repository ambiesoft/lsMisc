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
#include PCH_INCLUDE_FILE


#include <Windows.h>
#include <string>
#include <vector>

#include "SingleAppMutex.h"

using namespace std;
namespace Ambiesoft {

	struct EWProcInfo
	{
		const wchar_t* pTitle_;
		const wchar_t* pClassName_;
		std::vector<HWND>* pV_;

		EWProcInfo(
			const wchar_t* pTitle,
			const wchar_t* pClassName,
			std::vector<HWND>* pV
			) :
			pTitle_(pTitle),
			pClassName_(pClassName),
			pV_(pV){}
	};
	BOOL CALLBACK ewProc(
		HWND   hwnd,
		LPARAM lParam)
	{
		EWProcInfo& info = *((EWProcInfo*)lParam);
		if (info.pTitle_)
		{
			int winlen = GetWindowTextLength(hwnd);
			int infotitlelen = lstrlen(info.pTitle_);
			if (winlen != infotitlelen)
				return TRUE;
			
			wstring wintitle;
			wintitle.reserve(infotitlelen + 2);
			GetWindowText(hwnd, (LPWSTR)wintitle.data(), infotitlelen+1);
			if (lstrcmp(wintitle.c_str(), info.pTitle_) != 0)
				return TRUE;
		}
		if (info.pClassName_)
		{
			int infoclasslen = lstrlen(info.pClassName_);
			wstring winclass;
			winclass.reserve(infoclasslen + 2);
			GetClassName(hwnd, (LPWSTR)winclass.data(), infoclasslen + 1);
			if (lstrcmp(winclass.c_str(), info.pClassName_) != 0)
				return TRUE;
		}

		info.pV_->push_back(hwnd);
		return TRUE;
	}

	std::vector<HWND> CSingleAppMutex::GetExistingWindow(const wchar_t* pTitle, const wchar_t* pClassName)
	{
		vector<HWND> ret;
		EWProcInfo info(pTitle, pClassName, &ret);
		EnumWindows(ewProc, (LPARAM)&info);
		return ret;
	}
	bool CSingleAppMutex::SendInfo(HWND hwnd, HWND hSender, ULONG_PTR category, const unsigned char* data, unsigned int len)
	{
		if (!IsWindow(hwnd))
			return false;

		COPYDATASTRUCT cds;
		cds.dwData = category;
		cds.cbData = len;
		cds.lpData = (PVOID)data;
		return !!SendMessage(hwnd, WM_COPYDATA, (WPARAM)hSender, (LPARAM)&cds);
	}
	void CSingleAppMutex::ReceiveInfo(WPARAM wParam, LPARAM lParam, HWND* phSender, ULONG_PTR* pCategory, unsigned int* pLen, unsigned char** pData)
	{
		if (phSender)
			*phSender = (HWND)wParam;

		COPYDATASTRUCT* pCds = (COPYDATASTRUCT*)lParam;
		if (pCategory)
			*pCategory = pCds->dwData;

		if (pLen)
			*pLen = pCds->cbData;

		if (pData)
		{
			*pData = (unsigned char*)LocalAlloc(0, pCds->cbData);
			memcpy(*pData, pCds->lpData, pCds->cbData);
		}
	}
}











