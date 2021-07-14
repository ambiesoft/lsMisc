

#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif


#include <windows.h>
#include <tchar.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <Shlwapi.h>
#include <Winternl.h>
#include <WinIoCtl.h>

#include <string>
#include <algorithm>
#include <sstream>
#include <vector>

#include "OpenedFiles.h"
// #include "stlScopedClear.h"
#include "CHandle.h"
#include "SystemInfo.h"

#include "DebugNew.h"

#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"Psapi.lib")

using namespace std;
using namespace zoltan_csizmadia;

#pragma warning(disable:4996)

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

#define IOCTL_LISTDRV_BUFFERED_IO		\
        CTL_CODE(FILE_DEVICE_UNKNOWN,	\
        0x802,							\
        METHOD_BUFFERED,	            \
        FILE_READ_DATA | FILE_WRITE_DATA)

//typedef enum _SYSTEM_INFORMATION_CLASS {
//	SystemHandleInformation = 0X10,
//} SYSTEM_INFORMATION_CLASS;

#define SystemHandleInformationDefine ((SYSTEM_INFORMATION_CLASS)0x10)

typedef struct _HANDLE_INFO
{
	USHORT tcDeviceName[260];
	USHORT tcFileName[260];
	ULONG uType;
}HANDLE_INFO;

typedef struct _ADDRESS_INFO
{
	PVOID pAddress;
}ADDRESS_INFO;

typedef DWORD(WINAPI* GetFinalPathNameByHandleDef)(
	HANDLE hFile,
	LPWSTR lpszFilePath,
	DWORD cchFilePath,
	DWORD dwFlags);

typedef NTSTATUS (WINAPI *PNtQuerySystemInformation)(
	IN	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	OUT	PVOID					 SystemInformation,
	IN	ULONG					 SystemInformationLength,
	OUT	PULONG					 ReturnLength OPTIONAL
	);

void EnumerateLoadedModules(
	wstring& csPath,
	OF_CALLBACK CallBackProc,
	UINT_PTR pUserContext);

void EnumerateOpenedFiles(
	wstring& csPath,
	OF_CALLBACK CallBackProc,
	UINT_PTR pUserContext,
	HANDLE hDriver,
	GetFinalPathNameByHandleDef pGetFinalPathNameByHandle);

enum {
	VISTA_FILETYPE = 25,
	XP_FILETYPE = 28,
};
typedef struct _SYSTEM_HANDLE
{
	DWORD       dwProcessId;
	BYTE		bObjectType;
	BYTE		bFlags;
	WORD		wValue;
	PVOID       pAddress;
	DWORD GrantedAccess;
}SYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	DWORD         dwCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION, **PPSYSTEM_HANDLE_INFORMATION;

struct PROCESS_INFO_t
{
	wstring csProcess;
	DWORD dwImageListIndex;
};

static void MakeLower(wstring& s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::towlower);
}
static BOOL EnableTokenPrivilege(LPCTSTR pszPrivilege)
{
	// do it only once
	static bool bEnabled = false;
	if (bEnabled)
	{
		return TRUE;
	}
	bEnabled = true;

	HANDLE hToken = 0;
	TOKEN_PRIVILEGES tkp = { 0 };

	// Get a token for this process.
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES |
		TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}

	// Get the LUID for the privilege. 
	if (LookupPrivilegeValue(NULL, pszPrivilege,
		&tkp.Privileges[0].Luid))
	{
		tkp.PrivilegeCount = 1;  // one privilege to set    
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		// Set the privilege for this process. 
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
			(PTOKEN_PRIVILEGES)NULL, 0);

		if (GetLastError() != ERROR_SUCCESS)
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

