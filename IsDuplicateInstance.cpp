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
#include <set>

#include "CHandle.h"
#include "IsDuplicateInstance.h"
#include "SessionGlobalMemory/SessionGlobalMemory.h"

#include "DebugNew.h"

namespace Ambiesoft {

	bool IsDuplicateInstance(LPCWSTR pMutexName)
	{
		static CKernelHandle hDupCheck;
		if (hDupCheck) {
			// Second call from same instance
			return false;
		}
		hDupCheck = CreateMutex(NULL, TRUE, pMutexName);
		return GetLastError() == ERROR_ALREADY_EXISTS;
	}

	
	static std::set<CSessionGlobalMemory<void*, wchar_t>> theInstanceKeeper;
	static void* getSessionData(LPCWSTR pName)
	{
		CSessionGlobalMemory<void*, wchar_t> sgData(pName);
		return sgData;
	}
	static void setSessionData(LPCWSTR pName, void* data)
	{
		CSessionGlobalMemory<void*, wchar_t> sgData(pName);
		sgData = data;
		theInstanceKeeper.insert(std::move(sgData));
	}
	void* GetDuplicateInstanceData(LPCWSTR pMutexName)
	{
		return getSessionData(pMutexName);
	}
	void SetDuplicateInstanceData(LPCWSTR pMutexName, void* data)
	{
		setSessionData(pMutexName, data);
		assert(GetDuplicateInstanceData(pMutexName) == data);
	}
} // namespace