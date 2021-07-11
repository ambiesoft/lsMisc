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
// 2. Call i18nInitLangmap() at startup, or i18nInitLangmap(L"jpn") for specifying language.
// 3. Create a Folder named "lang" in the same folder your executable resides, and place a lang file in it 
//    and name it like "myapp.jpn.txt" where myapp is name of the application.
// 4. "myapp.jpn.txt" format is like follows:
//    "Hello"="こんにちは"
//    "Goodby"="さようなら"
// 5. Call I18N(L"Hello") in your code. It returns const "こんにちは".
// 6. Call i18NChangexxxx() for translating windows or dialog texts. Show the source for more informations.
// 7. Look the debugger output when the program exists. Untranslated words will be printed.

#pragma once

#include <locale>
#include <codecvt>
#include <string>

#include "../UTF16toUTF8.h"
#include "stdosd.h"

namespace Ambiesoft {
	namespace stdosd {
		
		void ClearMap();
		//errno_t mywcslwr(TCHAR* str, size_t size);

		void shownai();

		struct stringcomparer
		{
			bool operator()(LPCTSTR x, LPCTSTR y) const
			{
				return _tcscmp(x, y) > 0;
			}
		};
		typedef std::map<LPCWSTR, std::wstring, stringcomparer> I18NSTRINGMAP;
		extern I18NSTRINGMAP i18map;

		extern CRITICAL_SECTION cs;
		struct i18nlock
		{
			i18nlock()
			{
				EnterCriticalSection(&cs);
			}
			~i18nlock()
			{
				LeaveCriticalSection(&cs);
			}
		};

#ifdef _DEBUG
		extern std::set<std::wstring> nai;
		extern std::set<std::wstring> aru;
#endif

		enum APP_ENCODING {
			APP_ENCODING_SYSTEM_DEFAULT,
			APP_ENCODING_UTF8,
			APP_ENCODING_UTF16,
			APP_ENCODING_UTF32,
			APP_ENCODING_WINDOWS_ACP,
		};
		extern std::string stLang_;

		extern bool stLangInit_;
		extern bool stAtExitInit_;


		//template<class C>
		//inline static std::basic_string<C>& stLang()
		//{
		//	assert(false);
		//}
		//template<>
		//inline static std::basic_string<char>& stLang()
		//{
		//	return stLangA_;
		//}
		//template<>
		//inline static std::basic_string<wchar_t>& stLang()
		//{
		//	return stLangW_;
		//}

		inline static int AmbGetLocaleInfo(LCID lcl, LCTYPE lcType, char* lpLCData, int cchData)
		{
			return ::GetLocaleInfoA(lcl, lcType, lpLCData, cchData);
		}
		inline static int AmbGetLocaleInfo(LCID lcl, LCTYPE lcType, wchar_t* lpLCData, int cchData)
		{
			return ::GetLocaleInfoW(lcl, lcType, lpLCData, cchData);
		}

		template<class C>
		inline static std::basic_string<C> encode(const char* p)
		{}
		template<>
		inline static std::basic_string<char> encode(const char* p)
		{
			return p;
		}
		template<>
		inline static std::basic_string<wchar_t> encode(const char* p)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			// std::string narrow = converter.to_bytes(wide_utf16_source_string);
			std::wstring wide = converter.from_bytes(p);

			return wide;
		}


		template<class C>
		inline static std::basic_string<C> encode(const wchar_t* p)
		{}
		template<>
		inline static std::basic_string<char> encode(const wchar_t* p)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::string narrow = converter.to_bytes(p);
			// std::wstring wide = converter.from_bytes(p);

