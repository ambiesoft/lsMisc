#include "stdafx.h"

#include <windows.h>
#include <Psapi.h>
#include <string>

#include "CHandle.h"
#include "GetFullFileNameFromPID.h"

using namespace std;

namespace Ambiesoft {
	// https://stackoverflow.com/a/1933140
	std::wstring GetFullFileNameFromPID(DWORD pid, DWORD* pLastError)
	{
		WCHAR filename[MAX_PATH];
		CHandle processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
		if (processHandle != NULL) {
			if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) == 0) {
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