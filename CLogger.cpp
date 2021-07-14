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

#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif


#include <tchar.h>
#include <string>
#include <time.h>
#include "stdwin32/stdwin32.h"
#include "stdosd/stdosd.h"
#include "../lsMisc/UTF16toUTF8.h"

#include "tstring.h"

#include "CLogger.h"



namespace Ambiesoft {

	using namespace Ambiesoft::stdosd;
	using namespace stdwin32;
	using namespace std;

	HANDLE CLogger::m_hLog;
	CLogger::CLogger()
	{

	}

	CLogger::~CLogger()
	{
		Close();
	}
	bool CLogger::Close()
	{
		bool ret = true;
		if (m_hLog)
		{
			ret = !!CloseHandle(m_hLog);
			m_hLog = NULL;
		}
		return ret;
	}
	bool CLogger::Prepare()
	{
		tstring file = stdGetFileName(stdGetModuleFileName<TCHAR>()) + _T(".log");

		m_hLog = CreateFile(file.c_str(),
			FILE_APPEND_DATA,
			FILE_SHARE_READ | FILE_SHARE_WRITE, // share
			NULL, // sec
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (m_hLog == INVALID_HANDLE_VALUE)
		{
			m_hLog = NULL;
			return false;
		}

		return true;
	}

	bool CLogger::Out(LPCSTR p1, DWORD d1)
	{
		char szT[32];
		wsprintfA(szT, "%d", d1);

		string s(p1);
		s += szT;

		return Out(s.c_str());
	}

	bool CLogger::Out(LPCSTR pSTR)
	{
		if (!m_hLog)
			return false;

		CHAR buff[256]; buff[0] = 0;
		time_t now = time(NULL);
		struct tm stnow;
		localtime_s(&stnow, &now);
		strftime(buff, _countof(buff), "%X\t", &stnow);

		DWORD dwWritten = 0;
		BOOL bFailed = false;


		dwWritten = 0;
		bFailed = !WriteFile(m_hLog,
			buff,
			lstrlenA(buff)*sizeof(CHAR),
			&dwWritten,
			NULL);

		dwWritten = 0;
		bFailed = !WriteFile(m_hLog,
			pSTR,
			lstrlenA(pSTR)*sizeof(CHAR),
			&dwWritten,
			NULL);

		if (true)
		{
			bFailed |= !WriteFile(m_hLog,
				"\r\n",
				2,
				&dwWritten,
				NULL);
		}

		if (!bFailed)
			FlushFileBuffers(m_hLog);

		return (!bFailed);
	}

	bool CLogger::Out(const wstring& wstr)
	{
		return Out(toStdUtf8String(wstr).c_str());
	}

}