			return narrow;
		}
		template<>
		inline static std::basic_string<wchar_t> encode(const wchar_t* p)
		{
			return p;
		}


		template<class C>
		inline static std::basic_string<C> i18nInitLangmap(HINSTANCE hInst = NULL,
			const C* pLang = NULL, 
			const C* pAppName = NULL,
			APP_ENCODING appEncoding = APP_ENCODING_SYSTEM_DEFAULT)
		{
			// ghInst = hInst;
			typedef std::basic_string<C> mstring ;

			mstring strLangTmp;
			if (!pLang || pLang[0] == 0)
			{
				C szT[4];
				AmbGetLocaleInfo(
					LOCALE_SYSTEM_DEFAULT,
					LOCALE_SABBREVLANGNAME,
					szT,
					4);
				strLangTmp = szT;
				pLang = strLangTmp.c_str();
			}

			assert(pLang[0] == 0 || getCharLength(pLang) == 3);
			if (!isCharEqual(pLang, encode<C>(stLang_.c_str()).c_str(), true))
			{
				stLangInit_ = false;
				stLang_ = encode<char>(pLang);
				ClearMap();
			}

			if (!stAtExitInit_)
			{
				i18nlock lock;
				if (!stAtExitInit_)
				{
					stAtExitInit_ = true;
					atexit(ClearMap);
#ifdef _DEBUG
					atexit(shownai);
#endif
				}
			}
			if (!stLangInit_)
			{
				{
					i18nlock lock;
					if (!stLangInit_)
					{
						do
						{
							mstring strLang = encode<C>(stLang_.c_str());
							// lstrcpy(szLang, stLang);
							
							{
								strLang = stdStringLower(strLang);

								WCHAR szT[MAX_PATH] = { 0 };
								GetModuleFileNameW(hInst, szT, (sizeof(szT) / sizeof(szT[0])) - 1);
								*(_tcsrchr(szT, L'\\')) = 0;

								WCHAR szTry[MAX_PATH];
								if (pAppName && pAppName[0])
								{
									wsprintfW(szTry, L"%s\\lang\\%s.%s.txt", szT, pAppName, encode<wchar_t>(strLang.c_str()).c_str());
								}
								else
								{
									wsprintfW(szTry, L"%s\\lang\\%s.txt", szT, encode<wchar_t>(strLang.c_str()).c_str());
								}

								FILE* f = NULL;
								errno_t  err = _wfopen_s(&f, szTry, L"rb");
								if (err || !f)
								{
#ifdef _DEBUG
									wchar_t szT[1024]; szT[0] = 0;
									_wcserror_s(szT, err);
									wstring message;
									message.append(L"Failed to open lang-file: ");
									message.append(szT);
									message.append(L": ");
									message.append(szTry);
									message.append(L"\r\n");
									OutputDebugString(message.c_str());
#endif
									break;
								}
								// FileFreer ffreer(f);
								std::unique_ptr<FILE, int(_cdecl*)(FILE*)> fautoFree(f, fclose);

#ifdef _DEBUG
								{
									wstring message;
									message.append(L"Lang file opened: ");
									message.append(szTry);
									message.append(L"\r\n");
									OutputDebugString(message.c_str());
								}
#endif

								BYTE* pB = NULL;
								BYTE b;
								size_t count = 0;
								size_t bomcount = 0;
								while (1 == fread((void*)&b, 1, 1, f))
								{
									if (count == 0 && bomcount < 3)
									{
										if (bomcount == 0)
										{
											if (b == 0xEF)
											{
												++bomcount;
												continue;
											}
											else
												bomcount = 4;
										}
										else if (bomcount == 1)
										{
											if (b == 0xBB)
											{
												++bomcount;
												continue;
											}
											else
											{
												++count;
												pB = (BYTE*)realloc(pB, count);
												pB[count - 1] = 0xFE;

												bomcount = 4;
											}

										}
										else if (bomcount == 2)
										{
											if (b == 0xBF)
											{
												++bomcount;
												continue;
											}
											else
											{
												++count;
												pB = (BYTE*)realloc(pB, count);
												pB[count - 1] = 0xFE;

												++count;
												pB = (BYTE*)realloc(pB, count);
												pB[count - 1] = 0xBB;

												bomcount = 4;
											}
										}
									}
									++count;
									pB = (BYTE*)realloc(pB, count);
									pB[count - 1] = b;
								}
								++count;
								pB = (BYTE*)realloc(pB, count);
								pB[count - 1] = 0;


								unique_ptr<wchar_t> pA(UTF8toUTF16Ex((char*)pB));
								free(pB);
								// CFreer pAFreer(pA);

								wchar_t *context = NULL;
								LPCWSTR pTok = wcstok_s(pA.get(), L"\n", &context);
								while (pTok)
								{
									wstring left;
									wstring right;
									int phase = 0;
									bool done = false;
									bool esc = false;
									for (; *pTok; ++pTok)
									{
										if (phase == 0)
										{
											if (*pTok == L'#')
											{
												done = true;
												break;
											}

											if (*pTok != L'"')
												continue;

											phase = 1;
											continue;
										}
										else if (phase == 1)
										{
											if (esc)
											{
												esc = false;
												if (*pTok == L'n')
												{
													left += L"\n";
												}
												else if (*pTok == L'r')
												{
													left += L"\r";
												}
												else
													left += *pTok;
												continue;
											}
											if (*pTok == L'\\')
											{
												esc = true;
												continue;
											}
											if (*pTok == L'"')
											{
												phase = 2;
												continue;
											}
											left += *pTok;
											continue;
										}
										else if (phase == 2)
										{
											if (*pTok != L'=')
												continue;

											phase = 3;
										}
										else if (phase == 3)
										{
											if (*pTok != L'"')
												continue;

											phase = 4;
											continue;
										}
										else if (phase == 4)
										{
											if (esc)
											{
												esc = false;
												if (*pTok == L'n')
												{
													right += L"\n";
												}
												else if (*pTok == L'r')
												{
													right += L"\r";
												}
												else
												{
													right += *pTok;
												}
												continue;
											}
											if (*pTok == L'\\')
											{
												esc = true;
												continue;
											}
											if (*pTok == L'"')
											{
												phase = 5;
												continue;
											}
											right += *pTok;
											continue;
										}
										else if (phase == 5)
										{
											break;
										}
										else
										{
											assert(false);
										}
									}

									if (!left.empty())
									{
#ifdef _DEBUG
										if (i18map.count(left.c_str()) != 0)
										{
											aru.insert(left);
										}
#endif
										i18map[_wcsdup(left.c_str())] = right;
									}

									pTok = wcstok_s(NULL, L"\n", &context);
								}

							}


						} while (0);


					}
					stLangInit_ = true;
				}
			}

			return encode<C>(stLang_.c_str());
		}

		//template<class C>
		//const char* i18nGetCurrentLang()
		//{
		//	return CI18NStorage
		//}




		LPCWSTR i18nGetCurrentLang();

		LPCWSTR I18NW(LPCWSTR pIN);
#ifdef UNICODE
		inline LPCWSTR I18N(LPCWSTR pIN) { return I18NW(pIN); }
#else
		inline LPCSTR I18N(LPCSTR pIN) { return I18NA(pIN); }
#endif

		void i18nChangeWindowText(HWND hwnd);
		void i18nChangeChildWindowText(HWND hwnd);
		void i18nChangeMenuText(HMENU menu);
	} // namespace Ambiesoft
}