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
#pragma warning(disable:4786)
#pragma once

#include <vector>
#include <string>
#include <windows.h>
// #include "../tstring.h"

namespace Ambiesoft {
	namespace stdwin32 {

//		template<typename T>
//		struct StdCharTraits;
//		template<>
//		struct StdCharTraits<char>
//		{
//			static DWORD ssGetModuleFileName(
//				HMODULE hModule,
//				char* lpFilename,
//				DWORD nSize
//				)
//			{
//				return GetModuleFileNameA(hModule, lpFilename, nSize);
//			}
//		};
//		template<>
//		struct StdCharTraits<wchar_t>
//		{
//			static DWORD ssGetModuleFileName(
//				HMODULE hModule,
//				wchar_t* lpFilename,
//				DWORD nSize
//				)
//			{
//				return GetModuleFileNameW(hModule, lpFilename, nSize);
//			}
//		};
//
//		template<typename ST>
//		ST stdGetModuleFileNameTmplate(HINSTANCE hInst = NULL)
//		{
//			typename ST::traits_type::char_type* p = NULL;
//			DWORD size = 64;
//			for (;;)
//			{
//                                p = (typename ST::traits_type::char_type*)realloc(p, size * sizeof(typename ST::traits_type::char_type));
//                                if (StdCharTraits<typename ST::traits_type::char_type>::ssGetModuleFileName(hInst, p, size) < size)
//					break;
//
//				// Make double the size of required memory
//				size *= 2;
//			}
//
//			ST ret = p;
//			free((void*)p);
//			return ret;
//		}
//
//		inline std::string stdGetModuleFileNameA() {
//			return stdGetModuleFileNameTmplate<std::string>();
//		}
//		inline std::wstring stdGetModuleFileNameW() {
//			return stdGetModuleFileNameTmplate<std::wstring>();
//		}
//#ifdef UNICODE
//		inline std::wstring stdGetModuleFileName() {
//			return stdGetModuleFileNameW();
//		}
//#else
//		inline std::string stdGetModuleFileName() {
//			return stdGetModuleFileNameA();
//		}
//#endif




		// bool stdIsFullPath(LPCWSTR pD, bool allownetwork = true);


		//std::wstring stdCombinePath(LPCWSTR pD1, LPCWSTR pD2);
		//std::wstring stdCombinePath(const std::wstring& d1, const std::wstring& d2);

		//std::wstring stdGetParentDirectory(const std::wstring& path, bool bAddBackslach=false);
		//std::wstring stdGetParentDirectory(LPCWSTR pPath, bool bAddBackslach=false);

		//std::wstring stdGetFileName(const std::wstring& full);
		//std::wstring stdGetFileName(LPCWSTR pFull);

		//std::wstring stdGetFileNameWitoutExtension(LPCWSTR pPath);
		//std::wstring stdGetFileNameWitoutExtension(const std::wstring& w);

		// move to osd
		//std::wstring stdGetFileExtension(LPCWSTR pPath);
		//std::wstring stdGetFileExtension(const std::wstring& w);

		std::vector<std::wstring> stdSplitSCedPath(LPCWSTR pPath);



		//#ifndef __cplusplus_cli
		//	std::string string_formatA(const std::string fmt, ...);
		//	std::wstring string_formatW(const std::wstring fmt, ...);
		//#ifdef UNICODE
		//	#define string_format string_formatW
		//#else
		//	#define string_format string_formatA
		//#endif
		//#endif // __cplusplus_cli

		// // moved to stdTrim
//		std::string trimA(const std::string& str,
//			const std::string& whitespace = " \t\r\n");
//		std::wstring trimW(const std::wstring& str,
//			const std::wstring& whitespace = L" \t\r\n");
//#ifdef UNICODE
//#define trim trimW
//#else
//#define trim trimA
//#endif





		std::wstring stdGetCurrentDirectory();







		//	bool hasEndingIA (std::string const &fullString, std::string const &ending);
		//	bool hasEndingIW (std::wstring const &fullString, std::wstring const &ending);
		//#ifdef UNICODE
		//	#define hasEndingI hasEndingIW
		//#else
		//	#define hasEndingI hasEndingIA
		//#endif



		std::string stdItoA(int i);
		std::wstring stdItoW(int i);
#ifdef UNICODE
#define stdItoT stdItoW
#else
#define stdItoT stdItoA
#endif



		std::string stdItoA64(__int64 i);
		std::wstring stdItoW64(__int64 i);
#ifdef UNICODE
#define stdItoT64 stdItoW64
#else
#define stdItoT64 stdItoA
#endif


		std::string stdExpandEnvironmentStringsA(LPCSTR pStr);
		std::wstring stdExpandEnvironmentStringsW(LPCWSTR pStr);
#ifdef UNICODE
#define stdExpandEnvironmentStrings stdExpandEnvironmentStringsW
#else
#define stdExpandEnvironmentStrings stdExpandEnvironmentStringsA
#endif



		std::wstring stdGetEnvironmentVariableW(LPCWSTR pStr);
#ifdef UNICODE
#define stdGetEnvironmentVariable stdGetEnvironmentVariableW
#else
#define stdGetEnvironmentVariable stdGetEnvironmentVariableA
#endif



		bool stdGetUnittedSizeW(LPCWSTR pStr, int& nSign, __int64& lResult, int* pUnit = NULL);
#ifdef UNICODE
#define stdGetUnittedSize stdGetUnittedSizeW
#else
#define stdGetUnittedSize stdGetUnittedSizeA
#endif



//		std::wstring stdSplitCommandLineW(int argc, int startargc, LPCWSTR* argv);
//#ifdef UNICODE
//#define stdSplitCommandLine stdSplitCommandLineW
//#else
//#define stdSplitCommandLine stdSplitCommandLineA
//#endif


		std::wstring stdAddBackSlash(const std::wstring& d);


		// broken
		// std::wstring stdEncodeUrl(const std::wstring& d);
		// std::wstring stdEncodeUrl(LPCWSTR pData);




		//std::wstring stdGetFullPathName(LPCWSTR pPath);
		//std::wstring stdGetFullPathName(const std::wstring& ws);





#ifdef __cplusplus_cli
		std::wstring getStdWString(System::String^ s);
		std::string getStdString(System::String^ s);
#endif // __cplusplus_cli


		std::string stdToString(const wchar_t * pIN);
		std::string stdToString(const std::wstring& ws);

		std::wstring stdToWstring(const char* pStr);
		std::wstring stdToWstring(const std::string& s);

		std::wstring utf8ToWstring(const char* pStr);
		std::wstring utf8ToWstring(const std::string& s);

		//std::vector<std::wstring> stdSplitString(const std::wstring& str,
		//	const std::wstring& delimiter);
		// std::vector<std::wstring> stdSplitStringToLine(const std::wstring& str);




		//std::wstring StdStringReplaceW(std::wstring str, const std::wstring& from, const std::wstring& to);
		//std::string  StdStringReplaceA(std::string str, const std::string& from, const std::string& to);

		//#ifdef UNICODE
		//#define StdStringReplace StdStringReplaceW
		//#else
		//#define StdStringReplace StdStringReplaceA
		//#endif



		// std::wstring stdGetFirstLine(const std::wstring& str);

		std::wstring stdGetShortPath(const wchar_t* pIN);
		std::wstring stdGetShortPath(const std::wstring& ws);


		std::wstring stdApplyDQ(const std::wstring& ws);

		std::wstring stdGetDlgItemText(HWND hDlg, UINT id);
	}
}
