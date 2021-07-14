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



#ifdef AMBIESOFT_PROFILER_ENABLED

#include <Windows.h>


#include "stdosd/stdosd.h"
#include "stdwin32/stdwin32.h"
#include "Profiler.h"

using namespace Ambiesoft::stdosd;
using namespace Ambiesoft::stdwin32;
using namespace std;

namespace Ambiesoft
{
	CProfiler::CProfiler()
	{
		tick_ = GetTickCount();
		string name = stdGetFileNameWitoutExtension(stdGetModuleFileNameA()) + ".profile";
		fopen_s(&file_, name.c_str(), "w");
	}
	CProfiler::~CProfiler()
	{
		if(file_)
			fclose(file_);
	}
	void CProfiler::Stamp(const std::string& codemessage, const char* filename, int line)
	{
		static DWORD prevnow;
		DWORD now = GetTickCount();

		DWORD elapse = prevnow==0 ? now-tick_ : now-prevnow;

		fprintf_s(file_, "Elapsed=%d:%s:File=%s:Line=%d\n", elapse, codemessage.c_str(), filename, line);

		prevnow = now;
	}
	void CProfiler::Close(const std::string& codemessage, const char* filename, int line)
	{
		Stamp(codemessage, filename, line);
		if(file_)
		{
			fclose(file_);
			file_ = NULL;
		}
	}
}

#endif // AMBIESOFT_PROFILER_ENABLED