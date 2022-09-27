//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.


#include <windows.h>
#include <vector>
#include <string>

#include "CHandle.h"
#include "GetVolumeInfo.h"
#include "DebugNew.h"

namespace Ambiesoft {

    // Taken from https://learn.microsoft.com/en-us/windows/win32/fileio/displaying-volume-paths
    static std::vector<std::wstring> DisplayVolumePaths(
        __in PWCHAR VolumeName
    )
    {
        DWORD  CharCount = MAX_PATH + 1;
        PWCHAR Names = NULL;
        PWCHAR NameIdx = NULL;
        BOOL   Success = FALSE;

        std::vector<std::wstring> ret;

        for (;;)
        {
            //
            //  Allocate a buffer to hold the paths.
            Names = (PWCHAR) new BYTE[CharCount * sizeof(WCHAR)];

            if (!Names)
            {
                //
                //  If memory can't be allocated, return.
                return ret;
            }

            //
            //  Obtain all of the paths
            //  for this volume.
            Success = GetVolumePathNamesForVolumeNameW(
                VolumeName, Names, CharCount, &CharCount
            );

            if (Success)
            {
                break;
            }

            if (GetLastError() != ERROR_MORE_DATA)
            {
                break;
            }

            //
            //  Try again with the
            //  new suggested size.
            delete[] Names;
            Names = NULL;
        }

        if (Success)
        {
            //
            //  Display the various paths.
            for (NameIdx = Names;
                NameIdx[0] != L'\0';
                NameIdx += wcslen(NameIdx) + 1)
            {
                // wprintf(L"  %s", NameIdx);
                ret.push_back(NameIdx);
            }
            // wprintf(L"\n");
        }

        if (Names != NULL)
        {
            delete[] Names;
            Names = NULL;
        }

        return ret;
    }

	int GetVolumeInfo(std::vector<VolumeInfo>* pInfo)
	{
        DWORD  CharCount = 0;
        WCHAR  DeviceName[MAX_PATH] = L"";
        DWORD  Error = ERROR_SUCCESS;
        CFindVolumeHandle FindHandle;
        BOOL   Found = FALSE;
        size_t Index = 0;
        BOOL   Success = FALSE;
        WCHAR  VolumeName[MAX_PATH] = L"";

        //
        //  Enumerate all volumes in the system.
        FindHandle = FindFirstVolumeW(VolumeName, ARRAYSIZE(VolumeName));

        if (FindHandle == INVALID_HANDLE_VALUE)
            return GetLastError();

        for (;;)
        {
            VolumeInfo viRet;

            //
            //  Skip the \\?\ prefix and remove the trailing backslash.
            Index = wcslen(VolumeName) - 1;

            if (VolumeName[0] != L'\\' ||
                VolumeName[1] != L'\\' ||
                VolumeName[2] != L'?' ||
                VolumeName[3] != L'\\' ||
                VolumeName[Index] != L'\\')
            {
                return ERROR_BAD_PATHNAME;
                //wprintf(L"FindFirstVolumeW/FindNextVolumeW returned a bad path: %s\n", VolumeName);
                //break;
            }

            //
            //  QueryDosDeviceW does not allow a trailing backslash,
            //  so temporarily remove it.
            VolumeName[Index] = L'\0';

            CharCount = QueryDosDeviceW(&VolumeName[4], DeviceName, ARRAYSIZE(DeviceName));

            VolumeName[Index] = L'\\';

            if (CharCount == 0)
            {
                return GetLastError();
                //wprintf(L"QueryDosDeviceW failed with error code %d\n", Error);
                //break;
            }

            // wprintf(L"\nFound a device:\n %s", DeviceName);
            viRet.device = DeviceName;
            
            // wprintf(L"\nVolume name: %s", VolumeName);
            viRet.volume = VolumeName;

            // wprintf(L"\nPaths:");
            viRet.paths = DisplayVolumePaths(VolumeName);

            pInfo->push_back(viRet);

            //
            //  Move on to the next volume.
            Success = FindNextVolumeW(FindHandle, VolumeName, ARRAYSIZE(VolumeName));

            if (!Success)
            {
                Error = GetLastError();

                if (Error != ERROR_NO_MORE_FILES)
                {
                    return Error;
                    //wprintf(L"FindNextVolumeW failed with error code %d\n", Error);
                    //break;
                }

                //
                //  Finished iterating
                //  through all the volumes.
                Error = ERROR_SUCCESS;
                break;
            }
        }

        return 0;
	}

} // namespace