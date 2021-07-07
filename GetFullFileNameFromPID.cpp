#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE


#include <windows.h>
// #include <Psapi.h>
#include <string>

#include "CHandle.h"
#include "GetFullFileNameFromPID.h"

using namespace std;

namespace Ambiesoft {

	typedef DWORD
		(WINAPI *FNGetModuleFileNameExW)(
		HANDLE hProcess,
		HMODULE hModule,
		LPWSTR lpFilename,
		DWORD nSize
		);

	typedef CGetProcAddress<FNGetModuleFileNameExW> CGetModuleFileNameExW;

	CGetModuleFileNameExW getFNGetModuleFileNameExW()
	{
		{
			CGetModuleFileNameExW ret(L"psapi.dll", "GetModuleFileNameExW");
			if (ret)
				return ret;
		}
		{
			CGetModuleFileNameExW ret(L"kernel32.dll", "GetModuleFileNameExW");
			if (ret)
				return ret;
		}
		//FNGetModuleFileNameExW fnRet = nullptr;

		//CHModule h = LoadLibrary(L"psapi.dll");
		//if (h)
		//{
		//	fnRet = (FNGetModuleFileNameExW)GetProcAddress(h, "GetModuleFileNameExW");
		//}
		//if (fnRet)
		//	return fnRet;
		//h = LoadLibrary(L"kernel32.dll");
		//if (h)
		//{
		//	fnRet = (FNGetModuleFileNameExW)GetProcAddress(h, "GetModuleFileNameExW");
		//}
		//if (fnRet)
		//	return fnRet;

		return CGetModuleFileNameExW();
	}

	// https://stackoverflow.com/a/1933140
	std::wstring GetFullFileNameFromPID(DWORD pid, DWORD* pLastError)
	{
		typedef DWORD
			(WINAPI *FNGetModuleFileNameExW)(
			HANDLE hProcess,
			HMODULE hModule,
			LPWSTR lpFilename,
			DWORD nSize
			);
		static CGetModuleFileNameExW fnGetModuleFileNameExW = getFNGetModuleFileNameExW();
		if (!fnGetModuleFileNameExW)
			return wstring();

		WCHAR filename[MAX_PATH];
		CHandle processHandle(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid));
		if (processHandle != NULL) {
			if (fnGetModuleFileNameExW.GetProc()(processHandle, NULL, filename, MAX_PATH) == 0) {
				if (pLastError)
					*pLastError = GetLastError();
				return wstring();
			}
			else {
				return filename;
			}
		}
		else {
			if (pLastError)
				*pLastError = GetLastError();
			return wstring();
		}
	}
}