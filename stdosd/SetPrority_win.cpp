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

#include <Windows.h>
#include <winternl.h>

#include <cassert>

#include "SetPrority.h"

using namespace std;

namespace Ambiesoft {
	namespace stdosd {
		namespace Process {
			namespace {
				struct NativeHandle
				{
					DWORD dwLastError_;
					HANDLE h_;

					NativeHandle(HANDLE h) :
						dwLastError_(::GetLastError()),
						h_(h)
					{}
					~NativeHandle()
					{
						if (h_)
							CloseHandle(h_);
					}
					operator bool() const {
                        return h_ != nullptr;
					}
					DWORD lastError() const {
						return dwLastError_;
					}
					operator HANDLE() const {
						return h_;
					}
				};

				// http://blog.misterfoo.com/2010/07/process-priority-utility.html
				// these values determined by poking around in the debugger - use at your own risk!
#define ProcessInformationMemoryPriority			0x27
#define ProcessInformationIoPriority				0x21

#define CriticalIoPriority							4
#define HighIoPriority								3
#define	DefaultIoPriority							2
#define LowIoPriority								1
#define	VeryLowIoPriority							0


#define	DefaultMemoryPriority						5
#define MidumMemoryPriority							4
#define LowMemoryPriority							3
#define LowerMemoryPriority							2
#define LowestMemoryPriority						1
#define IdleMemoryPriority							0





				typedef NTSTATUS(NTAPI *FNNtSetInformationProcess)(
					HANDLE process,
					ULONG infoClass,
					void* data,
					ULONG dataSize);
				typedef NTSTATUS(NTAPI *FNNtQueryInformationProcess)(
					HANDLE               ProcessHandle,
					PROCESSINFOCLASS     ProcessInformationClass,
					PVOID                ProcessInformation,
					ULONG                ProcessInformationLength,
					PULONG ReturnLength OPTIONAL
					);
				struct NTFuncs
				{
				private:
                    FNNtQueryInformationProcess fnNtQueryInformationProcess = nullptr;
                    FNNtSetInformationProcess fnNtSetInformationProcess = nullptr;
                    HMODULE hDLL_ = nullptr;
					NTFuncs()
					{
						hDLL_ = LoadLibrary(L"ntdll.dll");
						if (hDLL_)
						{
                            fnNtQueryInformationProcess = (FNNtQueryInformationProcess)(void*)GetProcAddress(
								hDLL_,
                                "NtQueryInformationProcess");
                            fnNtSetInformationProcess = (FNNtSetInformationProcess)(void*)GetProcAddress(
								hDLL_,
								"NtSetInformationProcess");
						}
					}
					~NTFuncs()
					{
						if (hDLL_)
							FreeLibrary(hDLL_);
					}
					int InvalidReternValue(WORD w) {
						return MAKE_HRESULT(1, 832, w);
					}

