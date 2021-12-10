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


// #include <afx.h>

#ifndef _SHLOBJ_H_
#pragma message("including shlobj.h")
#include <Shlobj.h>
#endif

#ifndef _STRING_
#pragma message("including string")
#include <string>
#endif

#ifndef _VECTOR_
#pragma message("including vector")
#include <vector>
#endif

#include "DebugMacro.h"
#include "SetCSIDLtoEnv.h"

using namespace std;

static BOOL mySetEnvironmentVariable(LPCWSTR pPrefix, LPCWSTR pSuffix, LPCWSTR p1, LPCWSTR p2, std::vector<wstring>* pResult)
{

	wstring w1;
	if(pPrefix)
		w1 = pPrefix;
	w1 += p1;
	if(pSuffix)
		w1 += pSuffix;

	DTRACE(w1.c_str());
	DTRACE(p2);

	BOOL bRet = SetEnvironmentVariable(w1.c_str(),p2);
	if(bRet && pResult)
	{
		pResult->push_back(w1);
	}
	return bRet;
}

#define SETENVSPFOLDER(PPP,TTT, SSS,RRR) do {\
		if(SHGetSpecialFolderPath(NULL,szT, CSIDL_##TTT, FALSE)) {\
			DVERIFY(mySetEnvironmentVariable(PPP,SSS, L#TTT, szT,RRR)); \
		}\
	} while(false) 


void SetCSIDLtoEnvW(LPCWSTR pPrefix, LPCWSTR pSuffix, std::vector<wstring>* pResult)
{
	WCHAR szT[MAX_PATH];

	SETENVSPFOLDER(	pPrefix,	DESKTOP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	INTERNET	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAMS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	CONTROLS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PRINTERS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PERSONAL	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	FAVORITES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	STARTUP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	RECENT	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	SENDTO	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	BITBUCKET	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	STARTMENU	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	MYDOCUMENTS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	MYMUSIC	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	MYVIDEO	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	DESKTOPDIRECTORY	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	DRIVES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	NETWORK	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	NETHOOD	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	FONTS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	TEMPLATES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_STARTMENU	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_PROGRAMS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_STARTUP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_DESKTOPDIRECTORY	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	APPDATA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PRINTHOOD	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	LOCAL_APPDATA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	ALTSTARTUP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_ALTSTARTUP	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_FAVORITES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	INTERNET_CACHE	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COOKIES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	HISTORY	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_APPDATA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	WINDOWS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	SYSTEM	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAM_FILES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	MYPICTURES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROFILE	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	SYSTEMX86	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAM_FILESX86	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAM_FILES_COMMON	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	PROGRAM_FILES_COMMONX86	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_TEMPLATES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_DOCUMENTS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_ADMINTOOLS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	ADMINTOOLS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	CONNECTIONS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_MUSIC	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_PICTURES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_VIDEO	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	RESOURCES	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	RESOURCES_LOCALIZED	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMMON_OEM_LINKS	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	CDBURN_AREA	,pSuffix	,pResult	);
	SETENVSPFOLDER(	pPrefix,	COMPUTERSNEARME	,pSuffix	,pResult	);

}