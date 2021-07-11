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

#include "stdafx.h"

#pragma warning(disable :4786)
#pragma warning(disable: 4503)

#if !defined(UNICODE)
#error UNICODE required
#endif


#include <windows.h>
#include <tchar.h>
#include <assert.h>
#include <map>
#include <set>
#include <string>
using namespace std;


#include "I18NBeta.h"

#ifdef _DEBUG
#include "../DebugNew.h"
#endif

#ifndef countof
#define countof(a) (sizeof(a)/sizeof(a[0]))
#endif

namespace Ambiesoft {
	namespace stdosd {

		static HINSTANCE ghInst;

		CRITICAL_SECTION cs;
		static void UninitCS()
		{
			DeleteCriticalSection(&cs);
		}
		static bool InitCS();
		static bool bCinit = InitCS();
		static bool InitCS()
		{
			if (!bCinit)
			{
				InitializeCriticalSection(&cs);
				atexit(UninitCS);
			}
			return true;
		}


		struct CFreer {
			void* p_;
			CFreer(void* p) : p_(p) {}
			~CFreer() {
				free(p_);
			}
		};

		struct FileFreer {
			FILE* f_;
			FileFreer(FILE* f) : f_(f) {}
			~FileFreer() {
				fclose(f_);
			}
		};

	

		I18NSTRINGMAP i18map;

		std::string stLang_;

		bool stLangInit_;
		bool stAtExitInit_;


		static BYTE* UTF16toUTF8(LPCWSTR pIN)
		{
			int nReqSize = WideCharToMultiByte(CP_UTF8,
				0,
				pIN,
				-1,
				NULL,
				0,
				NULL,
				NULL);

			if (nReqSize == 0)
				return NULL;

			BYTE* pOut = (BYTE*)malloc(nReqSize);
			int nRet = WideCharToMultiByte(CP_UTF8,
				0,
				pIN,
				-1,
				(char*)pOut,
				nReqSize,
				NULL,
				NULL);

			if (nRet == 0 || nRet != nReqSize)
				return NULL;

			return pOut;;
		}
		static LPWSTR UTF8toUTF16(const LPBYTE pIN)
		{

			int nReqSize = MultiByteToWideChar(
				CP_UTF8,
				0,
				(const char*)pIN,
				-1,
				NULL,
				0);

			if (nReqSize == 0)
				return NULL;

			LPWSTR pOut = (LPWSTR)malloc(nReqSize * sizeof(WCHAR));
			int nRet = MultiByteToWideChar(CP_UTF8,
				0,
				(const char*)pIN,
				-1,
				pOut,
				nReqSize);

			if (nRet == 0 || nRet != nReqSize)
				return NULL;

			return pOut;;
		}
#ifdef _DEBUG
		std::set<wstring> nai;
		std::set<wstring> aru;
		void shownai()
		{
			WCHAR szModule[MAX_PATH] = { 0 };
			GetModuleFileNameW(ghInst, szModule, MAX_PATH);
			wstring strModule(szModule);

			std::set<wstring>::iterator it;
			wstring message;
			for (it = nai.begin(); it != nai.end(); ++it)
			{
				if (it->empty())
					continue;
				message += *it;
				message += L"\r\n";
			}

			if (!message.empty())
			{
				message = L"---------------------NOTI18N-------" + strModule + L">>>>>>>>>>>>\r\n" + message;
				message += L"---------------------NOTI18N-------" + strModule + L"<<<<<<<<<<<<\r\n";
				OutputDebugStringW(message.c_str());
			}



			message = L"";
			for (it = aru.begin(); it != aru.end(); ++it)
			{
				if (it->empty())
					continue;
				message += *it;
				message += L"\r\n";
			}

			if (!message.empty())
			{
				message = L"---------------------DUPLICATEI18N-------" + strModule + L">>>>>>>>>>>>\r\n" + message;
				message += L"---------------------DUPLICATEI18N-------" + strModule + L"<<<<<<<<<<<<\r\n";
				OutputDebugStringW(message.c_str());
			}
		}
#endif  // _DEBUG


		// static bool langinit = false;
		// static bool atexitinit = false;
		// static WCHAR stLang[4];


		void ClearMap()
		{
			for (I18NSTRINGMAP::iterator it = i18map.begin();
				it != i18map.end();
				++it)
			{
				free((void*)it->first);
			}
			i18map.clear();
		}

		//errno_t mywcslwr(TCHAR* str, size_t size)
		//{
		//	return _tcslwr_s(str, size);
		//}
		

		LPCWSTR I18NW(LPCWSTR pIN)
		{
			if (!bCinit)
				InitCS();

			if (!stLangInit_)
				i18nInitLangmap<wchar_t>();

			i18nlock lock;
			if (i18map.find(pIN) != i18map.end())
			{
				return i18map[pIN].c_str();
			}

#ifdef _DEBUG
			nai.insert(pIN);
#endif
			return pIN;
		}





		static BOOL CALLBACK enumDlgChild(
			HWND hwnd,
			LPARAM lParam)
		{
			TCHAR szClass[256];
			if (!GetClassNameW(hwnd, szClass, countof(szClass)))
				return TRUE;

			if (lstrcmpiW(szClass, _T("Static")) != 0 &&
				lstrcmpiW(szClass, _T("Button")) != 0 &&
				lstrcmpiW(szClass, _T("ScrollBar")) != 0)
			{
				return TRUE;
			}

			i18nChangeWindowText(hwnd);

			return TRUE;
		}

		void i18nChangeWindowText(HWND hwnd)
		{
			int size = ::GetWindowTextLength(hwnd);
			if (size)
			{
				LPTSTR p = new TCHAR[size + 1];
				if (size == ::GetWindowTextW(hwnd, p, size + 1))
				{
					::SetWindowTextW(hwnd, I18N(p));
				}
				delete[] p;
			}
		}
		void i18nChangeChildWindowText(HWND hDlg)
		{
			EnumChildWindows(hDlg,
				enumDlgChild,
				0);
		}

		void i18nChangeMenuText(HMENU menu)
		{
			int count = GetMenuItemCount(menu);
			for (int i = 0; i < count; ++i)
			{
				int len = GetMenuStringW(menu, i, NULL, 0, MF_BYPOSITION | MFT_STRING);
				int buffsize = (len + 1) * sizeof(TCHAR);
				TCHAR* pBuff = (TCHAR*)malloc(buffsize);
				CFreer maBuff(pBuff);
				GetMenuStringW(menu, i, pBuff, len + 1, MF_BYPOSITION | MFT_STRING);

				TCHAR* pTab = _tcschr(pBuff, L'\t');
				wstring afterTab;
				if (pTab)
				{
					*pTab = 0;
					afterTab = (pTab + 1);
				}

				wstring newtext = I18N(pBuff);
				if (!afterTab.empty())
				{
					newtext += L"\t";
					newtext += afterTab;
				}

				MENUITEMINFO miiset;
				miiset.cbSize = sizeof(miiset);
				miiset.fMask = 0x00000040; //MIIM_STRING;
				miiset.dwTypeData = (LPWSTR)newtext.c_str();
				SetMenuItemInfoW(menu, i, TRUE, &miiset);



				MENUITEMINFO  mii;
				mii.cbSize = sizeof(mii);
				mii.fMask = MIIM_SUBMENU;

				GetMenuItemInfoW(menu, i, TRUE, &mii);

				if (mii.hSubMenu)
				{
					i18nChangeMenuText(mii.hSubMenu);
				}


			}
		}



	}
} // namespace Ambiesoft