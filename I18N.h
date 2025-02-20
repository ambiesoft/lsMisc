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

// HOW TO USE
// 1. Include I18N.h and I18N.cpp to your project.
// 2. Call i18nInitLangmap() at startup, or i18nInitLangmap(nullptr, L"myapp", L"jpn") for specifying language.
// 3. Create a Folder named "lang" in the same folder your executable resides, and place a lang file in it 
//    and name it like "myapp.jpn.txt" where myapp is name of the application.
// 4. "myapp.jpn.txt" format is like follows:
//    "Hello"="こんにちは"
//    "Goodby"="さようなら"
// 5. Call I18N(L"Hello") in your code. It returns const "こんにちは".
// 6. Call i18NChangexxxx() for translating windows or dialog texts. Show the source for more informations.
// 7. Look the debugger output when the program exists. Untranslated words will be printed.

#pragma once
#include <Windows.h>
namespace Ambiesoft {
	bool i18nInitLangmap(HINSTANCE hInst = NULL, LPCWSTR pLang = NULL, LPCWSTR pAppName = NULL);
	LPCWSTR i18nGetCurrentLang();

	LPCWSTR I18NW(LPCWSTR pIN);
#ifdef UNICODE
	inline LPCWSTR I18N(LPCWSTR pIN) { return I18NW(pIN); }
#else
	inline LPCSTR I18N(LPCSTR pIN) { return I18NA(pIN); }
#endif

	void i18nChangeWindowText(HWND hwnd);
	void i18nChangeChildWindowText(HWND hwnd);
	void i18nChangeDialogText(HWND hWnd, HWND* except = nullptr, int exceptCount = 0);
	void i18nChangeMenuText(HMENU menu);

	void i18nClearLangmap();
} // namespace Ambiesoft
