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



#include <ostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Logging.h"

#include "DebugNew.h"

namespace Ambiesoft {

	Logging::Logging(const wchar_t* file)
	{
		if (file_.is_open())
			return;
		file_.open(file, std::ios::app);
	}

	Logging::~Logging()
	{
		file_.close();
	}
	void Logging::Write(const std::wstring& s)
	{
		// time_t now = std::time(nullptr);
		__time64_t now = 0;
		_time64(&now);

		// struct tm* localNow = std::localtime(&now);
		struct tm localNow = { 0 };
		_localtime64_s(&localNow, &now);
		
		
		wchar_t buf[64];
		wcsftime(buf, _countof(buf), L"%c", &localNow);
		file_ << buf << L"\t";

		file_ << s << std::endl;
	}
}