#include "stdafx.h"
#include <tchar.h>
#include <string>
#include <gtest/gtest.h>

#include "../stdosd/SetPrority.h"

using namespace std;
using namespace Ambiesoft::stdosd::Process;

TEST(SetPriority, Cpu)
{
	int err;
	DWORD id = GetCurrentProcessId();

	CPUPRIORITY cpuPriorityOrig = CPU_NONE;
	CPUPRIORITY cpuPriority = CPU_NONE;
	IOPRIORITY ioPriority;
	MEMORYPRIORITY memPriority;
	err = GetPriority(id,
		&cpuPriorityOrig,
		&ioPriority,
		&memPriority);
	EXPECT_EQ(err, 0);
	EXPECT_NE(cpuPriorityOrig, CPU_NONE);

	err = SetProirity(id,
		CPUPRIORITY::CPU_ABOVENORMAL,
		IOPRIORITY::IO_NONE,
		MEMORYPRIORITY::MEMORY_NONE);
	EXPECT_EQ(err, 0);
	err = GetPriority(id,
		&cpuPriority,
		&ioPriority,
		&memPriority);
	EXPECT_EQ(cpuPriority, CPU_ABOVENORMAL);

	err = SetProirity(id,
		cpuPriorityOrig,
		IOPRIORITY::IO_NONE,
		MEMORYPRIORITY::MEMORY_NONE);
	EXPECT_EQ(err, 0);
	err = GetPriority(id,
		&cpuPriority,
		&ioPriority,
		&memPriority);
	EXPECT_EQ(cpuPriority, cpuPriorityOrig);

}
TEST(SetPriority, IO)
{
	DWORD id = GetCurrentProcessId();
	int err;
	IOPRIORITY ioPriorityOrig;

	// Get Current IO Priority
	err = GetPriority(id,
		NULL,
		&ioPriorityOrig,
		NULL);
	EXPECT_EQ(err, 0);
	EXPECT_NE(ioPriorityOrig, IO_NONE);


	// Set IO as Idle and check
	err = SetProirity(id,
		CPUPRIORITY::CPU_NONE,
		IOPRIORITY::IO_IDLE,
		MEMORYPRIORITY::MEMORY_NONE);
	EXPECT_EQ(err, 0);

	IOPRIORITY ioPriority;
	err = GetPriority(id,
		NULL,
		&ioPriority,
		NULL);
	EXPECT_EQ(err, 0);
	EXPECT_EQ(ioPriority, IO_IDLE);

	
	// Set back IO andcheck
	err = SetProirity(id,
		CPUPRIORITY::CPU_NONE,
		ioPriorityOrig,
		MEMORYPRIORITY::MEMORY_NONE);
	EXPECT_EQ(err, 0);

	err = GetPriority(id,
		NULL,
		&ioPriority,
		NULL);
	EXPECT_EQ(err, 0);
	EXPECT_EQ(ioPriorityOrig, ioPriority);
}

TEST(SetPriority, Memory)
{
	DWORD id = GetCurrentProcessId();
	int err;
	MEMORYPRIORITY memPriorityOrig;

	// Get Current IO Priority
	err = GetPriority(id,
		NULL,
		NULL,
		&memPriorityOrig);
	EXPECT_EQ(err, 0);
	EXPECT_NE(memPriorityOrig, MEMORY_NONE);


	// Set Memory as Idle and check
	err = SetProirity(id,
		CPUPRIORITY::CPU_NONE,
		IOPRIORITY::IO_NONE,
		MEMORYPRIORITY::MEMORY_IDLE);
	EXPECT_EQ(err, 0);

	MEMORYPRIORITY memPriority;
	err = GetPriority(id,
		NULL,
		NULL,
		&memPriority);
	EXPECT_EQ(err, 0);
	EXPECT_EQ(memPriority, MEMORY_IDLE);


	// Set back IO andcheck
	err = SetProirity(id,
		CPUPRIORITY::CPU_NONE,
		IOPRIORITY::IO_NONE,
		memPriorityOrig);
	EXPECT_EQ(err, 0);

	err = GetPriority(id,
		NULL,
		NULL,
		&memPriority);

	EXPECT_EQ(err, 0);
	EXPECT_EQ(memPriorityOrig, memPriority);
}

TEST(SetPriority, SetInvalidValue)
{
	DWORD id = GetCurrentProcessId();
	int err = 0;
#ifndef NDEBUG
	// DEBUG
	EXPECT_DEATH(SetProirity(id,
		(CPUPRIORITY)55555,
		(IOPRIORITY)55555,
		(MEMORYPRIORITY)55555), "");

	// When process id invalid, it will not die in Debug mode
	err = SetProirity(0xFFFFFFFE,
		(CPUPRIORITY)55555,
		(IOPRIORITY)55555,
		(MEMORYPRIORITY)55555);
	EXPECT_NE(err, 0);
#else
	// Release
	err = SetProirity(id,
		(CPUPRIORITY)55555,
		(IOPRIORITY)55555,
		(MEMORYPRIORITY)55555);
	EXPECT_NE(err, 0);

	err = SetProirity(0xFFFFFFFE,
		(CPUPRIORITY)55555,
		(IOPRIORITY)55555,
		(MEMORYPRIORITY)55555);
	EXPECT_NE(err, 0);
#endif // DEBUG
}
TEST(SetPriority, GetAll)
{
	DWORD id = GetCurrentProcessId();
	int err;
	CPUPRIORITY cpuPriority = CPU_NONE;
	IOPRIORITY ioPriority = IO_NONE;
	MEMORYPRIORITY memPriority = MEMORY_NONE;

	// Get Current IO Priority
	err = GetPriority(id,
		&cpuPriority,
		&ioPriority,
		&memPriority);
	EXPECT_EQ(err, 0);
	EXPECT_NE(cpuPriority, CPU_NONE);
	EXPECT_NE(cpuPriority, CPU_UNKNOWN);
	EXPECT_NE(ioPriority, IO_NONE);
	EXPECT_NE(ioPriority, IO_UNKNOWN);
	EXPECT_NE(memPriority, MEMORY_NONE);
	EXPECT_NE(memPriority, MEMORY_UNKNOWN);
}