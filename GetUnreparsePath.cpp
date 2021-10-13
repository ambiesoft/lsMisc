// https://docs.microsoft.com/ja-jp/windows/win32/memory/obtaining-a-file-name-from-a-file-handle?redirectedfrom=MSDN

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <psapi.h>
#include <strsafe.h>
#include <Shlwapi.h>

#include <string>
#include <vector>

#include "CHandle.h"

#include "GetUnreparsePath.h"

#define BUFSIZE 512
using namespace std;

namespace Ambiesoft {

    static wstring GetUnreparsePath(HANDLE hFile)
    {
        BOOL bSuccess = FALSE;
        TCHAR pszFilename[MAX_PATH + 1];
        pszFilename[0] = 0;
        HANDLE hFileMap;

        // Get the file size.
        DWORD dwFileSizeHi = 0;
        DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

        if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
        {
            _tprintf(TEXT("Cannot map a file with a length of zero.\n"));
            return FALSE;
        }

        // Create a file mapping object.
        hFileMap = CreateFileMapping(hFile,
            NULL,
            PAGE_READONLY,
            0,
            1,
            NULL);

        if (hFileMap)
        {
            // Create a file mapping to get the file name.
            void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);

            if (pMem)
            {
                if (GetMappedFileName(GetCurrentProcess(),
                    pMem,
                    pszFilename,
                    MAX_PATH))
                {

                    // Translate path with device name to drive letters.
                    TCHAR szTemp[BUFSIZE];
                    szTemp[0] = '\0';

                    if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
                    {
                        TCHAR szName[MAX_PATH];
                        TCHAR szDrive[3] = TEXT(" :");
                        BOOL bFound = FALSE;
                        TCHAR* p = szTemp;

                        do
                        {
                            // Copy the drive letter to the template string
                            *szDrive = *p;

                            // Look up each device name
                            if (QueryDosDevice(szDrive, szName, MAX_PATH))
                            {
                                size_t uNameLen = _tcslen(szName);

                                if (uNameLen < MAX_PATH)
                                {
                                    bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
                                        && *(pszFilename + uNameLen) == _T('\\');

                                    if (bFound)
                                    {
                                        // Reconstruct pszFilename using szTempFile
                                        // Replace device path with DOS path
                                        TCHAR szTempFile[MAX_PATH];
                                        StringCchPrintf(szTempFile,
                                            MAX_PATH,
                                            TEXT("%s%s"),
                                            szDrive,
                                            pszFilename + uNameLen);
                                        StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
                                    }
                                }
                            }

                            // Go to the next NULL character.
                            while (*p++);
                        } while (!bFound && *p); // end of string
                    }
                }
                bSuccess = TRUE;
                UnmapViewOfFile(pMem);
            }

            CloseHandle(hFileMap);
        }
        return pszFilename;
    }

    wstring GetUnreparsePath(const wchar_t* path)
    {
        if (path == nullptr)
            return wstring();
        if (PathIsUNC(path))
            return path;

        Ambiesoft::CFileHandle file(CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL,
            OPEN_EXISTING, 0, NULL));

        if (!file)
        {
            return path;
        }
        wstring ret = GetUnreparsePath(file);
        return ret.empty() ? path : ret;
    }
    vector<wstring> GetUnreparsePath(const vector<wstring>& paths)
    {
        vector<wstring> rets;
        for (auto&& path : paths)
            rets.push_back(GetUnreparsePath(path.c_str()));
        return rets;
    }
} // namespace Ambiesoft