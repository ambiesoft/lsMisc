#include "stdafx.h"

namespace Ambiesoft {

	// https://stackoverflow.com/a/39290139
	HWND FindTopWindowFromPID(DWORD pid)
	{
		std::pair<HWND, DWORD> params = { 0, pid };

		// Enumerate the windows using a lambda to process each window
		BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
			{
				auto pParams = (std::pair<HWND, DWORD>*)(lParam);

				DWORD processId;
				if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams->second)
				{
					// Stop enumerating
					SetLastError(-1);
					pParams->first = hwnd;
					return FALSE;
				}

				// Continue enumerating
				return TRUE;
			}, (LPARAM)& params);

		if (!bResult && GetLastError() == -1 && params.first)
		{
			return params.first;
		}

		return 0;
	}
}