				public:
					int GetPriority(HANDLE hProcess,
						CPUPRIORITY* cpuPriority,
						IOPRIORITY* ioPriority,
						MEMORYPRIORITY* memPriority)
					{
						int firsterr = 0;
						if (cpuPriority)
						{
							DWORD dwPriortyClass = GetPriorityClass(hProcess);
							if (dwPriortyClass == 0)
							{
								firsterr = static_cast<int>(GetLastError());
							}
							switch (dwPriortyClass)
							{
							case HIGH_PRIORITY_CLASS:			*cpuPriority = CPU_HIGH; break;
							case ABOVE_NORMAL_PRIORITY_CLASS:	*cpuPriority = CPU_ABOVENORMAL; break;
							case NORMAL_PRIORITY_CLASS:			*cpuPriority = CPU_NORMAL; break;
							case BELOW_NORMAL_PRIORITY_CLASS:	*cpuPriority = CPU_BELOWNORMAL; break;
							case IDLE_PRIORITY_CLASS:			*cpuPriority = CPU_IDLE; break;
							default:							*cpuPriority = CPU_UNKNOWN; break;
							}
						}

						{
							ULONG nativeIOPriority = -1;
							// TODO: implement
							if (ioPriority && fnNtQueryInformationProcess)
							{
								if (!fnNtQueryInformationProcess)
								{
									*ioPriority = IO_UNKNOWN;
								}
								else
								{
									NTSTATUS ntStatus = fnNtQueryInformationProcess(hProcess,
										(PROCESSINFOCLASS)ProcessInformationIoPriority,
										&nativeIOPriority,
										sizeof(nativeIOPriority),
										NULL);
									if (ntStatus != 0)
									{
										*ioPriority = IO_UNKNOWN;
										if (firsterr != 0)
											firsterr = ntStatus;
									}
									else
									{
										switch (nativeIOPriority)
										{
										case CriticalIoPriority:	*ioPriority = IO_HIGH; break;
										case HighIoPriority:		*ioPriority = IO_ABOVENORMAL; break;
										case DefaultIoPriority:		*ioPriority = IO_NORMAL; break;
										case LowIoPriority:			*ioPriority = IO_BELOWNORMAL; break;
										case VeryLowIoPriority:		*ioPriority = IO_IDLE; break;
										default:					*ioPriority = IO_UNKNOWN; break;
										}
									}
								}
							}
						}

						{
							ULONG nativeMemPriority = -1;
							// TODO: implement
							if (memPriority)
							{
								if (!fnNtQueryInformationProcess)
								{
									*memPriority = MEMORY_UNKNOWN;
								}
								else
								{
									NTSTATUS ntStatus = fnNtQueryInformationProcess(hProcess,
										(PROCESSINFOCLASS)ProcessInformationMemoryPriority,
										&nativeMemPriority,
										sizeof(nativeMemPriority),
										NULL);
									if (ntStatus != 0)
									{
										*memPriority = MEMORY_UNKNOWN;
										if (firsterr != 0)
											firsterr = ntStatus;
									}
									else
									{
										switch (nativeMemPriority)
										{
										case DefaultMemoryPriority:		*memPriority = MEMORY_HIGH; break;
										case MidumMemoryPriority:		*memPriority = MEMORY_ABOVENORMAL; break;
										case LowMemoryPriority:			*memPriority = MEMORY_NORMAL; break;
										case LowerMemoryPriority:		*memPriority = MEMORY_BELOWNORMAL; break;
										case LowestMemoryPriority:		*memPriority = MEMORY_IDLE; break;
										default:						*memPriority = MEMORY_UNKNOWN; break;
										}
									}
								}
							}
						}
						return firsterr;
					}
					int SetPriority(HANDLE hProcess,
						CPUPRIORITY cpuPriority,
						IOPRIORITY ioPriority,
						MEMORYPRIORITY memPriority)
					{
						DWORD dwProcessPriority = -1;
						switch (cpuPriority)
						{
						case CPU_NONE:break;
						case CPU_HIGH:			dwProcessPriority = HIGH_PRIORITY_CLASS; break;
						case CPU_ABOVENORMAL:	dwProcessPriority = ABOVE_NORMAL_PRIORITY_CLASS; break;
						case CPU_NORMAL:		dwProcessPriority = NORMAL_PRIORITY_CLASS; break;
						case CPU_BELOWNORMAL:	dwProcessPriority = BELOW_NORMAL_PRIORITY_CLASS; break;
						case CPU_IDLE:			dwProcessPriority = IDLE_PRIORITY_CLASS; break;
						default: assert(false); return InvalidReternValue(-1); break;
						}


						int firstError = 0;
						if (dwProcessPriority != (DWORD)-1)
						{
							if (!SetPriorityClass(hProcess, dwProcessPriority))
							{
								DWORD dwLastError = GetLastError();
								if (firstError == 0)
									firstError = static_cast<int>(dwLastError);
							}
						}

						{
							ULONG nativeIOPriority = -1;
							switch (ioPriority)
							{
							case IO_NONE:break;
							case IO_HIGH:			nativeIOPriority = CriticalIoPriority; break;
							case IO_ABOVENORMAL:	nativeIOPriority = HighIoPriority; break;
							case IO_NORMAL:			nativeIOPriority = DefaultIoPriority; break;
							case IO_BELOWNORMAL:	nativeIOPriority = LowIoPriority; break;
							case IO_IDLE:			nativeIOPriority = VeryLowIoPriority; break;
							default: assert(false); return InvalidReternValue(1); break;
							}

							if (nativeIOPriority != (ULONG)-1)
							{
								if (!fnNtSetInformationProcess)
								{
									if (firstError == 0)
										firstError = InvalidReternValue(2);
								}
								else
								{
									NTSTATUS ntStatus = fnNtSetInformationProcess(
										hProcess,
										ProcessInformationIoPriority,
										&nativeIOPriority,
										sizeof(nativeIOPriority));
									if (ntStatus != 0)
									{
										if (firstError == 0)
											firstError = static_cast<int>(ntStatus);
									}
								}
							}
						}

						{
							ULONG nativeMemoryPriority = -1;
							switch (memPriority)
							{
							case MEMORY_NONE:break;
							case MEMORY_HIGH:			nativeMemoryPriority = DefaultMemoryPriority; break;
							case MEMORY_ABOVENORMAL:	nativeMemoryPriority = MidumMemoryPriority; break;
							case MEMORY_NORMAL:			nativeMemoryPriority = LowMemoryPriority; break;
							case MEMORY_BELOWNORMAL:	nativeMemoryPriority = LowerMemoryPriority; break;
							case MEMORY_IDLE:			nativeMemoryPriority = LowestMemoryPriority; break;
							default: assert(false); return InvalidReternValue(-1); break;
							}

							if (nativeMemoryPriority != (ULONG)-1)
							{
								if (!fnNtSetInformationProcess)
								{
									if (firstError == 0)
										firstError = InvalidReternValue(2);
								}
								else
								{
									NTSTATUS ntStatus = fnNtSetInformationProcess(
										hProcess,
										ProcessInformationMemoryPriority,
										&nativeMemoryPriority,
										sizeof(nativeMemoryPriority));
									if (ntStatus != 0)
									{
										if (firstError == 0)
											firstError = static_cast<int>(ntStatus);
									}
								}
							}
						}
						return firstError;
					}

