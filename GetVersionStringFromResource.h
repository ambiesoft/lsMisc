#pragma once

#include <windows.h>

#include <string>

namespace Ambiesoft {
	//
	//	Get the specified file-version information string from a file
	//	
	//	szItem	- version item string, e.g:
	//		"FileDescription", "FileVersion", "InternalName", 
	//		"ProductName", "ProductVersion", etc  (see MSDN for others)
	//
	// TCHAR *GetVersionStringFromResource(TCHAR *szFileName, TCHAR *szValue, TCHAR *szBuffer, ULONG nLength);
	std::wstring GetVersionStringFromResource(LPCTSTR szFileName, int keta = -1);
}