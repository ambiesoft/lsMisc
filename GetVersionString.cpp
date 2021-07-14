





#include <string>
#include <memory>
#include "tchar.h"
#include "GetVersionString.h"

#pragma comment(lib, "Version.lib")
#pragma comment(lib, "User32.lib")

using namespace std;

namespace Ambiesoft {
	//
	//	Get the specified file-version information string from a file
	//	
	//	szItem	- version item string, e.g:
	//		"FileDescription", "FileVersion", "InternalName", 
	//		"ProductName", "ProductVersion", etc  (see MSDN for others)
	//
	TCHAR *GetVersionString_obsolete(TCHAR *szFileName, TCHAR *szValue, TCHAR *szBuffer, ULONG nLength)
	{
		DWORD  len;
		PVOID  ver;
		DWORD  *codepage;
		TCHAR   fmt[0x40];
		PVOID  ptr = 0;
		BOOL   result = FALSE;

		szBuffer[0] = '\0';

		len = GetFileVersionInfoSize(szFileName, 0);

		if (len == 0 || (ver = malloc(len)) == 0)
			return NULL;

		if (GetFileVersionInfo(szFileName, 0, len, ver))
		{
			if (VerQueryValue(ver, _T("\\VarFileInfo\\Translation"), (LPVOID*)&codepage, (PUINT)&len))
			{
				wsprintf(fmt, _T("\\StringFileInfo\\%04x%04x\\%s"), (*codepage) & 0xFFFF,
					(*codepage) >> 16, szValue);

				if (VerQueryValue(ver, fmt, &ptr, (PUINT)&len))
				{
					lstrcpyn(szBuffer, (TCHAR*)ptr, min(nLength, len));
					result = TRUE;
				}
			}
		}

		free(ver);
		return result ? szBuffer : NULL;
	}


	// https://stackoverflow.com/a/940743
	std::wstring GetVersionString(LPCTSTR szFileName, int keta)
	{
		DWORD  verHandle = 0;
		UINT   size = 0;
		LPBYTE lpBuffer = NULL;
		DWORD  verSize = GetFileVersionInfoSize(szFileName, &verHandle);

		if (verSize != 0)
		{
			// LPTSTR verData = new TCHAR[verSize];
			unique_ptr<TCHAR[]> verData(new TCHAR[verSize]);

			if (GetFileVersionInfo(szFileName, verHandle, verSize, verData.get()))
			{
				if (VerQueryValue(verData.get(), _T("\\"), (VOID FAR* FAR*)&lpBuffer, &size))
				{
					if (size)
					{
						VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
						if (verInfo->dwSignature == 0xfeef04bd)
						{
							TCHAR szBuff[256];
							// Doesn't matter if you are on 32 bit or 64 bit,
							// DWORD is always 32 bits, so first two revision numbers
							// come from dwFileVersionMS, last two come from dwFileVersionLS

							switch (keta)
							{
							case 1:
								wsprintf(szBuff, _T("%d"),
									(verInfo->dwFileVersionMS >> 16) & 0xffff
									);
								return szBuff;
							case 2:
								wsprintf(szBuff, _T("%d.%d"),
									(verInfo->dwFileVersionMS >> 16) & 0xffff,
									(verInfo->dwFileVersionMS >> 0) & 0xffff
									);
								return szBuff;
							case 3:
								wsprintf(szBuff, _T("%d.%d.%d"),
									(verInfo->dwFileVersionMS >> 16) & 0xffff,
									(verInfo->dwFileVersionMS >> 0) & 0xffff,
									(verInfo->dwFileVersionLS >> 16) & 0xffff
									);
								return szBuff;
							default:
								wsprintf(szBuff, _T("%d.%d.%d.%d"),
									(verInfo->dwFileVersionMS >> 16) & 0xffff,
									(verInfo->dwFileVersionMS >> 0) & 0xffff,
									(verInfo->dwFileVersionLS >> 16) & 0xffff,
									(verInfo->dwFileVersionLS >> 0) & 0xffff
									);
								return szBuff;
							}
						}
					}
				}
			}
		}
		return wstring();
	}
}