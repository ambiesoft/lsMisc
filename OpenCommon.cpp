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
#include "UTF16toUTF8.h"

#ifndef _INC_SHELLAPI
#include <Shellapi.h>
#endif

#ifndef _INC_SHLWAPI
#include <shlwapi.h>
#ifdef _MSC_VER
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"Ole32.lib")
#endif
#endif

#include <windows.h>
#include <shldisp.h> // IShellDispatch
//#include <comutil.h> // _bstr_t (COM string utility)
#include <shlobj.h>

#ifndef _STRING_
#include <string>
#endif
#include "DebugNew.h"

#include "CommandLineString.h"
#include "OpenCommon.h"



namespace Ambiesoft {

	BOOL OpenCommonW(HWND hWnd,
		LPCWSTR pApp,
		LPCWSTR pArg,
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
		sei.lpParameters = pArg;
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
		LPCSTR pArg,
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
		sei.lpParameters = pArg;
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

	//BOOL OpenFolderA(HWND h, LPCSTR pFileOrFolder)
	//{
	//	return OpenFolderW(h, toStdWstringFromACP(pFileOrFolder).c_str());
	//}

	static bool OpenExplorerAndSelectFile(const std::wstring& filePath) {
		size_t pos = filePath.find_last_of(L"\\/");
		if (pos == std::wstring::npos) {
			return false;
		}

		std::wstring folderPath = filePath.substr(0, pos);
		std::wstring fileName = filePath.substr(pos + 1);

		PIDLIST_ABSOLUTE pidlFolder = nullptr;
		HRESULT hr = SHParseDisplayName(folderPath.c_str(), nullptr, &pidlFolder, 0, nullptr);
		if (FAILED(hr)) {
			return false;
		}

		PIDLIST_ABSOLUTE pidlItem = nullptr;
		hr = SHParseDisplayName(filePath.c_str(), nullptr, &pidlItem, 0, nullptr);
		if (FAILED(hr)) {
			CoTaskMemFree(pidlFolder);
			return false;
		}

		std::vector<PCUITEMID_CHILD> pidlItems = { reinterpret_cast<PCUITEMID_CHILD>(pidlItem) };

		hr = SHOpenFolderAndSelectItems(pidlFolder, static_cast<UINT>(pidlItems.size()), pidlItems.data(), 0);

		CoTaskMemFree(pidlFolder);
		CoTaskMemFree(pidlItem);

		if (FAILED(hr)) {
			return false;
		}

		return true;
	}
	BOOL OpenFolderW(HWND , LPCWSTR pFileOrFolder)
	{
		wchar_t fileOrFolder[MAX_PATH];
		lstrcpyW(fileOrFolder, pFileOrFolder);
		if (!PathFileExistsW(pFileOrFolder) && !PathIsDirectoryW(pFileOrFolder))
			return FALSE;
		if (PathIsDirectoryW(pFileOrFolder))
		{
			PathAddBackslashW(fileOrFolder);
		}
		return !!OpenExplorerAndSelectFile(fileOrFolder);

		// following code create explorer.exe process which does not close forever
		//std::wstring arg = L"/select,\"";
		//arg += pFileOrFolder;
		//arg += L"\",/n";

		//ShellExecuteW(h,
		//	NULL,
		//	L"explorer.exe",
		//	arg.c_str(),
		//	NULL,
		//	SW_SHOW);

		//return TRUE;
	}

	BOOL ReopenCommon(HWND h)
	{
		std::wstring exe, arg;
		CCommandLineString::ExplodeExeAndArg(GetCommandLine(), exe, arg);
		return OpenCommonW(h, exe.c_str(), arg.c_str());
	}
} // namespace ambiesoft
