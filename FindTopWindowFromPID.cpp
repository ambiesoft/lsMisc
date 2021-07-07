#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE

#include <set>

#include <windows.h>

#include "FindTopWindowFromPID.h"

using namespace std;
namespace Ambiesoft {

	// https://stackoverflow.com/a/39290139
	set<HWND> FindTopWindowFromPID(DWORD pid)
	{
		using THETYPE = pair<set<HWND>, DWORD>;
		THETYPE rets;
		rets.second = pid;
		// Enumerate the windows using a lambda to process each window
		BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
			{
				THETYPE* pRets = (THETYPE*)(lParam);

				DWORD processId;
				if (GetWindowThreadProcessId(hwnd, &processId) && processId == pRets->second)
				{
					pRets->first.insert(hwnd);
				}

				// Continue enumerating
				return TRUE;
			}, (LPARAM)& rets);

		return rets.first;
	}
}
