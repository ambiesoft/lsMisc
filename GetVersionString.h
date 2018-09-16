#pragma once

#include <windows.h>
namespace Ambiesoft {
	//
	//	Get the specified file-version information string from a file
	//	
	//	szItem	- version item string, e.g:
	//		"FileDescription", "FileVersion", "InternalName", 
	//		"ProductName", "ProductVersion", etc  (see MSDN for others)
	//
	// TCHAR *GetVersionString(TCHAR *szFileName, TCHAR *szValue, TCHAR *szBuffer, ULONG nLength);
	std::wstring GetVersionString(LPCTSTR szFileName);
}