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

#ifndef _INC_SHELLAPI
#include <Shellapi.h>
#endif

#ifndef _INC_SHLWAPI
#include <shlwapi.h>
#ifdef _MSC_VER
#pragma comment(lib,"shlwapi.lib")
#endif
#endif

#ifndef _STRING_
#include <string>
#endif
#include "DebugNew.h"

#include "CommandLineString.h"
#include "OpenCommon.h"



namespace Ambiesoft {

	BOOL OpenCommonW(HWND hWnd,
		LPCWSTR pApp,
		LPCWSTR pCommand,
		LPCWSTR pDirectory,
		HANDLE* pPHandle,
		LPCWSTR pVerb)
	{
        SHELLEXECUTEINFOW sei;
        memset(&sei,0,sizeof(sei));

		sei.cbSize = sizeof(sei);
		sei.fMask = 0;
		if (pPHandle)
		{
			sei.fMask |= SEE_MASK_NOCLOSEPROCESS;
		}

		sei.fMask |= SEE_MASK_UNICODE;

		sei.hwnd = hWnd;
		sei.lpFile = pApp;
		sei.lpParameters = pCommand;
		sei.lpDirectory = pDirectory;
		sei.nShow = SW_SHOW;
		sei.lpVerb = pVerb;

		BOOL ret = ShellExecuteExW(&sei);
		if (ret && pPHandle)
		{
			*pPHandle = sei.hProcess;
		}

		return ret;
	}

	BOOL OpenCommonA(HWND hWnd,
		LPCSTR pApp,
		LPCSTR pCommand,
		LPCSTR pDirectory,
		HANDLE* pPHandle,
		LPCSTR pVerb)
	{
        SHELLEXECUTEINFOA sei;
        memset(&sei,0,sizeof(sei));

		sei.cbSize = sizeof(sei);
		sei.fMask = 0;
		if (pPHandle)
		{
			sei.fMask |= SEE_MASK_NOCLOSEPROCESS;
		}

		sei.hwnd = hWnd;
		sei.lpFile = pApp;
		sei.lpParameters = pCommand;
		sei.lpDirectory = pDirectory;
		sei.nShow = SW_SHOW;
		sei.lpVerb = pVerb;

		BOOL ret = ShellExecuteExA(&sei);
		if (ret && pPHandle)
		{
			*pPHandle = sei.hProcess;
		}

		return ret;
	}

	BOOL OpenFolderA(HWND h, LPCSTR pFileOrFolder)
	{
		if (!PathFileExistsA(pFileOrFolder) && !PathIsDirectoryA(pFileOrFolder))
			return FALSE;

		std::string arg = "/select,\"";
		arg += pFileOrFolder;
		arg += "\",/n";
		ShellExecuteA(h,
			NULL,
			"explorer.exe",
			arg.c_str(),
			NULL,
			SW_SHOW);

		return TRUE;
	}
	BOOL OpenFolderW(HWND h, LPCWSTR pFileOrFolder)
	{
		if (!PathFileExistsW(pFileOrFolder) && !PathIsDirectoryW(pFileOrFolder))
			return FALSE;

		std::wstring arg = L"/select,\"";
		arg += pFileOrFolder;
		arg += L"\",/n";
		ShellExecuteW(h,
			NULL,
			L"explorer.exe",
			arg.c_str(),
			NULL,
			SW_SHOW);

		return TRUE;
	}

	BOOL ReopenCommon(HWND h)
	{
		std::wstring exe, arg;
		CCommandLineString::ExplodeExeAndArg(GetCommandLine(), exe, arg);
		return OpenCommonW(h, exe.c_str(), arg.c_str());
	}
} // namespace ambiesoft
