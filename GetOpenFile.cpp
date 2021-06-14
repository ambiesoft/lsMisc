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

#include "stdafx.h"

#include <Windows.h>
#include <Commdlg.h>
#include <tchar.h>
#include <string.h>
#include <string>
using namespace std;

#include"GetOpenFile.h"

#pragma comment(lib,"Comdlg32.lib")

#ifndef OFN_DONTADDTORECENT
	#define OFN_DONTADDTORECENT 0x2000000 
#endif

namespace Ambiesoft {

#define OFNBUFFERSIZE 1024
	struct OFNBUFFER
	{
		LPTSTR pFile_;
		LPTSTR pFileTitle_;
		LPTSTR pFilter_;
		OFNBUFFER() :
			pFile_(new TCHAR[OFNBUFFERSIZE]),
			pFileTitle_(new TCHAR[OFNBUFFERSIZE]),
			pFilter_(new TCHAR[OFNBUFFERSIZE]) {
			pFile_[0] = 0;
			pFileTitle_[0] = 0;
			pFilter_[0] = 0;
		}

		~OFNBUFFER() {
			delete[] pFile_;
			delete[] pFileTitle_;
			delete[] pFilter_;
		}
	};

	BOOL GetOpenFile(
		HINSTANCE hInst,
		HWND hWnd,
		LPCTSTR pFilter,
		LPCTSTR pInitialDir,
		LPCTSTR pTitle,
		wstring* fullpath, wstring* path)
	{
		OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hInstance = hInst;
		ofn.hwndOwner = hWnd;
		ofn.Flags =
			OFN_DONTADDTORECENT |
			OFN_ENABLESIZING |
			OFN_FILEMUSTEXIST |
			// OFN_HIDEREADONLY |
			OFN_PATHMUSTEXIST | 
			0;

		OFNBUFFER buff;
		if (pFilter)
		{
			memset(buff.pFilter_, 0, OFNBUFFERSIZE);
			lstrcpy(buff.pFilter_, pFilter);
			LPTSTR p = buff.pFilter_;
			while (*p && (p = _tcschr(p, L'|')))
				*p++ = 0;

			ofn.lpstrFilter = buff.pFilter_;
			ofn.nFilterIndex = 1;
		}

		ofn.lpstrFile = buff.pFile_;
		ofn.nMaxFile = OFNBUFFERSIZE;

		ofn.lpstrFileTitle = buff.pFileTitle_;
		ofn.nMaxFileTitle = OFNBUFFERSIZE;

		ofn.lpstrInitialDir = pInitialDir;

		ofn.lpstrTitle = pTitle;

		if (!GetOpenFileName(&ofn))
			return FALSE;

		if (path)
			*path = ofn.lpstrFileTitle;

		*fullpath = ofn.lpstrFile;
		return TRUE;
	}

	BOOL GetOpenFile(
		HINSTANCE hInst,
		HWND hWnd,
		GETFILEFILTER filter,
		LPCTSTR pInitialDir,
		LPCTSTR pTitle,
		std::wstring* fullpath, std::wstring* path)
	{
		std::wstring strFilter;
		switch (filter)
		{
		case GETFILEFILTER::APP:
			strFilter = _T("Application|*.exe;*.com|All Files|*.*");
			break;
		}
		return GetOpenFile(hInst, hWnd, strFilter.c_str(), pInitialDir, pTitle,
			fullpath, path);
	}


	BOOL GetSaveFile(
		HWND hWnd,
		LPCTSTR pFilter,
		LPCTSTR pInitialDir,
		LPCTSTR pInitialFile,
		LPCTSTR pDefExt,
		LPCTSTR pTitle,
		wstring* fullpath, wstring* path)
	{
		OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hWnd;
		ofn.Flags =
			OFN_DONTADDTORECENT |
			OFN_ENABLESIZING |
			OFN_OVERWRITEPROMPT |
			OFN_HIDEREADONLY |
			0;

		OFNBUFFER buff;
		if (pFilter)
		{
			lstrcpy(buff.pFilter_, pFilter);
			LPTSTR p = buff.pFilter_;
			while (*p && (p = _tcschr(p, L'|')))
				*p++ = 0;
			ofn.lpstrFilter = buff.pFilter_;
		}

		if (pInitialFile)
		{
			lstrcpy(buff.pFile_, pInitialFile);
		}

		ofn.lpstrFile = buff.pFile_;
		ofn.nMaxFile = OFNBUFFERSIZE;

		ofn.lpstrFileTitle = buff.pFileTitle_;
		ofn.nMaxFileTitle = OFNBUFFERSIZE;

		ofn.lpstrInitialDir = pInitialDir;

		ofn.lpstrDefExt = pDefExt;

		ofn.lpstrTitle = pTitle;
		if (!GetSaveFileName(&ofn))
			return FALSE;

		if (path)
			*path = ofn.lpstrFileTitle;

		*fullpath = ofn.lpstrFile;
		return TRUE;
	}
}