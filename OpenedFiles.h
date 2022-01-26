
#pragma once
#include <string>
#include <Windows.h>

#ifndef _VECTOR_
#pragma message(" Please include vector")
#endif

enum OF_TYPE
{
	FILES_ONLY = 1,
	MODULES_ONLY = 2,
	ALL_TYPES = 3
};

struct OF_INFO_t
{
	DWORD dwPID;
	LPCWSTR lpFile;
	HANDLE hFile;
};

struct OPENEDFILEINFO
{
	DWORD dwPID;
	TCHAR filename[MAX_PATH];
	HANDLE hFile;
};

typedef void (CALLBACK* OF_CALLBACK)(OF_INFO_t* pOpenedFileInf0, UINT_PTR uUserContext);


void ShowOpenedFiles(LPCWSTR lpPath);
void GetOpenedFiles(LPCWSTR lpPath,
	OF_TYPE Filter,
	OF_CALLBACK CallBackProc,
	UINT_PTR pUserContext);

std::wstring GetPathFromProcessID(const DWORD dwID);
bool EnableDebugPriv(void);
bool GetOpenedFilesSimple(LPCTSTR pFilter, std::vector<OPENEDFILEINFO>& v);