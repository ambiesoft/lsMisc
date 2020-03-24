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

#include "StdAfx.h"
#include <Windows.h>
#include "Lock.h"

namespace Ambiesoft {

	bool DynamicTrait::init_;
	bool DynamicTrait::useNew_;

	DynamicTrait::FNInitializeSRWLock DynamicTrait::fnInitializeSRWLock_;
	DynamicTrait::FNAcquireSRWLockShared DynamicTrait::fnAcquireSRWLockShared_;
	DynamicTrait::FNReleaseSRWLockShared DynamicTrait::fnReleaseSRWLockShared_;

	DynamicTrait::FNAcquireSRWLockExclusive DynamicTrait::fnAcquireSRWLockExclusive_;
	DynamicTrait::FNReleaseSRWLockExclusive DynamicTrait::fnReleaseSRWLockExclusive_;

	void DynamicTrait::init()
	{
		if (init_)
			return;
		init_ = true;

		HMODULE hKernel = LoadLibrary(L"kernel32.dll");
		if (!hKernel)
			return;
		
		fnInitializeSRWLock_ = (FNInitializeSRWLock)
			GetProcAddress(hKernel, "InitializeSRWLock");
		if (!fnInitializeSRWLock_)
			return;

		fnAcquireSRWLockShared_ = (FNAcquireSRWLockShared)
			GetProcAddress(hKernel, "AcquireSRWLockShared");
		if (!fnAcquireSRWLockShared_)
			return;

		fnReleaseSRWLockShared_ = (FNReleaseSRWLockShared)
			GetProcAddress(hKernel, "ReleaseSRWLockShared");
		if (!fnReleaseSRWLockShared_)
			return;

		fnAcquireSRWLockExclusive_ = (FNAcquireSRWLockExclusive)
			GetProcAddress(hKernel, "AcquireSRWLockExclusive");
		if (!fnAcquireSRWLockExclusive_)
			return;

		fnReleaseSRWLockExclusive_ = (FNReleaseSRWLockExclusive)
			GetProcAddress(hKernel, "ReleaseSRWLockExclusive");
		if (!fnReleaseSRWLockExclusive_)
			return;

		FreeLibrary(hKernel);
		useNew_ = true;
	}

	void DynamicTrait::Initialize(LockObject& lock)
	{
		if (!init_)
			init();

		if (useNew_)
			fnInitializeSRWLock_(&lock.l);
		else
			InitializeCriticalSectionAndSpinCount(&lock.c, 2000);
	}
	void DynamicTrait::Close(LockObject& lock)
	{
		assert(init_);
		if (!useNew_)
			DeleteCriticalSection(&lock.c);
	}

	void DynamicTrait::AcquireRead(LockObject& lock)
	{
		assert(init_);
		if (useNew_)
			fnAcquireSRWLockShared_(&lock.l);
		else
			EnterCriticalSection(&lock.c);
	}
	void DynamicTrait::ReleaseRead(LockObject& lock)
	{
		assert(init_);
		if (useNew_)
			fnReleaseSRWLockShared_(&lock.l);
		else
			LeaveCriticalSection(&lock.c);
	}


	void DynamicTrait::AcquireWrite(LockObject& lock)
	{
		assert(init_);
		if (useNew_)
			fnAcquireSRWLockExclusive_(&lock.l);
		else
			EnterCriticalSection(&lock.c);
	}
	void DynamicTrait::ReleaseWrite(LockObject& lock)
	{
		assert(init_);
		if (useNew_)
			fnReleaseSRWLockExclusive_(&lock.l);
		else
			LeaveCriticalSection(&lock.c);
	}


} // namespace Ambiesoft