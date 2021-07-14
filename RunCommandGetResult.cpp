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



#include <cassert>
#include <string>
#include <codecvt>
#include <memory>

#include <process.h>

#include "RunCommandGetResult.h"
#include "UTF16toUTF8.h"

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
	private:
		HANDLE h_;
		string* s_;
		using OutFunc = std::function<void(const char* pOutput, void* pUserData)>;
		OutFunc fnOut_;
		void* pUserData_;
	public:
		WorkerStruct(HANDLE h, string* s, OutFunc of, void* pUD) :
			h_(h), s_(s), fnOut_(of), pUserData_(pUD)
		{}
		HANDLE handle() {
			return h_;
		}
		void OnOut(const char* p) {
			if (s_)
				*s_ += p;
			if (fnOut_)
				fnOut_(p, pUserData_);
		}
	};

	static UINT __stdcall workerThread(LPVOID pParam)
	{
		WorkerStruct* pWorker = (WorkerStruct*)pParam;
		DWORD d = 0;
		char szB[4096] = { 0 };
		while (ReadFile(pWorker->handle(),
			szB,
			sizeof(szB) - 1,
			&d,
			NULL))
		{
			pWorker->OnOut(szB);
			//pWorker->s_ += (char*)szB;
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

	static RunProcessInfo RunCommandGetResultImple(
		LPCWSTR pExe,
		LPCWSTR pArg,
		DWORD* pIRetCommand,
		DWORD* pdwLastError,
		std::string* pStrOutCommand,
		std::string* pStrErrCommand,
		std::function<BOOL(STARTUPINFOW*, SECURITY_ATTRIBUTES*, DWORD* pdwCreationFlags)> fnBeforeCreateProcess,
		std::function<void(PROCESS_INFORMATION* ppi)> fnAfterCreateProcess,
		std::function<void(const char*, void* pUserData)> fnOnOut,
		void* pOutUserData,
		std::function<void(const char*, void* pUserData)> fnOnErr,
		void* pErrUserData)
	{
		RunProcessInfo retInfo;
		HANDLE hPipeStdInRead = NULL;
		HANDLE hPipeStdInWrite = NULL;
		if (!CreateHandles(hPipeStdInRead, hPipeStdInWrite, TRUE, FALSE, pdwLastError))
			return retInfo;
		HandleFreer h1(&hPipeStdInRead);
		HandleFreer h2(&hPipeStdInWrite);

		HANDLE hPipeStdOutRead = nullptr;
		HANDLE hPipeStdOutWrite = nullptr;
		if (!CreateHandles(hPipeStdOutRead, hPipeStdOutWrite, FALSE, TRUE, pdwLastError))
			return retInfo;
		HandleFreer h3(&hPipeStdOutRead);
		HandleFreer h4(&hPipeStdOutWrite);

		HANDLE hPipeStdErrRead = nullptr;
		HANDLE hPipeStdErrWrite = nullptr;
		if (!CreateHandles(hPipeStdErrRead, hPipeStdErrWrite, FALSE, TRUE, pdwLastError))
			return retInfo;
		HandleFreer h5(&hPipeStdErrRead);
		HandleFreer h6(&hPipeStdErrWrite);

		// createprocess
		// LPWSTR pArgCP = pArg ? _wcsdup(pArg) : NULL;
		// STLSOFT_SCODEDFREE_CRT(pArgCP);
		wstring argCP = pArg ? pArg : wstring();
		

		STARTUPINFOW siStartInfo = {};
		siStartInfo.cb = sizeof(STARTUPINFO);
		siStartInfo.hStdInput = hPipeStdInRead;
		siStartInfo.hStdOutput = hPipeStdOutWrite;
		siStartInfo.hStdError = hPipeStdErrWrite;
		siStartInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		siStartInfo.wShowWindow = SW_HIDE;

		SECURITY_ATTRIBUTES sa = {};
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
		PROCESS_INFORMATION pi = {};
		DWORD dwCreationFlags = 0;
		if (fnBeforeCreateProcess)
			if (!fnBeforeCreateProcess(&siStartInfo, &sa, &dwCreationFlags))
				return retInfo;
		if(!CreateProcessW(
			NULL,
			pCmd.get(),
			&sa, // sec
			nullptr, // sec thread
			TRUE, // inherit
			dwCreationFlags, // flags
			nullptr, // env
			nullptr, // dir
			&siStartInfo,
			&pi))
		{
			if (pdwLastError)
				*pdwLastError = GetLastError();
			return retInfo;
		}
		if (pdwLastError)
			*pdwLastError = GetLastError();
		
		retInfo.set(pi);

		if (fnAfterCreateProcess)
			fnAfterCreateProcess(&pi);

		// ~RunProcessInfo() will close these
		//HandleFreer h7(&pi.hProcess);
		//HandleFreer h8(&pi.hThread);

		ClearHandle(hPipeStdInRead);
		ClearHandle(hPipeStdOutWrite);
		ClearHandle(hPipeStdErrWrite);

		WorkerStruct wsOut(hPipeStdOutRead, pStrOutCommand, fnOnOut, pOutUserData);
		HANDLE hWorkerOut = (HANDLE)_beginthreadex(NULL,
			0,
			workerThread,
			&wsOut,
			CREATE_SUSPENDED,
			NULL);
		HandleFreer hWorkerOut_Free(&hWorkerOut);

		WorkerStruct wsErr(hPipeStdErrRead, pStrErrCommand, fnOnErr, pErrUserData);
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

		//if (pStrOutCommand)
		//	*pStrOutCommand = wsOut.s_;
		//if (pStrErrCommand)
		//	*pStrErrCommand = wsErr.s_;

		if (pIRetCommand)
			GetExitCodeProcess(pi.hProcess, pIRetCommand);

		return retInfo;
	}

	RunProcessInfo RunCommandGetResultCallBack(
		LPCSTR pExe,
		LPCSTR pArg,
		DWORD* pIRetCommand,
		DWORD* pdwLastError,
		std::function<BOOL(STARTUPINFOW*, SECURITY_ATTRIBUTES*, DWORD* pdwCreationFlags)> fnBeforeCreateProcess,
		std::function<void(PROCESS_INFORMATION* ppi)> fnAfterCreateProcess,
		std::function<void(const char*, void* pUserData)> fnOnOut,
		void* pOutUserData,
		std::function<void(const char*, void* pUserData)> fnOnErr,
		void* pErrUserData)
	{
		return RunCommandGetResultCallBack(
			toStdWstringFromUtf8(pExe).c_str(),
			toStdWstringFromUtf8(pArg).c_str(),
			pIRetCommand,
			pdwLastError,
			fnBeforeCreateProcess,
			fnAfterCreateProcess,
			fnOnOut,
			pOutUserData,
			fnOnErr,
			pErrUserData);
	}
	RunProcessInfo RunCommandGetResultCallBack(
		LPCWSTR pExe,
		LPCWSTR pArg,
		DWORD* pIRetCommand,
		DWORD* pdwLastError,
		std::function<BOOL(STARTUPINFOW*, SECURITY_ATTRIBUTES*, DWORD* pdwCreationFlags)> fnBeforeCreateProcess,
		std::function<void(PROCESS_INFORMATION* ppi)> fnAfterCreateProcess,
		std::function<void(const char*, void* pUserData)> fnOnOut,
		void* pOutUserData,
		std::function<void(const char*, void* pUserData)> fnOnErr,
		void* pErrUserData)
	{
		return RunCommandGetResultImple(
			pExe,
			pArg,
			pIRetCommand,
			pdwLastError,
			nullptr,
			nullptr,
			fnBeforeCreateProcess,
			fnAfterCreateProcess,
			fnOnOut,
			pOutUserData,
			fnOnErr,
			pErrUserData);
	}
	RunProcessInfo RunCommandGetResult(
		LPCWSTR pExe,
		LPCWSTR pArg,
		DWORD* pIRetCommand,
		DWORD* pdwLastError,
		std::string* pStrOutCommand,
		std::string* pStrErrCommand)
	{
		return RunCommandGetResultImple(
			pExe,
			pArg,
			pIRetCommand,
			pdwLastError,
			pStrOutCommand,
			pStrErrCommand,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr);
	}
	RunProcessInfo RunCommandGetResult(
		LPCSTR pExe,
		LPCSTR pArg,
		DWORD* pIRetCommand,
		DWORD* pdwLastError,
		std::string* pStrOutCommand,
		std::string* pStrErrCommand)
	{
		return RunCommandGetResult(
			toStdWstringFromUtf8(pExe).c_str(),
			toStdWstringFromUtf8(pArg).c_str(),
			pIRetCommand,
			pdwLastError,
			pStrOutCommand,
			pStrErrCommand
		);
	}
}