static bool replace(std::wstring& str, const std::wstring& from, const std::wstring& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::wstring::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

// Function resolves the fosedevice name to drive name
static BOOL GetDrive(LPCTSTR pszDosName, wstring& csDrive, bool bDriveLetterOnly)
{
	TCHAR tcDeviceName[50];
	TCHAR tcDrive[3] = _T("A:");

	// Iterating through the drive letters
	for (TCHAR actDrive = _T('A'); actDrive <= _T('Z'); actDrive++)
	{
		tcDrive[0] = actDrive;
		// Query the device for the drive letter
		if (QueryDosDevice(tcDrive, tcDeviceName, 50) != 0)
		{
			// Is this the drive letter we are looking for?
			if (_tcsnicmp(tcDeviceName, pszDosName, _tcslen(tcDeviceName)) == 0)
			{
				if (bDriveLetterOnly)
				{
					csDrive = tcDrive;
				}
				else
				{
					csDrive = pszDosName;
					// csDrive.Replace(tcDeviceName, tcDrive);
					replace(csDrive, tcDeviceName, tcDrive);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}


void GetOpenedFiles(LPCWSTR lpPath, OF_TYPE Filter, OF_CALLBACK CallBackProc,
	UINT_PTR pUserContext)
{
	wstring csPath = lpPath;
	std::transform(csPath.begin(), csPath.end(), csPath.begin(), ::towlower);


	EnableTokenPrivilege(SE_DEBUG_NAME);
	if (Filter& MODULES_ONLY)
	{
		EnumerateLoadedModules(csPath, CallBackProc, pUserContext);
	}

	if (Filter& FILES_ONLY)
	{
		// Extract the driver from the resource and install it.
		// HANDLE hDriver = ExtractAndInstallDrv();
		HANDLE hDriver = NULL;
		GetFinalPathNameByHandleDef pGetFinalPathNameByHandle = 0;
		if (!hDriver)
		{
			pGetFinalPathNameByHandle = (GetFinalPathNameByHandleDef)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetFinalPathNameByHandleW");
		}
		// Now walk all handles
		EnumerateOpenedFiles(csPath, CallBackProc, pUserContext, hDriver, pGetFinalPathNameByHandle);
		if (hDriver)
		{	// Time to wind up		
			// StopAndUninstallDrv( hDriver );
		}
	}
}

UINT g_CurrentIndex = 0;
struct THREAD_PARAMS
{
	PSYSTEM_HANDLE_INFORMATION pSysHandleInformation;
	GetFinalPathNameByHandleDef pGetFinalPathNameByHandle;
	LPTSTR lpPath;
	int nFileType;
	HANDLE hStartEvent;
	HANDLE hFinishedEvent;
	bool bStatus;
};

DWORD WINAPI ThreadProc(LPVOID lParam)
{
	THREAD_PARAMS* pThreadParam = (THREAD_PARAMS*)lParam;

	GetFinalPathNameByHandleDef pGetFinalPathNameByHandle = pThreadParam->pGetFinalPathNameByHandle;
	for (g_CurrentIndex; g_CurrentIndex < pThreadParam->pSysHandleInformation->dwCount;)
	{

		WaitForSingleObject(pThreadParam->hStartEvent, INFINITE);
		ResetEvent(pThreadParam->hStartEvent);
		pThreadParam->bStatus = false;
		SYSTEM_HANDLE& sh = pThreadParam->pSysHandleInformation->Handles[g_CurrentIndex];
		g_CurrentIndex++;
		HANDLE hDup = (HANDLE)sh.wValue;
		HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, sh.dwProcessId);
		if (hProcess)
		{
			BOOL b = DuplicateHandle(hProcess, (HANDLE)sh.wValue, GetCurrentProcess(), &hDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
			if (!b)
			{
				hDup = (HANDLE)sh.wValue;
			}
			CloseHandle(hProcess);
		}
		DWORD dwRet = 0;
		if(pGetFinalPathNameByHandle)
			dwRet = pGetFinalPathNameByHandle(hDup, pThreadParam->lpPath, MAX_PATH, 0);
		else
		{
			wstring s = GetPathFromProcessID(sh.dwProcessId);
			if(s.empty())
			{
				dwRet=0;
			}
			else
			{
				lstrcpyn(pThreadParam->lpPath, s.c_str(), MAX_PATH);
				dwRet=1;
			}
		}

		if (hDup && (hDup != (HANDLE)sh.wValue))
		{
			CloseHandle(hDup);
		}
		if (dwRet)
		{
			pThreadParam->bStatus = true;
		}
		SetEvent(pThreadParam->hFinishedEvent);

	}
	return 0;
}

void EnumerateOpenedFiles(wstring& csPath, OF_CALLBACK CallBackProc, UINT_PTR pUserContext, HANDLE hDriver,
	GetFinalPathNameByHandleDef pGetFinalPathNameByHandle)
{
	int nFileType = XP_FILETYPE;
	OSVERSIONINFO info = { 0 };
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&info);
	if (info.dwMajorVersion == 6 &&
		info.dwMinorVersion == 0)
	{
		nFileType = VISTA_FILETYPE;
	}
	LPCTSTR lpPath = csPath.c_str();

	TCHAR csShortName[MAX_PATH] = { 0 };
	GetShortPathName(csPath.c_str(), csShortName, MAX_PATH);
	_tcslwr_s(csShortName);

	bool bShortPath = false;
	LPCTSTR lpShortPath = csShortName;
	if (csPath != csShortName && csShortName[0] != 0)
	{
		bShortPath = true;
	}

	HMODULE hModule = GetModuleHandle(_T("ntdll.dll"));
	PNtQuerySystemInformation NtQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (0 == NtQuerySystemInformation)
	{
		OutputDebugString(L"Getting proc of NtQuerySystemInformation failed");
		return;
	}


	//{
	//	ULONG needed=0;
	//	NTSTATUS status = NtQuerySystemInformation(SystemHandleInformation, NULL, 0, &needed);
	//	PSYSTEM_HANDLE_INFORMATION p = (PSYSTEM_HANDLE_INFORMATION)new BYTE[80000];

	//	status = NtQuerySystemInformation(SystemHandleInformation, p, needed, NULL);
	//	if(!NT_SUCCESS(status))
	//		return;
	//}

	// Get the list of all handles in the system
	PSYSTEM_HANDLE_INFORMATION pSysHandleInformation = new SYSTEM_HANDLE_INFORMATION;
	DWORD size = sizeof(SYSTEM_HANDLE_INFORMATION);
	DWORD needed = 0;
	NTSTATUS status = NtQuerySystemInformation(SystemHandleInformationDefine, pSysHandleInformation, size, &needed);
	if (!NT_SUCCESS(status))
	{
		if (0 == needed)
		{
			return;// some other error
		}
		// The previously supplied buffer wasn't enough.
		delete pSysHandleInformation;
		size = needed + 1024;
		pSysHandleInformation = (PSYSTEM_HANDLE_INFORMATION)new BYTE[size];
		status = NtQuerySystemInformation(SystemHandleInformationDefine, pSysHandleInformation, size, &needed);
		if (!NT_SUCCESS(status))
		{
			// some other error so quit.
			delete pSysHandleInformation;
			return;
		}

	}

	if (true)//pGetFinalPathNameByHandle)// there is no driver, we have do it ugly way
	{
		g_CurrentIndex = 0;
		TCHAR tcFileName[MAX_PATH + 1];
		THREAD_PARAMS ThreadParams;
		ThreadParams.lpPath = tcFileName;
		ThreadParams.nFileType = nFileType;
		ThreadParams.pGetFinalPathNameByHandle = pGetFinalPathNameByHandle;
		ThreadParams.pSysHandleInformation = pSysHandleInformation;
		ThreadParams.hStartEvent = ::CreateEvent(0, TRUE, FALSE, 0);
		ThreadParams.hFinishedEvent = ::CreateEvent(0, TRUE, FALSE, 0);
		HANDLE ThreadHandle = 0;
		while (g_CurrentIndex < pSysHandleInformation->dwCount)
		{
			if (!ThreadHandle)
			{
				ThreadHandle = CreateThread(0, 0, ThreadProc, &ThreadParams, 0, 0);
			}
			ResetEvent(ThreadParams.hFinishedEvent);
			SetEvent(ThreadParams.hStartEvent);
			if (WAIT_TIMEOUT == WaitForSingleObject(ThreadParams.hFinishedEvent, 100))
			{
				wstring csError;
				std::wostringstream wos;
				// csError.Format(L"Query hang for handle %d", (int)pSysHandleInformation->Handles[g_CurrentIndex - 1].wValue);
				wos << L"Query hang for handle " << (int)pSysHandleInformation->Handles[g_CurrentIndex - 1].wValue;
				csError = wos.str();

				OutputDebugString(csError.c_str());
				TerminateThread(ThreadHandle, 0);
				CloseHandle(ThreadHandle);
				ThreadHandle = 0;
				continue;
			}
			if (!ThreadParams.bStatus)
			{
				continue;
			}
			int nCmpStart = pGetFinalPathNameByHandle ? 4 : 0;
			wstring csFileName(&ThreadParams.lpPath[nCmpStart]);
			MakeLower(csFileName);
			if(!csPath.empty())
			{
				if (0 != _tcsncmp(lpPath, csFileName.c_str(), csPath.length()))
				{
					continue;
				}
			}
			OF_INFO_t stOFInfo;
			stOFInfo.dwPID = pSysHandleInformation->Handles[g_CurrentIndex - 1].dwProcessId;
			stOFInfo.lpFile = csFileName.c_str();
			stOFInfo.hFile = (HANDLE)pSysHandleInformation->Handles[g_CurrentIndex - 1].wValue;
			CallBackProc(&stOFInfo, pUserContext);
		}
		if (ThreadHandle)
		{
			if (WAIT_TIMEOUT == WaitForSingleObject(ThreadHandle, 1000))
			{
				TerminateThread(ThreadHandle, 0);
			}
			CloseHandle(ThreadHandle);
		}
		CloseHandle(ThreadParams.hStartEvent);
		CloseHandle(ThreadParams.hFinishedEvent);

		delete pSysHandleInformation;
		return;
	}

	// Walk through the handle list
	for (DWORD i = 0; i < pSysHandleInformation->dwCount; i++)
	{
		SYSTEM_HANDLE& sh = pSysHandleInformation->Handles[i];
		if (sh.bObjectType != nFileType)// Under windows XP file handle is of type 28
		{
			continue;
		}

		wstring csFileName;
		wstring csDir;
		if (hDriver)
		{
			HANDLE_INFO stHandle = { 0 };
			ADDRESS_INFO stAddress;
			stAddress.pAddress = sh.pAddress;
			DWORD dwReturn = 0;
			BOOL bSuccess = DeviceIoControl(
				hDriver,
				IOCTL_LISTDRV_BUFFERED_IO,
				&stAddress,
				sizeof(ADDRESS_INFO),
				&stHandle,
				sizeof(HANDLE_INFO),
				&dwReturn,
				NULL);


			if (bSuccess && stHandle.tcFileName[0] != 0 &&
				stHandle.uType != FILE_DEVICE_SOUND &&
				stHandle.uType != FILE_DEVICE_NAMED_PIPE)
			{

				if (stHandle.uType != FILE_DEVICE_NETWORK_FILE_SYSTEM)
				{
					// Get the drive name from the dos device name
					if (!GetDrive((LPCTSTR)stHandle.tcDeviceName, csFileName, true))
					{
						OutputDebugString(L"GetDrive failed");
					}
					csFileName += (LPCTSTR)stHandle.tcFileName;
				}
				else
				{
					csFileName = _T("\\");
					csFileName += (LPCTSTR)stHandle.tcFileName;
				}
			}
			else
			{
				continue;
			}
		}
		else
		{
			return;
		}


		MakeLower(csFileName);
		// Check whether the file belongs to the specified folder
		// 		if( -1 == csFileName.Find( csPath ))
		// 		{
		// 			if( bShortPath )
		// 			{
		// 				// Some times the file name may be in short path form.
		// 				if( -1 == csFileName.Find( csShortName ))
		// 				{
		// 					continue;
		// 				}
		// 			}
		// 			else
		// 			{
		// 				continue;
		// 			}
		// 		}

		if (0 != _tcsncmp(lpPath, csFileName.c_str(), csPath.length()))
		{
			if (bShortPath)
			{
				// Some times the file name may be in short path form.
				if (0 != _tcsncmp(lpShortPath, csFileName.c_str(), lstrlen(csShortName)))
				{
					continue;
				}
			}
			else
			{
				continue;
			}
		}
		OF_INFO_t stOFInfo;
		stOFInfo.dwPID = sh.dwProcessId;
		stOFInfo.lpFile = csFileName.c_str();
		stOFInfo.hFile = (HANDLE)sh.wValue;
		CallBackProc(&stOFInfo, pUserContext);
	}
	delete pSysHandleInformation;

}


void EnumerateLoadedModules(wstring& csPath, OF_CALLBACK CallBackProc, UINT_PTR pUserContext)
{
	TCHAR csShortName[MAX_PATH]; csShortName[0] = 0;
	GetShortPathName(csPath.c_str(), csShortName, MAX_PATH);
	_tcslwr_s(csShortName);

	bool bShortPath = false;
	if (csShortName != csPath && csShortName[0])
	{
		bShortPath = true;
	}

	DWORD dwsize = 300;
	PDWORD pDwId = (PDWORD)new BYTE[dwsize];
	DWORD dwRetruned = dwsize;
	// Enum all the process first
	while (1)
	{
		EnumProcesses(pDwId, dwsize, &dwRetruned);
		if (dwsize > dwRetruned)
		{
			break;
		}
		delete pDwId;
		dwsize += 50;
		pDwId = (PDWORD)new BYTE[dwsize];
	}
	int nCount = dwRetruned / sizeof(DWORD);
	int nItemCount = -1;
	// Enumerate modules of the above process
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		if (0 != pDwId[nIdx])
		{
			HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
			MODULEENTRY32 me32;
			// Take a snapshot of all modules in the specified process. 
			hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pDwId[nIdx]);
			if (hModuleSnap == INVALID_HANDLE_VALUE)
			{
				continue;
			}
			me32.dwSize = sizeof(MODULEENTRY32);
			if (!Module32First(hModuleSnap, &me32))
			{
				CloseHandle(hModuleSnap);
				continue;
			}
			bool bFirst = true;
			PROCESS_INFO_t stInfo;
			do
			{
				wstring csModule;
				if (bFirst)
				{
					// First module is always the exe name
					bFirst = false;
					if (!PathFileExists(me32.szExePath))
					{
						TCHAR tcFileName[MAX_PATH];
						HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, TRUE, pDwId[nIdx]);
						if (GetProcessImageFileName(hProcess, tcFileName, MAX_PATH))
						{
							GetDrive(tcFileName, csModule, false);
						}
						CloseHandle(hProcess);
					}
					else
					{
						csModule = me32.szExePath;
					}
					MakeLower(csModule);
				}
				else
				{
					csModule = me32.szExePath;
					MakeLower(csModule);
				}
				//if (-1 == csModule.Find(csPath))
				if (csModule.find(csPath) == std::wstring::npos)
				{
					if (bShortPath)
					{
						if (csModule.find(csShortName) == std::wstring::npos)
						{
							continue;
						}
					}
					else
					{
						continue;
					}
				}
				OF_INFO_t stOFInfo;
				stOFInfo.dwPID = pDwId[nIdx];
				stOFInfo.lpFile = csModule.c_str();
				CallBackProc(&stOFInfo, pUserContext);
			} while (Module32Next(hModuleSnap, &me32));
			CloseHandle(hModuleSnap);
		}
	}
	delete pDwId;
}















struct CallbackData
{
private:
	LPTSTR pFilter_;
	vector<OPENEDFILEINFO>* pv_;
	bool removePV_;

public:
	CallbackData() :pFilter_(NULL), pv_(NULL), removePV_(false) {}
	CallbackData(vector<OPENEDFILEINFO>* pv) : pFilter_(NULL), pv_(pv), removePV_(false) {}
	~CallbackData()
	{
		free(pFilter_);

		if (removePV_)
			delete pv_;
	}

	LPCTSTR getFilter() const {
		return pFilter_;
	}
	void setFilter(LPCTSTR pFilter)
	{
		pFilter_ = _tcsdup(pFilter);
		_tcslwr_s(pFilter_, lstrlen(pFilter) + 1);
	}

	void AddOpendFileInfo(OPENEDFILEINFO& info)
	{
		if (!pv_)
		{
			pv_ = new vector<OPENEDFILEINFO>;
			removePV_ = true;
		}

		pv_->push_back(info);
	}
};



void CALLBACK mycallback(OF_INFO_t* pOpenedFileInfo, UINT_PTR uUserContext)
{
	CallbackData* cd = (CallbackData*)uUserContext;

	TCHAR szT1[MAX_PATH];
	lstrcpy(szT1, pOpenedFileInfo->lpFile);
	_tcslwr_s(szT1);

	if (!_tcsstr(szT1, cd->getFilter()))
		return;

	OPENEDFILEINFO t;
	t.dwPID = pOpenedFileInfo->dwPID;
	lstrcpy(t.filename, pOpenedFileInfo->lpFile);
	t.hFile = pOpenedFileInfo->hFile;
	cd->AddOpendFileInfo(t);
}

static bool starts_with(const wstring& path, LPCTSTR pStart)
{
	if(!pStart || !pStart[0])
		return true;

	size_t startlen=lstrlen(pStart);
	if(path.length() < startlen)
		return false;

	for(size_t i=0; i<startlen; ++i)
	{
		if(towlower(path[i]) != towlower(pStart[i]))
			return false;
	}
	return true;
}

static std::wstring NtPathToWin32Path( std::wstring ntPath )
{
	static TCHAR szWindowsPath[MAX_PATH];
	if(szWindowsPath[0]==0)
	{
		GetWindowsDirectory(szWindowsPath,MAX_PATH);
		PathAddBackslash(szWindowsPath);
	}

    if (starts_with(ntPath, L"\\systemroot\\"))
    {
        ntPath.replace(ntPath.begin(), ntPath.begin() + 12, szWindowsPath);
        return ntPath;
    }
    if (starts_with(ntPath, L"\\\\?\\"))
    {
        ntPath.erase(ntPath.begin(), ntPath.begin() + 4);
        return ntPath;
    }
    if (starts_with(ntPath, L"\\??\\"))
    {
        ntPath.erase(ntPath.begin(), ntPath.begin() + 4);
    }
    if (starts_with(ntPath, L"\\"))
    {
        ntPath.erase(ntPath.begin(), ntPath.begin() + 1);
    }
    if (starts_with(ntPath, L"globalroot\\"))
    {
        ntPath.erase(ntPath.begin(), ntPath.begin() + 11);
    }
    if (starts_with(ntPath, L"systemroot"))
    {
        ntPath.replace(ntPath.begin(), ntPath.begin() + 10, szWindowsPath);
    }
    if (starts_with(ntPath, L"windows"))
    {
        ntPath.replace(ntPath.begin(), ntPath.begin() + 7, szWindowsPath);
    }
    return ntPath;
}

wstring GetPathFromProcessID(const DWORD dwID)
{
	HANDLE processHandle = NULL;
	TCHAR filename[MAX_PATH];

	wstring ret;

	processHandle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		dwID);
	if (processHandle != NULL)
	{
		if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0)
		{
			return ret;
		}
		else
		{
			ret = filename;
		}
		CloseHandle(processHandle);
	}
	return NtPathToWin32Path(ret);
}

