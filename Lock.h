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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace Ambiesoft {
	class CLock
	{
	public:
		CLock(LPCRITICAL_SECTION pSection) {
			assert(pSection);
			m_pSection = pSection;
			EnterCriticalSection(pSection);
		}

		~CLock() {
			LeaveCriticalSection(m_pSection);
		}
	protected:
		LPCRITICAL_SECTION m_pSection;
	};

	// How to use
	//{
	//	CReadWriteLock<CriticalSectionTrait> lock;
	//	CReadWriteLock<CriticalSectionTrait>::CReadWriteLocker locker(lock);
	//}
	//{
	//	CReadWriteLock<SRWTrait> lock;
	//	CReadWriteLock<SRWTrait>::CReadWriteLocker locker(lock);
	//}

	// DynamicTrait use new lock if availble,
	// safe for XP
	//{
	//	CReadWriteLock<DynamicTrait> lock;
	//	CReadWriteLock<DynamicTrait>::CReadWriteLocker locker(lock);
	//}
	//{
	//	using MyLock = CReadWriteLock<DynamicTrait>;
	//	MyLock lock;
	//	MyLock::CReadWriteLocker lockerR1(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
	//	MyLock::CReadWriteLocker lockerR2(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
	//	// MyLock::CReadWriteLocker lockerW1(lock, MyLock::CReadWriteLocker::LockType::READWRITE_LOCK);
	//}

	struct CriticalSectionTrait
	{
		typedef CRITICAL_SECTION LockObject;

		static void Initialize(LockObject& lock)
		{
			InitializeCriticalSectionAndSpinCount(&lock, 2000);
		}
		static void Close(LockObject& lock)
		{
			DeleteCriticalSection(&lock);
		}

		static void AcquireRead(LockObject& lock)
		{
			EnterCriticalSection(&lock);
		}
		static void ReleaseRead(LockObject& lock)
		{
			LeaveCriticalSection(&lock);
		}

		static void AcquireWrite(LockObject& lock)
		{
			EnterCriticalSection(&lock);
		}
		static void ReleaseWrite(LockObject& lock)
		{
			LeaveCriticalSection(&lock);
		}

	};
	struct SRWTrait
	{
		typedef SRWLOCK LockObject;
		static void Initialize(LockObject& lock)
		{
			InitializeSRWLock(&lock);
		}
		static void Close(LockObject& lock)
		{
			// No need
		}
		
		static void AcquireRead(LockObject& lock)
		{
			AcquireSRWLockShared(&lock);
		}
		static void ReleaseRead(LockObject& lock)
		{
			ReleaseSRWLockShared(&lock);
		}

		static void AcquireWrite(LockObject& lock)
		{
			AcquireSRWLockExclusive(&lock);
		}
		static void ReleaseWrite(LockObject& lock)
		{
			ReleaseSRWLockExclusive(&lock);
		}
	};

	

	struct DynamicTrait
	{
		typedef union xxxxx {
			CRITICAL_SECTION c;
			SRWLOCK l;
		} LockObject;

		static bool init_;
		static bool useNew_;
		static void init();

		typedef VOID(WINAPI *FNInitializeSRWLock)(PSRWLOCK SRWLock);
		static FNInitializeSRWLock fnInitializeSRWLock_;

		typedef VOID(WINAPI *FNAcquireSRWLockShared)(PSRWLOCK SRWLoc);
		static FNAcquireSRWLockShared fnAcquireSRWLockShared_;

		typedef VOID(WINAPI *FNReleaseSRWLockShared)(PSRWLOCK SRWLock);
		static FNReleaseSRWLockShared fnReleaseSRWLockShared_;


		typedef VOID(WINAPI* FNAcquireSRWLockExclusive)(PSRWLOCK SRWLock);
		static FNAcquireSRWLockExclusive fnAcquireSRWLockExclusive_;

		typedef VOID(WINAPI *FNReleaseSRWLockExclusive)(PSRWLOCK SRWLock);
		static FNReleaseSRWLockExclusive fnReleaseSRWLockExclusive_;

		static void Initialize(LockObject& lock);
		static void Close(LockObject& lock);

		static void AcquireRead(LockObject& lock);
		static void ReleaseRead(LockObject& lock);

		static void AcquireWrite(LockObject& lock);
		static void ReleaseWrite(LockObject& lock);
	};

	template<typename Trait>
	class CReadWriteLock
	{
		typename Trait::LockObject lock_;
	public:
		CReadWriteLock()
		{
			Trait::Initialize(lock_);
		}
		~CReadWriteLock()
		{
			Trait::Close(lock_);
		}

		void CReadWriteLock::acRead() {
			Trait::AcquireRead(lock_);
		}
		void CReadWriteLock::acReadWrite() {
			Trait::AcquireWrite(lock_);
		}
		void CReadWriteLock::rlRead() {
			Trait::ReleaseRead(lock_);
		}
		void CReadWriteLock::rlReadWrite() {
			Trait::ReleaseWrite(lock_);
		}

		class CReadWriteLocker
		{
			CReadWriteLock<Trait>& lock_;
		public:
			enum class LockType {
				READ_LOCK,
				READWRITE_LOCK,
			} lockType_;
			CReadWriteLocker(CReadWriteLock<Trait>& lock, LockType lt = LockType::READWRITE_LOCK) :
				lock_(lock), lockType_(lt) {
				if (lt == LockType::READ_LOCK)
					lock_.acRead();
				else if (lt == LockType::READWRITE_LOCK)
					lock_.acReadWrite();
				else
					assert(false);
			}
			~CReadWriteLocker()
			{
				if (lockType_ == LockType::READ_LOCK)
					lock_.rlRead();
				else if (lockType_ == LockType::READWRITE_LOCK)
					lock_.rlReadWrite();
				else
					assert(false);
			}
		};

	};



} // namespace Ambiesoft