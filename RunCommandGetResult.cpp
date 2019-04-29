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

#include <cassert>
#include <string>
#include <codecvt>
#include <process.h>

#include "RunCommandGetResult.h"
#include "DebugNew.h"

using namespace std;
namespace Ambiesoft {
	
	static BOOL CreateHandles(HANDLE& h1, HANDLE& h2, BOOL i1, BOOL i2, DWORD* pdwLastError)
	{
		SECURITY_ATTRIBUTES sa;

		ZeroMemory(&sa, sizeof(sa));
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		if (!CreatePipe(&h1, &h2, &sa, 0))
		{
			if (pdwLastError)
				*pdwLastError = GetLastError();
			return FALSE;
		}

		if (!i1)
		{
			if (!SetHandleInformation(h1, HANDLE_FLAG_INHERIT, 0))
			{
				if (pdwLastError)
					*pdwLastError = GetLastError();
				return FALSE;
			}
		}
		if (!i2)
		{
			if (!SetHandleInformation(h2, HANDLE_FLAG_INHERIT, 0))
			{
				if (pdwLastError)
					*pdwLastError = GetLastError();
				return FALSE;
			}
		}

#ifdef _DEBUG
		{
			// check inheritance
			DWORD dwFlags = 0;
			if (!GetHandleInformation(h1, &dwFlags))
				assert(false);
			assert((i1 && (dwFlags & HANDLE_FLAG_INHERIT)) || (!i1 && (dwFlags & HANDLE_FLAG_INHERIT) == 0));

			if (!GetHandleInformation(h2, &dwFlags))
				assert(false);
			assert((i2 && (dwFlags & HANDLE_FLAG_INHERIT)) || (!i2 && (dwFlags & HANDLE_FLAG_INHERIT) == 0));
		}
#endif
		return TRUE;
	}

	static wstring dq(LPCWSTR p)
	{
		if (!p || !p[0])
			return wstring();
		if (p[0] == L'"')
			return p;
		for (LPCWSTR pT = p; *pT; ++pT)
		{
			if (*pT == L' ')
				return wstring(L"\"") + p + L"\"";
		}
		return p;
	}
	struct WorkerStruct {
		HANDLE h_;
		string s_;
	};
	static UINT __stdcall workerThread(LPVOID pParam)
	{
		WorkerStruct* pWorker = (WorkerStruct*)pParam;
		DWORD d = 0;
		BYTE szB[4096] = { 0 };
		while (ReadFile(pWorker->h_,
			szB,
			sizeof(szB) - 1,
			&d,
			NULL))
		{
			
			pWorker->s_ += (char*)szB;
			ZeroMemory(szB, sizeof(szB));
		}
		return 0;
	}


	struct HandleFreer {
		HANDLE* ph_;
		HandleFreer(HANDLE* ph) :ph_(ph)
		{
			assert(ph);
		}
		~HandleFreer() {
			if (*ph_)
				CloseHandle(*ph_);
		}
	};

