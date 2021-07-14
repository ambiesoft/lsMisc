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



#include "ProxySetting.h"

namespace Ambiesoft {
	typedef struct {

		//
		// dwAccessType - INTERNET_OPEN_TYPE_DIRECT, INTERNET_OPEN_TYPE_PROXY, or
		// INTERNET_OPEN_TYPE_PRECONFIG (set only)
		//

		DWORD dwAccessType;

		//
		// lpszProxy - proxy server list
		//

		LPCSTR lpszProxy;

		//
		// lpszProxyBypass - proxy bypass list
		//

		LPCSTR lpszProxyBypass;
	} INTERNET_PROXY_INFOA;


	BOOL ChangeProxySetting(PROXYTYPE useproxy, LPCSTR server, LPCSTR bypass)
	{
		tstring error;
		switch (useproxy)
		{
		case PROXY_PRECONFIG:
		{
			INTERNET_PROXY_INFOA pi = { 0 };
			pi.dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
			if (!InternetSetOptionA(NULL, INTERNET_OPTION_PROXY, &pi, sizeof(pi)))
			{
				error = GetLastErrorString(GetLastError());
			}
		}
		break;

		case PROXY_DIRECT:
		{
			INTERNET_PROXY_INFOA pi = { 0 };
			pi.dwAccessType = INTERNET_OPEN_TYPE_DIRECT;
			if (!InternetSetOptionA(NULL, INTERNET_OPTION_PROXY, &pi, sizeof(pi)))
			{
				error = GetLastErrorString(GetLastError());
			}
		}
		break;

		case PROXY_USEPROXY:
		{
			INTERNET_PROXY_INFOA pi = { 0 };
			pi.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
			pi.lpszProxy = server;
			pi.lpszProxyBypass = bypass;
			if (!InternetSetOptionA(NULL, INTERNET_OPTION_PROXY, &pi, sizeof(pi)))
			{
				error = GetLastErrorString(GetLastError());
			}
		}
		break;

		default:
			error = I18N(_T("Invalid Proxy Settings"));
			break;
		}

		if (error.size() != 0)
		{
			tstring message = I18N(_T("Proxy Settings Failed."));
			message += _T("\r\n") + error;
			AfxMessageBox(message.c_str());
		}

		return error.size() == 0;
	}



} // namespace Ambiesoft