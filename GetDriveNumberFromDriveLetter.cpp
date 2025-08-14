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

#include <string>
#include <windows.h>

#include "DebugNew.h"

namespace Ambiesoft {

	bool GetDriveNumberFromDriveLetter(wchar_t driveLetter, int* pDriveNumber, DWORD* pLastError)
	{
        std::wstring volumePath = std::wstring(L"\\\\.\\") + driveLetter + L":";
        HANDLE hVolume = CreateFileW(volumePath.c_str(),
            0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
            OPEN_EXISTING, 0, NULL);
        if (hVolume == INVALID_HANDLE_VALUE)
        {
            if(pLastError)
				*pLastError = GetLastError();
            return false;
        }

        STORAGE_DEVICE_NUMBER sdn;
        DWORD bytesReturned = 0;
        if (!DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER,
            NULL, 0,
            &sdn, sizeof(sdn),
            &bytesReturned, NULL))
        {
			if (pLastError)
                *pLastError = GetLastError();
            CloseHandle(hVolume);
            return false;
        }

		*pDriveNumber = sdn.DeviceNumber;
        CloseHandle(hVolume);
        return true;
	}
}