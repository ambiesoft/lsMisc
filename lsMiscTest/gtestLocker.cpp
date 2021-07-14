#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <cassert>
#include <thread>

#include <gtest/gtest.h>

#include "../Lock.h"
#include "debug.h"

using namespace Ambiesoft;
using namespace std;

TEST(Locker, APITest)
{
	{
		CReadWriteLock<CriticalSectionTrait> lock;
		CReadWriteLock<CriticalSectionTrait>::CReadWriteLocker locker(lock);
	}
	{
		CReadWriteLock<SRWTrait> lock;
		CReadWriteLock<SRWTrait>::CReadWriteLocker locker(lock);
	}

	{
		CReadWriteLock<DynamicTrait> lock;
		CReadWriteLock<DynamicTrait>::CReadWriteLocker locker(lock);
	}
	{
		using MyLock = CReadWriteLock<DynamicTrait>;
		MyLock lock;
		MyLock::CReadWriteLocker lockerR1(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
		MyLock::CReadWriteLocker lockerR2(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
		// MyLock::CReadWriteLocker lockerW1(lock, MyLock::CReadWriteLocker::LockType::READWRITE_LOCK);
	}
}

using MyLock = CReadWriteLock<DynamicTrait>;
static void threadfunc_readlock(MyLock lock)
{
	MyLock::CReadWriteLocker locker(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
}
static void threadfunc_writelock(MyLock lock)
{
	MyLock::CReadWriteLocker locker(lock, MyLock::CReadWriteLocker::LockType::READWRITE_LOCK);
}
TEST(Locker, DeadLockTest)
{
	{
		MyLock lock;
		MyLock::CReadWriteLocker lockerR1(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
		MyLock::CReadWriteLocker lockerR2(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);

		// Must not lock
		thread th(threadfunc_readlock, lock);
		
		// vs2013 does not compile
		//this_thread::sleep_for(2s);
		Sleep(3000);

		// must have finished
		EXPECT_EQ(WAIT_OBJECT_0, WaitForSingleObject(th.native_handle(), 0));
		th.join();
	}
	{
		MyLock lock;
		MyLock::CReadWriteLocker lockerR1(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);
		MyLock::CReadWriteLocker lockerR2(lock, MyLock::CReadWriteLocker::LockType::READ_LOCK);

		// Must deadlock
		thread th(threadfunc_writelock, lock);
		
		Sleep(3000);

		// must not finished
		EXPECT_NE(WAIT_OBJECT_0, WaitForSingleObject(th.native_handle(), 0));

		TerminateThread(th.native_handle(), 255);
		th.join();
	}
}
