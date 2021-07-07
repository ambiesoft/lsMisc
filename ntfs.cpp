

#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE


#include <windows.h>
#include <WinIoCtl.h>

#include <string>

#include <stlsoft/smartptr/scoped_handle.hpp>

using std::wstring;

namespace Ambiesoft{


	// http://blog.kalmbach-software.de/2008/02/
	typedef struct _REPARSE_DATA_BUFFER {
		ULONG  ReparseTag;
		USHORT  ReparseDataLength;
		USHORT  Reserved;
		union {
			struct {
				USHORT  SubstituteNameOffset;
				USHORT  SubstituteNameLength;
				USHORT  PrintNameOffset;
				USHORT  PrintNameLength;
				ULONG   Flags; // it seems that the docu is missing this entry (at least 2008-03-07)
				WCHAR  PathBuffer[1];
			} SymbolicLinkReparseBuffer;
			struct {
				USHORT  SubstituteNameOffset;
				USHORT  SubstituteNameLength;
				USHORT  PrintNameOffset;
				USHORT  PrintNameLength;
				WCHAR  PathBuffer[1];
			} MountPointReparseBuffer;
			struct {
				UCHAR  DataBuffer[1];
			} GenericReparseBuffer;
		};
	} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

#define REPARSE_DATA_BUFFER_HEADER_SIZE  FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)

#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE  ( 16 * 1024 )

	typedef DWORD(WINAPI* GetFinalPathNameByHandleDef)(
		HANDLE hFile,
		LPWSTR lpszFilePath,
		DWORD cchFilePath,
		DWORD dwFlags);

	static bool worker1(LPCWSTR pPath, wstring& ret)
	{
		HANDLE hFile = CreateFile(pPath,
			FILE_READ_EA,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, 
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		stlsoft::scoped_handle<HANDLE> ha(hFile, CloseHandle);

		static GetFinalPathNameByHandleDef pGetFinalPathNameByHandle = 
			(GetFinalPathNameByHandleDef)GetProcAddress(
				GetModuleHandle(_T("kernel32.dll")), "GetFinalPathNameByHandleW");
	
		if (pGetFinalPathNameByHandle)
		{
			TCHAR szT[MAX_PATH]; szT[0] = 0;
			pGetFinalPathNameByHandle(hFile,
				szT,
				_countof(szT),
				0);
			if (GetLastError() == NO_ERROR)
			{
				ret =szT;
				return true;
			}
		}
		return false;
	}
	static wstring ResolveDirectory(LPCWSTR pPath)
	{
		HANDLE hFile;
		// LPCTSTR szMyFile = _T("C:\\Documents and Settings");  // Mount-Point (JUNCTION)
		//LPCTSTR szMyFile = _T("C:\\Users\\All Users");  // Symbolic-Link (SYMLINKD)

		hFile = CreateFile(pPath,
			FILE_READ_EA,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, 
			NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return pPath;
		}
		
		// Allocate the reparse data structure
		DWORD dwBufSize = MAXIMUM_REPARSE_DATA_BUFFER_SIZE;
		REPARSE_DATA_BUFFER* rdata;
		rdata = (REPARSE_DATA_BUFFER*)malloc(dwBufSize);

		// Query the reparse data
		DWORD dwRetLen;
		BOOL bRet = DeviceIoControl(hFile, 
			FSCTL_GET_REPARSE_POINT,
			NULL,
			0,
			rdata,
			dwBufSize, 
			&dwRetLen,
			NULL);
		if (bRet == FALSE)
		{
			CloseHandle(hFile);
			return pPath;
		}
		CloseHandle(hFile);

		if (IsReparseTagMicrosoft(rdata->ReparseTag))
		{
			if (rdata->ReparseTag == IO_REPARSE_TAG_SYMLINK)
			{
				printf("Symbolic-Link\n");
				size_t slen = rdata->SymbolicLinkReparseBuffer.SubstituteNameLength / sizeof(WCHAR);
				WCHAR *szSubName = new WCHAR[slen + 1];
				wcsncpy_s(szSubName, slen + 1, &rdata->SymbolicLinkReparseBuffer.PathBuffer[rdata->SymbolicLinkReparseBuffer.SubstituteNameOffset / sizeof(WCHAR)], slen);
				szSubName[slen] = 0;
				printf("SubstitutionName (len: %d): '%S'\n", rdata->SymbolicLinkReparseBuffer.SubstituteNameLength, szSubName);
				delete[] szSubName;

				size_t plen = rdata->SymbolicLinkReparseBuffer.PrintNameLength / sizeof(WCHAR);
				WCHAR *szPrintName = new WCHAR[plen + 1];
				wcsncpy_s(szPrintName, plen + 1, &rdata->SymbolicLinkReparseBuffer.PathBuffer[rdata->SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(WCHAR)], plen);
				szPrintName[plen] = 0;
				printf("PrintName (len: %d): '%S'\n", rdata->SymbolicLinkReparseBuffer.PrintNameLength, szPrintName);
				delete[] szPrintName;
			}
			else if (rdata->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT)
			{
				printf("Mount-Point\n");
				size_t slen = rdata->MountPointReparseBuffer.SubstituteNameLength / sizeof(WCHAR);
				WCHAR *szSubName = new WCHAR[slen + 1];
				wcsncpy_s(szSubName, slen + 1, &rdata->MountPointReparseBuffer.PathBuffer[rdata->MountPointReparseBuffer.SubstituteNameOffset / sizeof(WCHAR)], slen);
				szSubName[slen] = 0;
				printf("SubstitutionName (len: %d): '%S'\n", rdata->MountPointReparseBuffer.SubstituteNameLength, szSubName);
				delete[] szSubName;

				size_t plen = rdata->MountPointReparseBuffer.PrintNameLength / sizeof(WCHAR);
				WCHAR *szPrintName = new WCHAR[plen + 1];
				wcsncpy_s(szPrintName, plen + 1, &rdata->MountPointReparseBuffer.PathBuffer[rdata->MountPointReparseBuffer.PrintNameOffset / sizeof(WCHAR)], plen);
				szPrintName[plen] = 0;
				printf("PrintName (len: %d): '%S'\n", rdata->MountPointReparseBuffer.PrintNameLength, szPrintName);
				delete[] szPrintName;
			}
			else
			{
				printf("No Mount-Point or Symblic-Link...\n");
			}
		}
		else
		{
			_tprintf(_T("Not a Microsoft-reparse point - could not query data!\n"));
		}
		free(rdata);
		return 0;
	}

	wstring ResolveNtfsPath(LPCWSTR pPathIn)
	{
		wstring ret;
		if(worker1(pPathIn, ret))
			return ret;

		int pathsize = GetFullPathName(pPathIn, 0, NULL,NULL);
		LPWSTR pPath = (LPWSTR)malloc((pathsize+1) * sizeof(WCHAR));
		stlsoft::scoped_handle<void*> pPathFree(pPath, free);

		GetFullPathName(pPathIn, pathsize+1, pPath, NULL);

		vector<wstring> dirs;
		LPWSTR pTmp = pPath;
		while(pTmp)
		{
			dirs.push_back(pTmp);
			pTmp = PathFindNextComponent(pTmp);
		}

		muzui
		wstring mountpoint;
		for(vector<string>::iterator it ; it != dirs.end() ; ++it)
		{
			wstring checkPath = ResolveDirectory(it->c_str());
		}

	}

}