					static NTFuncs& GetInstance()
					{
						static NTFuncs theInst;
						return theInst;
					}
				};  // struct NTFuncs
			} // anonymous namespace

			int GetPriority(uint64_t  pid,
				CPUPRIORITY* cpuPriority,
				IOPRIORITY* ioPriority,
				MEMORYPRIORITY* memPriority)
			{
				NativeHandle handle(OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, (DWORD)pid));
				if (!handle)
				{
					// std::stringstream ss(error);
					// ss << "Failed to OpenProcess(" << pid << ")." << std::endl;
					return handle.lastError();
				}

				return NTFuncs::GetInstance().GetPriority(
					handle,
					cpuPriority,
					ioPriority,
					memPriority);

			}
			int SetProirity(uint64_t pid,
				CPUPRIORITY cpuPriority,
				IOPRIORITY ioPriority,
				MEMORYPRIORITY memPriority)
			{
				NativeHandle handle(OpenProcess(PROCESS_SET_INFORMATION, FALSE, (DWORD)pid));
				if (!handle)
				{
					// std::stringstream ss(error);
					// ss << "Failed to OpenProcess(" << pid << ")." << std::endl;
					return handle.lastError();
				}

				return NTFuncs::GetInstance().SetPriority(
					handle,
					cpuPriority,
					ioPriority,
					memPriority);
			}
		}
	}
} // namespace Ambiesoft
