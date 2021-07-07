#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE


#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

#include "GetFileNameFromHwnd.h"

// warning C4996: 'GetVersionExW': was declared deprecated
#pragma warning(disable: 4996)

namespace Ambiesoft {
	static BOOL(WINAPI *lpfEnumProcessModules)
		(HANDLE, HMODULE*, DWORD, LPDWORD);
	static DWORD(WINAPI *lpfGetModuleFileNameEx)
		(HANDLE, HMODULE, LPTSTR, DWORD);
	typedef BOOL(WINAPI *LPFEnumProcessModulesEx)
		(HANDLE, HMODULE*, DWORD, LPDWORD, DWORD);
	static LPFEnumProcessModulesEx lpfEnumProcessModulesEx;

	typedef BOOL(WINAPI *LPFQueryFullProcessImageNameW)(
		HANDLE hProcess,
		DWORD  dwFlags,
		LPTSTR lpExeName,
		PDWORD lpdwSize
		);
	static LPFQueryFullProcessImageNameW lpfQueryFullProcessImageNameW;

	static bool init()
	{
		static bool initted = false;
		static bool ok = false;
		if (initted)
			return ok;
		initted = true;

		static HINSTANCE sthPSAPI;
		if (!sthPSAPI)
			sthPSAPI = LoadLibrary(_T("PSAPI.DLL"));
		if (sthPSAPI == NULL)
			return ok = false;

		static HINSTANCE sthKernel;
		if (!sthKernel)
			sthKernel = LoadLibrary(_T("kernel32.dll"));
		if (sthKernel == NULL)
			return ok = false;

		lpfEnumProcessModulesEx = (LPFEnumProcessModulesEx)GetProcAddress(
			sthPSAPI, "EnumProcessModulesEx");

		lpfEnumProcessModules = (BOOL(WINAPI *)
			(HANDLE, HMODULE *, DWORD, LPDWORD))GetProcAddress(
				sthPSAPI, "EnumProcessModules");
		if (lpfEnumProcessModules == NULL)
			return ok = false;
#ifdef UNICODE
		lpfGetModuleFileNameEx = (DWORD(WINAPI *)
			(HANDLE, HMODULE, LPWSTR, DWORD))GetProcAddress(
				sthPSAPI, "GetModuleFileNameExW");
		if (lpfGetModuleFileNameEx == NULL)
			return ok = false;
#else
		lpfGetModuleFileNameEx = (DWORD(WINAPI *)
			(HANDLE, HMODULE, LPSTR, DWORD))GetProcAddress(
				sthPSAPI, "GetModuleFileNameExA");
		if (lpfGetModuleFileNameEx == NULL)
			return ok = false;
#endif

		lpfQueryFullProcessImageNameW = (LPFQueryFullProcessImageNameW)GetProcAddress(
			sthKernel, "QueryFullProcessImageNameW");
		return ok = true;
	}

#ifndef LIST_MODULES_ALL
#define LIST_MODULES_ALL 0x03
#endif

	static BOOL getModuleFromHProcess(HANDLE hProcess, HMODULE* phModule)
	{
		DWORD dwNeed = 0;
		if (lpfEnumProcessModulesEx)
			return lpfEnumProcessModulesEx(hProcess, phModule, sizeof(*phModule), &dwNeed, LIST_MODULES_ALL);

		return lpfEnumProcessModules(hProcess, phModule, sizeof(*phModule), &dwNeed);
	}

	BOOL GetFileNameFromHwnd(HWND hWnd, LPTSTR lpszFileName, DWORD nSize)
	{
		BOOL bResult = FALSE;

		DWORD dwProcessId;
		GetWindowThreadProcessId(hWnd, &dwProcessId);

		OSVERSIONINFO osverinfo;
		osverinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx(&osverinfo))
			return FALSE;


		if (osverinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			if (init())
			{
				HANDLE hProcess;
				hProcess = OpenProcess(
					PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
					FALSE, dwProcessId);
				if (hProcess)
				{
					if (lpfQueryFullProcessImageNameW)
					{
						DWORD dwSize = nSize;
						bResult = lpfQueryFullProcessImageNameW(hProcess,
							0,
							lpszFileName,
							&dwSize);
					}
					else
					{
						HMODULE hModule;
						if (getModuleFromHProcess(hProcess, &hModule))
						{
							if (lpfGetModuleFileNameEx(hProcess, hModule, lpszFileName, nSize))
								bResult = TRUE;
						}
					}
					CloseHandle(hProcess);
				}
			}
			//FreeLibrary( hInstLib ) ;
		}
#if !UNICODE
		else if (osverinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		{
			HANDLE(WINAPI *lpfCreateSnapshot)(DWORD, DWORD);
			BOOL(WINAPI *lpfProcess32First)(HANDLE, LPPROCESSENTRY32);
			BOOL(WINAPI *lpfProcess32Next)(HANDLE, LPPROCESSENTRY32);

			lpfCreateSnapshot =
				(HANDLE(WINAPI*)(DWORD, DWORD))GetProcAddress(
					GetModuleHandleA("kernel32.dll"),
					"CreateToolhelp32Snapshot");
			lpfProcess32First =
				(BOOL(WINAPI*)(HANDLE, LPPROCESSENTRY32))GetProcAddress(
					GetModuleHandleA("kernel32.dll"),
					"Process32First");
			lpfProcess32Next =
				(BOOL(WINAPI*)(HANDLE, LPPROCESSENTRY32))GetProcAddress(
					GetModuleHandleA("kernel32.dll"),
					"Process32Next");
			if (!lpfCreateSnapshot ||
				!lpfProcess32First ||
				!lpfProcess32Next)
				return FALSE;

			HANDLE hSnapshot;
			hSnapshot = lpfCreateSnapshot(TH32CS_SNAPPROCESS, 0);
			if (hSnapshot != (HANDLE)-1)
			{
				PROCESSENTRY32 pe;
				pe.dwSize = sizeof(PROCESSENTRY32);
				if (lpfProcess32First(hSnapshot, &pe))
				{
					do {
						if (pe.th32ProcessID == dwProcessId)
						{
							lstrcpyA(lpszFileName, pe.szExeFile);
							bResult = TRUE;
							break;
						}
					} while (lpfProcess32Next(hSnapshot, &pe));
				}

				CloseHandle(hSnapshot);
			}
		}
#endif

		return bResult;
	}
}