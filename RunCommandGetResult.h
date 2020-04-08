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

#include <functional>
#include <string>


namespace Ambiesoft {
	class RunProcessInfo {
		bool success_ = false;
		HANDLE hProcess_ = nullptr;
		DWORD pid_ = 0;
		HANDLE hThread_ = nullptr;
		DWORD tid_ = 0;
		RunProcessInfo(const RunProcessInfo& that) = delete;
		RunProcessInfo& operator=(const RunProcessInfo&) = delete;
	public:
		RunProcessInfo() {
		}
		RunProcessInfo(RunProcessInfo&& that)
		{
			success_ = that.success_;
			
			hProcess_ = that.hProcess_;
			that.hProcess_ = nullptr;
			
			pid_ = that.pid_;
			
			hThread_ = that.hThread_;
			that.hThread_ = nullptr;

			tid_ = that.pid_;
		}
		~RunProcessInfo() {
			if (success_)
			{
				if(hProcess_)
					CloseHandle(hProcess_);
				if(hThread_)
					CloseHandle(hThread_);
			}
		}
		void set(const PROCESS_INFORMATION& pi) {
			success_ = true;
			hProcess_ = pi.hProcess;
			pid_ = pi.dwProcessId;
			hThread_ = pi.hThread;
			tid_ = pi.dwThreadId;
		}
		HANDLE GetProcess() const {
			return hProcess_;
		}
		DWORD GetProcessID() const {
			return pid_;
		}
		HANDLE GetThread() const {
			return hThread_;
		}
		DWORD GetThreadID() const {
			return tid_;
		}
		operator bool() const {
			return success_;
		}
	};

	RunProcessInfo RunCommandGetResult(
		LPCSTR pExe,
		LPCSTR pArg,
		DWORD* pIRetCommand,
		DWORD* pdwLastError,
		std::string* pStrOutCommand,
		std::string* pStrErrCommand);

	RunProcessInfo RunCommandGetResult(
		LPCWSTR pExe,
		LPCWSTR pArg,
		DWORD* pIRetCommand,
		DWORD* pdwLastError,
		std::string* pStrOutCommand,
		std::string* pStrErrCommand);

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
		void* pErrUserData);
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
		void* pErrUserData);

}