void GetOpenedFilesSimple_obsolete(LPCTSTR pFilter, vector<OPENEDFILEINFO>& v)
{
	CallbackData cd(&v);
	cd.setFilter(pFilter);

	GetOpenedFiles(
		L"",
		ALL_TYPES,
		mycallback,
		(UINT_PTR)&cd);


}

// http://read.pudn.com/downloads36/sourcecode/windows/111597/NtSystemInfo/NtSystemInfoTest/NtSystemInfoTest.cpp__.htm
static LPCTSTR GetFileNamePosition(LPCTSTR lpPath)
{
	LPCTSTR lpAct = lpPath + _tcslen(lpPath);

	while (lpAct > lpPath && *lpAct != _T('\\') && *lpAct != _T('/'))
		lpAct--;

	if (lpAct > lpPath)
		lpAct++;

	return lpAct;
}
void WhoUsesFile(LPCTSTR lpFileName, BOOL bFullPathCheck, vector<OPENEDFILEINFO>& vResult)
{
	BOOL bShow = FALSE;
	wstring name;
	wstring processName;
	wstring deviceFileName;
	wstring fsFilePath;
	SystemProcessInformation::SYSTEM_PROCESS_INFORMATION* p;
	zoltan_csizmadia::SystemProcessInformation pi;
	zoltan_csizmadia::SystemHandleInformation hi;

	if (!SystemInfoUtils::GetDeviceFileName(lpFileName, deviceFileName))
	{
		// _tprintf(_T("GetDeviceFileName() failed.\n"));
		return;
	}


	hi.SetFilter(_T("File"), TRUE);

	// if (hi.m_HandleInfos.GetHeadPosition() == NULL)
	if (hi.m_HandleInfos.empty())
	{
		// _tprintf(_T("No handle information\n"));
		return;
	}

	pi.Refresh();

	//_tprintf(_T("%-6s  %-20s  %s\n"), _T("PID"), _T("Name"), _T("Path"));
	//_tprintf(_T("------------------------------------------------------\n"));

	//for (POSITION pos = hi.m_HandleInfos.GetHeadPosition(); pos != NULL;)
	for (const auto& h : hi.m_HandleInfos)
	{
		//SystemHandleInformation::SYSTEM_HANDLE& h = hi.m_HandleInfos.GetNext(pos);

		// if (pi.m_ProcessInfos.Lookup(h.ProcessID, p))
		if (pi.m_ProcessInfos.find(h.ProcessID) != pi.m_ProcessInfos.end() )
		{
			p = pi.m_ProcessInfos[h.ProcessID];
			SystemInfoUtils::Unicode2wstring(&p->usName, processName);
		}
		else
			processName = L"";

		//NT4 Stupid thing if it is the services.exe and I call GetName :((
		if (INtDll::dwNTMajorVersion == 4 && _tcsicmp(processName.c_str(), _T("services.exe")) == 0)
			continue;

		hi.GetName((HANDLE)h.HandleNumber, name, h.ProcessID);

		if (bFullPathCheck)
			bShow = _tcsicmp(name.c_str(), deviceFileName.c_str()) == 0;
		else
		{
			// bShow = _tcsicmp(GetFileNamePosition(name.c_str()), lpFileName) == 0;
			bShow = wcsstr(name.c_str(), deviceFileName.c_str()) == name.c_str();
		}

		if (bShow)
		{
			if (!bFullPathCheck)
			{
				fsFilePath = L"";
				SystemInfoUtils::GetFsFileName(name.c_str(), fsFilePath);
			}

			//_tprintf(_T("0x%04X  %-20s  %s\n"),
			//	h.ProcessID,
			//	processName,
			//	!bFullPathCheck ? fsFilePath : lpFileName);
			OPENEDFILEINFO ofi;
			ofi.dwPID = h.ProcessID;
			lstrcpy(ofi.filename, !bFullPathCheck ? fsFilePath.c_str() : lpFileName);
			ofi.hFile = nullptr;

			vResult.push_back(ofi);
		}
	}
}
bool EnableDebugPriv(void)
{
	Ambiesoft::CHandle token;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	// enable the SeDebugPrivilege
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
	{
		// _tprintf(_T("OpenProcessToken() failed, Error = %d SeDebugPrivilege is not available.\n"), GetLastError());
		return false;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		// _tprintf(_T("LookupPrivilegeValue() failed, Error = %d SeDebugPrivilege is not available.\n"), GetLastError());
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(token, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		// _tprintf(_T("AdjustTokenPrivileges() failed, Error = %d SeDebugPrivilege is not available.\n"), GetLastError());
		return false;
	}

	return true;
}
bool GetOpenedFilesSimple(LPCTSTR pFilter, vector<OPENEDFILEINFO>& v)
{
	WhoUsesFile(pFilter, FALSE, v);
	return true;
}