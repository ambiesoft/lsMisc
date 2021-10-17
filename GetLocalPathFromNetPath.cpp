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
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <psapi.h>
#include <strsafe.h>
#include <Shlwapi.h>
#include <lmshare.h>
#include <Lm.h>
#include <string>
#include <vector>

#include "CHandle.h"

#include "GetLocalPathFromNetPath.h"

#pragma comment(lib,"Mpr.lib")
#pragma comment(lib,"Netapi32.lib")

#define BUFSIZE 512
using namespace std;

namespace Ambiesoft {

    static void getServerAndShare(const wstring& netfile, 
        wstring* server, 
        wstring* serverandshare,
        wstring* remnant)
    {
        if (netfile.size() < 3)
            return;
        if (netfile[0] != L'\\' || netfile[1] != L'\\')
            return;
        size_t firstsep = netfile.find(L'\\', 2);
        if (firstsep <= 0)
            return;

        *server = netfile.substr(2, firstsep - 2);
        
        size_t secondsep = netfile.find(L'\\', firstsep + 1);
        if (secondsep == wstring::npos) 
        {
            *serverandshare = netfile.substr(0);
            *remnant = wstring();
        }
        else
        {
            *serverandshare = netfile.substr(0, secondsep);
            *remnant = netfile.substr(secondsep);
        }
    }
    wstring GetLocalPathFromNetPath(const wchar_t* path)
    {
        if (path == nullptr || path[0] == 0)
            return path;
        wstring networkpath;
        vector<wchar_t> buff;
        DWORD len;
        if (lstrlen(path) >= 3 &&
            (iswalpha(path[0]) && path[1] == L':' && path[2] == L'\\'))
        {
            wstring strPath(path);
        
            buff.resize(512);
            len = buff.size();
            DWORD dwRet = WNetGetConnection(strPath.substr(0, 2).c_str(), &buff[0], &len);
            if (!(dwRet != 0 || buff[0] == 0))
            {
                networkpath = &buff[0];
                networkpath += strPath.substr(2);
            }
        }
        if (networkpath.empty())
            networkpath = path;
        if (!PathIsUNC(networkpath.c_str()))
            return path;
        wstring server, serverandshare, remnant;
        getServerAndShare(networkpath, &server, &serverandshare, &remnant);
        if (serverandshare.empty())
            return path;
        buff.resize(512);
        len = buff.size();
        if (0 == GetComputerName(&buff[0], &len) || GetLastError() == ERROR_BUFFER_OVERFLOW)
            return path;
        if (lstrcmpi(&buff[0], server.c_str()) != 0)
            return path;
        LPBYTE pBuf = nullptr;
        DWORD entriesread = 0;
        DWORD totalentries = 0;
        DWORD resume_handle = 0;
        wstring localpath;
        NET_API_STATUS res;
        do {
            pBuf = nullptr;
            res = NetShareEnum(
                (LPWSTR)server.c_str(),
                2,                  // [in]      DWORD   level,
                &pBuf,              //[out]     LPBYTE * bufptr,
                MAX_PREFERRED_LENGTH, //[in]      DWORD   prefmaxlen,
                &entriesread,       // [out]     LPDWORD entriesread,
                &totalentries,      // [out]     LPDWORD totalentries,
                &resume_handle      // [in, out] LPDWORD resume_handle
            );
            if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
            {
                for (DWORD i = 0; i < entriesread; ++i)
                {
                    SHARE_INFO_2* si2 = ((SHARE_INFO_2*)pBuf) + i;
                    wstring t = L"\\\\";
                    t += server;
                    t += L"\\";
                    t += si2->shi2_netname;
                    if (lstrcmpi(t.c_str(), serverandshare.c_str()) == 0)
                    {
                        localpath = si2->shi2_path;
                        localpath += remnant;
                        res = ERROR_SUCCESS;
                        break;
                    }
                }
            }
            NetApiBufferFree(pBuf);
        } while (res == ERROR_MORE_DATA);
        if (localpath.empty())
            return path;
        return localpath;
    }
    vector<wstring> GetLocalPathFromNetPath(const vector<wstring>& paths)
    {
        vector<wstring> rets;
        for (auto&& path : paths)
            rets.push_back(GetLocalPathFromNetPath(path.c_str()));
        return rets;
    }
} // namespace Ambiesoft