	struct CHandle {
		HANDLE h_;
		CHandle(HANDLE h) :h_(h)
		{

		}
		~CHandle() {
			Close();
		}
		HANDLE Detach() {
			HANDLE ret = h_;
			h_ = NULL;
			return ret;
		}
		void Close() {
			if (h_)
				CloseHandle(h_);
			h_ = NULL;
		}
	};
	static BOOL ClearHandle(HANDLE& h)
	{
		BOOL bRet = CloseHandle(h);
		h = NULL;
		return bRet;
	}
	static wchar_t* wcsnew(const wchar_t* p)
	{
		if (!p)
			return nullptr;

		size_t size = wcslen(p) + 1;
		wchar_t* pRet = new wchar_t[size];
		wcscpy_s(pRet, size, p);
		return pRet;
	}
	BOOL RunCommandGetResult(
		LPCWSTR pExe,
		LPCWSTR pArg,
		DWORD* pIRetCommand, 
		std::string* pStrOutCommand,
		std::string* pStrErrCommand,
		DWORD* pdwLastError)
	{
		HANDLE hPipeStdInRead = NULL;
		HANDLE hPipeStdInWrite = NULL;
		if (!CreateHandles(hPipeStdInRead, hPipeStdInWrite, TRUE, FALSE, pdwLastError))
			return FALSE;
		HandleFreer h1(&hPipeStdInRead);
		HandleFreer h2(&hPipeStdInWrite);

		HANDLE hPipeStdOutRead = nullptr;
		HANDLE hPipeStdOutWrite = nullptr;
		if (!CreateHandles(hPipeStdOutRead, hPipeStdOutWrite, FALSE, TRUE, pdwLastError))
			return FALSE;
		HandleFreer h3(&hPipeStdOutRead);
		HandleFreer h4(&hPipeStdOutWrite);

		HANDLE hPipeStdErrRead = nullptr;
		HANDLE hPipeStdErrWrite = nullptr;
		if (!CreateHandles(hPipeStdErrRead, hPipeStdErrWrite, FALSE, TRUE, pdwLastError))
			return FALSE;
		HandleFreer h5(&hPipeStdErrRead);
		HandleFreer h6(&hPipeStdErrWrite);

		// createprocess
		// LPWSTR pArgCP = pArg ? _wcsdup(pArg) : NULL;
		// STLSOFT_SCODEDFREE_CRT(pArgCP);
		wstring argCP = pArg ? pArg : wstring();
		

		STARTUPINFOW siStartInfo = { 0 };
		siStartInfo.cb = sizeof(STARTUPINFO);
		siStartInfo.hStdInput = hPipeStdInRead;
		siStartInfo.hStdOutput = hPipeStdOutWrite;
		siStartInfo.hStdError = hPipeStdErrWrite;
		siStartInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		siStartInfo.wShowWindow = SW_HIDE;

		SECURITY_ATTRIBUTES sa = { 0 };
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;

		wstring cmd = dq(pExe);
		if (!argCP.empty())
		{
			cmd += L" ";
			cmd += argCP;
		}
		unique_ptr<wchar_t[]> pCmd(wcsnew(cmd.c_str()));
		PROCESS_INFORMATION pi = { 0 };
		if(!CreateProcessW(
			NULL,
			pCmd.get(),
			&sa, // sec
			nullptr, // sec thread
			TRUE, // inherit
			0, // flags
			nullptr, // env
			nullptr, // dir
			&siStartInfo,
			&pi))
		{
			if (pdwLastError)
				*pdwLastError = GetLastError();
			return FALSE;
		}
		HandleFreer h7(&pi.hProcess);
		HandleFreer h8(&pi.hThread);

		ClearHandle(hPipeStdInRead);
		ClearHandle(hPipeStdOutWrite);
		ClearHandle(hPipeStdErrWrite);

		WorkerStruct wsOut;
		wsOut.h_ = hPipeStdOutRead;
		HANDLE hWorkerOut = (HANDLE)_beginthreadex(NULL,
			0,
			workerThread,
			&wsOut,
			CREATE_SUSPENDED,
			NULL);
		HandleFreer hWorkerOut_Free(&hWorkerOut);

		WorkerStruct wsErr;
		wsErr.h_ = hPipeStdErrRead;
		HANDLE hWorkerErr = (HANDLE)_beginthreadex(NULL,
			0,
			workerThread,
			&wsErr,
			CREATE_SUSPENDED,
			NULL);
		HandleFreer hWorkerErr_Free(&hWorkerErr);

		
		ResumeThread(hWorkerOut);
		ResumeThread(hWorkerErr);
		HANDLE hWaits[] = { pi.hProcess, hWorkerOut, hWorkerErr };
		WaitForMultipleObjects(sizeof(hWaits) / sizeof(hWaits[0]), hWaits, TRUE, INFINITE);

		if (pStrOutCommand)
			*pStrOutCommand = wsOut.s_;
		if (pStrErrCommand)
			*pStrErrCommand = wsErr.s_;

		if (pIRetCommand)
			GetExitCodeProcess(pi.hProcess, pIRetCommand);

		return TRUE;
	}

	static wstring toWstring(const char* pIN)
	{
		if (!pIN || pIN[0] == 0)
			return wstring();

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(pIN);
	}
	BOOL RunCommandGetResult(
		LPCSTR pExe,
		LPCSTR pArg,
		DWORD* pIRetCommand,
		std::string* pStrOutCommand,
		std::string* pStrErrCommand,
		DWORD* pdwLastError)
	{
		return RunCommandGetResult(
			toWstring(pExe).c_str(),
			toWstring(pArg).c_str(),
			pIRetCommand,
			pStrOutCommand,
			pStrErrCommand,
			pdwLastError);
	}
}