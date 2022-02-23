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

namespace Ambiesoft {

	BOOL OpenCommonW(
		HWND hWnd,
		LPCWSTR pApp,
		LPCWSTR pArg = NULL,
		LPCWSTR pDirectory = NULL,
		HANDLE* pPHandle = NULL,
		LPCWSTR pVerb = NULL);
	BOOL OpenCommonA(
		HWND hWnd,
		LPCSTR pApp,
		LPCSTR pArg = NULL,
		LPCSTR pDirectory = NULL,
		HANDLE* pPHandle = NULL,
		LPCSTR pVerb = NULL);

	inline BOOL OpenCommon(
		HWND hWnd,
		LPCTSTR pApp,
		LPCTSTR pArg = NULL,
		LPCTSTR pDirectory = NULL,
		HANDLE* pPHandle = NULL,
		LPCTSTR pVerb = NULL)
	{
#ifdef UNICODE
		return OpenCommonW(
#else
		return OpenCommonA(
#endif
			hWnd,
			pApp,
			pArg,
			pDirectory,
			pPHandle,
			pVerb);
	}

	BOOL OpenFolderW(HWND h, LPCWSTR pFileOrFolder);
	BOOL OpenFolderA(HWND h, LPCSTR pFileOrFolder);
	inline BOOL OpenFolder(HWND h, LPCTSTR pFileOrFolder)
	{
#ifdef UNICODE
		return OpenFolderW(
#else
		return OpenFolderA(
#endif
			h,
			pFileOrFolder);
	}

	BOOL ReopenCommon(HWND h);
} // namespace Ambiesoft