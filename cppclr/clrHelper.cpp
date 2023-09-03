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

#ifdef __cplusplus_cli
#include <Windows.h>
#include <vcclr.h>
#include "clrHelper.h"

#pragma comment(lib,"Shell32.lib")

using namespace System;
using namespace System::Windows::Forms;
using namespace std;

namespace Ambiesoft {


	DialogResult Alert(String^ text)
	{
		return MessageBox::Show(
			text,
			Application::ProductName,
			MessageBoxButtons::OK,
			MessageBoxIcon::Warning);
	}
	DialogResult Alert(Exception^ ex)
	{
		return Alert(ex->Message);
	}


	String^ getShortPah(String^ file)
	{
		TCHAR szShort[MAX_PATH]; szShort[0] = 0;

		pin_ptr<const wchar_t> ps = PtrToStringChars(file);
		DWORD dwRet = GetShortPathName(ps, szShort, _countof(szShort));
		if (dwRet == 0 || dwRet > _countof(szShort))
			return file;

		return gcnew String(szShort);
	}
}

#endif  // __cplusplus_cli
