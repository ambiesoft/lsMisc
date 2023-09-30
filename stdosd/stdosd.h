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

// import Shlwapi.lib by '#pragma comment(lib, "Shlwapi.lib")'

#ifndef STDOSD_INCLUDED
#define STDOSD_INCLUDED

#include <vector>
#include <string>
#include <memory>
#include <regex>
#include <functional>
#include <set>
#include <iomanip>

#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#include <wctype.h>
#include <stdio.h>

#ifdef _WIN32
    #include <Windows.h>
    #include <direct.h>
	#include <tchar.h>
	#include <mbstring.h>
#endif
#if defined(__GNUC__)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER <= 1500)
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;
#else
    /* gcc or VC10 */
    #include <stdint.h>
#endif

#include "../DebugNew.h"

#include "stdosd_literal.h"

#define STDOSD_UNUSED(x) (void)x;

#if __GNUC__
    #define STDOSD_CONSTEXPR const constexpr
    #define CHAR16T_AVAILABLE
    #define STDOSD_ATTR_UNUSED __attribute__((unused))
	#define STDOSD_IS_CASESENSITIVE true
	#define STDOSD_SYSTEM_CHAR_LITERAL(s) s
#elif _WIN32 // not __GNUC__ but _WIN32
    #if _MSC_VER <= 1800  // less than or equal to VC2013 ( or VC12 )
    #define STDOSD_CONSTEXPR const
    #else
    #define STDOSD_CONSTEXPR const constexpr
    #endif

    #if _MSC_VER >= 1900
    #define CHAR16T_AVAILABLE
    #endif

    #define STDOSD_ATTR_UNUSED
	#define STDOSD_IS_CASESENSITIVE false
	#define STDOSD_SYSTEM_CHAR_LITERAL(s) STDOSD_WCHARLITERAL(s)
#endif // _WIN32 __GNUC__


#ifndef stdosd_max
#define stdosd_max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef stdosd_min
#define stdosd_min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace Ambiesoft {
	namespace stdosd {

#ifdef _WIN32
		using SYSTEM_CHAR_TYPE = wchar_t;
#else
		using SYSTEM_CHAR_TYPE = char;
#endif
		using osdstring = std::basic_string<SYSTEM_CHAR_TYPE>;

        template <typename T, std::size_t N>
        constexpr std::size_t stdCountOf(T const (&)[N]) noexcept
        {
            return N;
        }

		// https://stackoverflow.com/a/15208908
		template<typename T, typename U>
		bool stdIsAllEqual(T&& t, U&& u)
		{
			return (t == u);
		}

		template<typename T, typename U, typename... Ts>
		bool stdIsAllEqual(T&& t, U&& u, Ts&&... args)
		{
			return (t == u) && stdIsAllEqual(u, std::forward<Ts>(args)...);
		}

		typedef void* HFILEITERATOR;
		template<typename C>
		class FileDirectoryInfo
		{
			unsigned long long size_ = 0;
			std::basic_string<C> name_;
			std::basic_string<C> path_;
			bool isDir_ = false;
		public:
			unsigned long long size() const {
				return size_;
			}
			std::basic_string<C> name() const {
				return name_;
			}
			bool isDirectory() const {
				return isDir_;
			}

			void setAll(bool isDir,
				const std::basic_string<C>& name,
				const unsigned long long& size)
			{
				isDir_ = isDir;
				name_ = name;
				size_ = size;
			}
		};

		// Use in for loop
		class IsBetweenLoop
		{
			bool called_ = false;
		public:
			operator bool() {
				if (called_)
					return true;
				called_ = true;
				return false;
			}
		};


		template<typename C>
		inline bool isEmptyString(const C* str, size_t len) {
			return (len == 0 || !str || str[0] == 0);
		}
		inline size_t stdStringLength(const char* p) {
			return std::strlen(p);
		}
		inline size_t stdStringLength(const wchar_t* p) {
			return wcslen(p);
		}
		template<typename C>
		inline size_t stdStringLength(const std::basic_string<C>& str) {
			return str.size();
		}

#if defined(_MSC_VER) || defined(__MINGW32__)
		inline static int myStrCaseCmp(const char* left, const char* right) {
			return _strcmpi(left, right);
		}
		inline static int myStrCaseCmpW(const wchar_t* left, const wchar_t* right) {
			return _wcsicmp(left, right);
		}
		inline static int myStrNCaseCmp(const char* left, const char* right, size_t len) {
			return _strnicmp(left, right, len);
		}
		inline static int myStrNCaseCmpW(const wchar_t* left, const wchar_t* right, size_t len) {
			return _wcsnicmp(left, right, len);
		}
#else
		inline static int myStrCaseCmp(const char* left, const char* right) {
			return strcasecmp(left, right);
		}
		inline static int myStrCaseCmpW(const wchar_t* left, const wchar_t* right) {
			return wcscasecmp(left, right);
		}
		inline static int myStrNCaseCmp(const char* left, const char* right, size_t len) {
			return strncasecmp(left, right, len);
		}
		inline static int myStrNCaseCmpW(const wchar_t* left, const wchar_t* right, size_t len) {
			return wcsncasecmp(left, right, len);
		}
#endif

		inline bool isCharEqual(const char* left, const char* right, bool ignoreCase = false) {
			// Linux GNU C dose not have _strcmpi
			return ignoreCase ? myStrCaseCmp(left, right) == 0 : strcmp(left, right) == 0;
		}
		inline bool isCharEqual(const wchar_t* left, const wchar_t* right, bool ignoreCase = false) {
			return ignoreCase ? myStrCaseCmpW(left, right) == 0 : wcscmp(left, right) == 0;
		}

		inline bool isCharEqualN(const char* left, const char* right, size_t len, bool ignoreCase = false) {
			return ignoreCase ? myStrNCaseCmp(left, right, len) == 0 : strncmp(left, right, len) == 0;
		}
		inline bool isCharEqualN(const wchar_t* left, const wchar_t* right, size_t len, bool ignoreCase = false) {
			return ignoreCase ? myStrNCaseCmpW(left, right, len) == 0 : wcsncmp(left, right, len) == 0;
		}



















		template<typename T, typename = std::enable_if_t<
			std::is_arithmetic<T>::value &&
			!std::is_floating_point<T>::value &&
			!std::is_same<T, bool>::value>
		>
		inline bool stdIsAscii(T c) {
			return (0 <= c) && (c <= 127);
		}

		template<typename C>
		inline bool stdIsAsciiString(const C* str, size_t len = -1)
		{
			if (isEmptyString(str, len))
				return false;

			if (len == static_cast<size_t>(-1))
				len = stdStringLength(str);

			for (size_t i = 0; i < len; ++i)
			{
				if (!stdIsAscii(str[i]))
					return false;
			}
			return true;
		}
		inline bool stdIsAsciiString(const std::wstring& s)
		{
			return stdIsAsciiString(s.c_str(), s.size());
		}
		inline bool stdIsAsciiString(const std::string& s)
		{
			return stdIsAsciiString(s.c_str(), s.size());
		}

















		inline bool stdIsAsciiDigit(char c) {
			return ('0' <= c) && (c <= '9');
		}
		inline bool stdIsAsciiDigit(wchar_t c) {
			return (L'0' <= c) && (c <= L'9');
		}

		template<typename C>
		inline bool stdIsAsciiDigitString(const C* str, size_t len = -1)
		{
			if (isEmptyString(str, len))
				return false;

			if (len == static_cast<size_t>(-1))
				len = stdStringLength(str);

			for (size_t i = 0; i < len; ++i)
			{
				if (!stdIsAsciiDigit(str[i]))
					return false;
			}
			return true;
		}
		inline bool stdIsAsciiDigitString(const std::wstring& s)
		{
			return stdIsAsciiDigitString(s.c_str(), s.size());
		}
		inline bool stdIsAsciiDigitString(const std::string& s)
		{
			return stdIsAsciiDigitString(s.c_str(), s.size());
		}





		inline bool stdIsDigit(char c)
		{
			return !!isdigit(c);
		}
		inline bool stdIsDigit(wchar_t c)
		{
			return !!iswdigit(c);
		}
		template<typename C>
		inline bool stdIsDigitString(const C* str, size_t len = -1)
		{
			if (isEmptyString(str, len))
				return false;

			if (len == static_cast<size_t>(-1))
				len = stdStringLength(str);

			for (size_t i = 0; i < len; ++i)
			{
				if (!stdIsDigit(str[i]))
					return false;
			}
			return true;
		}
		inline bool stdIsDigitString(const std::wstring& s)
		{
			return stdIsDigitString(s.c_str(), s.size());
		}
		inline bool stdIsDigitString(const std::string& s)
		{
			return stdIsDigitString(s.c_str(), s.size());
		}

#if defined(_MSC_VER) || defined(__MINGW32__)
		inline char* stdIncCharPtr(const char* p)
		{
			return (char*)_mbsinc((const unsigned char*)p);
		}
		inline wchar_t* stdIncCharPtr(const wchar_t* p)
		{
			// _wcsinc is not defined unless _UNICODE
			return (wchar_t*)(p + 1);
		}
		
		inline bool stdIsLeadChar(const char* p)
		{
			return _ismbslead((const unsigned char*)p, (const unsigned char*)p) == -1;
		}
		inline bool stdIsLeadChar(const wchar_t*)
		{
			return false;
		}
#else
		template<typename C>
		C* stdIncCharPtr(const C* p)
		{
			return ++p;
		}

		template<typename C>
		bool stdIsLeadChar(const C* p)
		{
			return false;
		}
#endif

		template<class C>
		const C* stdSkipSpace(const C* p)
		{
			while (*p)
			{
				if (stdIsAsciiSpace(*p))
					++p;
				else
					return p;
			}
			return p;
		}

		template<class C>
		const C* stdSkipUntillSpace(const C* p)
		{
			while (*p)
			{
				if (!stdIsAsciiSpace(*p))
					p++;
				else
					return p;
			}
			return p;
		}

		inline const wchar_t* getRChar(const wchar_t* p, wchar_t c)
		{
			return wcsrchr(p, c);
		}
		inline const char* getRChar(const char* p, char c)
		{
			return strrchr(p, c);
		}

		template<typename C>
		inline const C* getOneofRChar(const C* pOriginal, const C* pOneof)
		{
			const C* pRet = nullptr;
			// const C* pOriginal = pOneof;
			for (const C* pC = pOneof; *pC; ++pC)
			{
				const C* pT = getRChar(pOriginal, *pC);
				if (pT)
					++pT;
				if (pT > pRet)
					pRet = pT;
			}
			return pRet;
		}

		template<typename C>
		inline const C* stdGetFileName(const C* pPath)
		{
			if (!pPath)
				return nullptr;
			const C* pSeparator = getOneofRChar(pPath, stdLiterals<C>::defaultPathSeparatorStrings());
			if (!pSeparator)
				return pPath;

			return pSeparator;
		}
		inline std::string stdGetFileName(const std::string& w)
		{
			return stdGetFileName(w.c_str());
		}
		inline std::wstring stdGetFileName(const std::wstring& w)
		{
			return stdGetFileName(w.c_str());
		}



		template<typename C>
		inline const C* stdGetFileExtension(const C* pPath)
		{
			if (!pPath)
				return nullptr;

			const C* pFilename = stdGetFileName(pPath);
			if (!pFilename)
				return nullptr;

			const C* pExt = getRChar(pFilename, stdLiterals<C>::period);
			if (!pExt)
				return stdLiterals<C>::nulString();

			return pExt;
		}
		inline std::string stdGetFileExtension(const std::string& w)
		{
			return stdGetFileExtension(w.c_str());
		}
		inline std::wstring stdGetFileExtension(const std::wstring& w)
		{
			return stdGetFileExtension(w.c_str());
		}


		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C>> 
			stdGetFileNameWithoutExtension(const C* pPath)
		{
			if (!pPath)
                return std::basic_string<C>();

			const C* pFilename = stdGetFileName(pPath);
			if (!pFilename)
                return std::basic_string<C>();

			const C* pExt = getRChar(pFilename, stdLiterals<C>::period);
			if (!pExt)
				return pFilename;

            std::basic_string<C> ret;
            size_t len = (pExt - pFilename);
            ret.assign(pFilename, len);
			return ret;
		}
		inline std::string stdGetFileNameWithoutExtension(const std::string& w)
		{
			return stdGetFileNameWithoutExtension(w.c_str());
		}
		inline std::wstring stdGetFileNameWithoutExtension(const std::wstring& w)
		{
			return stdGetFileNameWithoutExtension(w.c_str());
		}


		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C>>
			stdRemoveExtension(const C* pPath)
		{
			if (!pPath)
				return std::basic_string<C>();

			const C* pExt = getRChar(pPath, stdLiterals<C>::period);
			if (!pExt)
				return pPath;

			std::basic_string<C> ret;
			size_t len = (pExt - pPath);
			ret.assign(pPath, len);
			return ret;
		}
		inline std::string stdRemoveExtension(const std::string& w)
		{
			return stdRemoveExtension(w.c_str());
		}
		inline std::wstring stdRemoveExtension(const std::wstring& w)
		{
			return stdRemoveExtension(w.c_str());
		}






		template<typename C>
		inline bool isEndwithSeparator(const std::basic_string<C, std::char_traits<C>, std::allocator<C>>& s)
		{
			if (s.empty())
				return false;

			C lastChar = s[s.length() - 1];
			for (const C* p = stdLiterals<C>::defaultPathSeparatorStrings(); *p; ++p)
			{
				if (lastChar == *p)
					return true;
			}
			return false;
		}

		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C> >
			stdGetParentDirectory(const C* pPath, bool bAddSeparator = false)
		{
			using mys = std::basic_string<C, std::char_traits<C>, std::allocator<C>>;

			if (!pPath || pPath[0] == 0)
				return mys();

			mys s(pPath);
			size_t len = s.size();
			if (isEndwithSeparator(s))
			{
				s = s.substr(0, len - 1);
			}

			const C* pStart = s.c_str();
			const C* pSeparator = getOneofRChar(s.c_str(), stdLiterals<C>::defaultPathSeparatorStrings());
			if (!pSeparator)
			{
				return mys();
			}

			if (!bAddSeparator)
				s.assign(pStart, pSeparator - 1);
			else
				s.assign(pStart, pSeparator);

			return s;
		}
		inline std::string stdGetParentDirectory(const std::string& w, bool bAddSeparator = false)
		{
			return stdGetParentDirectory(w.c_str(), bAddSeparator);
		}
#ifdef _WIN32
		inline std::wstring stdGetParentDirectory(const std::wstring& w, bool bAddSeparator = false)
		{
			return stdGetParentDirectory(w.c_str(), bAddSeparator);
		}
#endif



		// TODO: test with Linux
		template<typename C>
		inline bool stdIsFullPath(const C* pD, bool allownetwork)
		{
			if (!pD || pD[0] == 0)
				return false;

			if (pD[0] == stdLiterals<C>::NSlash)
				return true;

			
			if (!((stdLiterals<C>::Na <= pD[0] && pD[0] <= stdLiterals<C>::Nz) ||
				(stdLiterals<C>::NA <= pD[0] && pD[0] <= stdLiterals<C>::NZ)))
			{
				if (!allownetwork)
					return false;

				if (pD[1] == 0)
					return false;
				
				if (!(pD[0] == stdLiterals<C>::NBackSlash && pD[1] == stdLiterals<C>::NBackSlash))
					return false;

				return pD[2] != 0;
			}

			if (pD[1] == stdLiterals<C>::NColon)
				return true;

			return false;
		}
		inline bool stdIsFullPath(const std::string& w, bool allownetwork = true)
		{
			return stdIsFullPath(w.c_str(), allownetwork);
		}
		inline bool stdIsFullPath(const std::wstring& w, bool allownetwork = true)
		{
			return stdIsFullPath(w.c_str(), allownetwork);
		}



		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C>>
		stdCombinePath(const C* pD1, const C* pD2)
		{
			using myS = std::basic_string<C, std::char_traits<C>, std::allocator<C>>;
			if (!pD1 || !pD1[0])
				return pD2;

			if (!pD2 || !pD2[0])
				return pD1;

			if (stdIsFullPath(pD2))
				return pD2;

			myS ret = pD1;
			if (!isEndwithSeparator(ret))
				ret += stdLiterals<C>::defaultPathSeparatorString(); // '\\'; // TODO to make this template

			ret += pD2;
			return ret;
		}
		inline std::wstring stdCombinePath(const std::wstring& d1, const std::wstring& d2)
		{
			return stdCombinePath(d1.c_str(), d2.c_str());
		}
		inline std::string stdCombinePath(const std::string& d1, const std::string& d2)
		{
			return stdCombinePath(d1.c_str(), d2.c_str());
		}


		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C>>
			stdAddPathSeparator(const C* pD)
		{
			using myS = std::basic_string<C, std::char_traits<C>, std::allocator<C>>;
			if (!pD || !pD[0])
				return stdLiterals<C>::defaultPathSeparatorString();

			myS ret = pD;
			if (!isEndwithSeparator(ret))
				ret += stdLiterals<C>::defaultPathSeparatorString();

			return ret;
		}
		inline std::wstring stdAddPathSeparator(const std::wstring& d = stdLiterals<wchar_t>::defaultPathSeparatorStrings())
		{
			return stdAddPathSeparator(d.c_str());
		}
		inline std::string stdAddPathSeparator(const std::string& d = stdLiterals<char>::defaultPathSeparatorStrings())
		{
			return stdAddPathSeparator(d.c_str());
		}


		template<typename StringType>
		inline StringType stdStringReplaceHelper(
			StringType str,
			const StringType& from,
			const StringType& to)
		{
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != StringType::npos)
			{
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
			return str;
		}
		inline std::string stdStringReplace(
			std::string str,
			const std::string& from,
			const std::string& to)
		{
			return stdStringReplaceHelper(str, from, to);
		}
		inline std::wstring stdStringReplace(
			std::wstring str,
			const std::wstring& from,
			const std::wstring& to)
		{
			return stdStringReplaceHelper(str, from, to);
		}
		inline std::string stdStringReplace(
			std::string str,
			char from,
			char to)
		{
			return stdStringReplaceHelper(str, std::string(1, from), std::string(1, to));
		}
		inline std::wstring stdStringReplace(
			std::wstring str,
			wchar_t from,
			wchar_t to)
		{
			return stdStringReplaceHelper(str, std::wstring(1, from), std::wstring(1, to));
		}


		template<typename C>
		inline std::basic_string<C> stdToCRLFString(const std::basic_string<C>& str)
		{
			std::basic_string<C> ret = stdStringReplace(str, stdLiterals<C>::CRLF(), stdLiterals<C>::LF());
			ret = stdStringReplace(ret, stdLiterals<C>::CR(), stdLiterals<C>::LF());
			ret = stdStringReplace(ret, stdLiterals<C>::LF(), stdLiterals<C>::CRLF());
			
			return ret;
		}


		// https://stackoverflow.com/a/13172514
		template<typename StringType>
		inline std::vector<StringType> stdSplitStringHelper(
			const StringType& str,
			const StringType& delimiter)
		{
			std::vector<StringType> strings;

			size_t pos = 0;
			size_t prev = 0;
			while ((pos = str.find(delimiter, prev)) != StringType::npos)
			{
				strings.push_back(str.substr(prev, pos - prev));
				prev = pos + 1;
			}

			// To get the last substring (or only, if delimiter is not found)
			strings.push_back(str.substr(prev));

			return strings;
		}
		inline std::vector<std::string> stdSplitString(
			const std::string& str,
			const std::string& delimiter)
		{
			return stdSplitStringHelper(str, delimiter);
		}
        inline std::vector<std::wstring> stdSplitString(
            const std::wstring& str,
            const std::wstring& delimiter)
        {
            return stdSplitStringHelper(str, delimiter);
        }
        inline std::vector<std::u16string> stdSplitString(
            const std::u16string& str,
            const std::u16string& delimiter)
        {
            return stdSplitStringHelper(str, delimiter);
        }


		template<typename StringType>
		inline std::vector<StringType> stdSplitStringToLineHelper(const StringType& str)
		{
			using C = typename StringType::traits_type::char_type;

			StringType t = stdStringReplace(
				stdStringReplace(
					str,
					stdLiterals<C>::CRLF(),
					stdLiterals<C>::LF()),
				stdLiterals<C>::CR(),
				stdLiterals<C>::LF());

			return stdSplitString(t, stdLiterals<C>::LF());
		}
		inline std::vector<std::string> stdSplitStringToLine(const std::string& str)
		{
			return stdSplitStringToLineHelper(str);
		}
		inline std::vector<std::wstring> stdSplitStringToLine(const std::wstring& str)
		{
			return stdSplitStringToLineHelper(str);
		}


// varargs not supported under /clr
#ifndef __cplusplus_cli
		inline int stdSprintF(char *buffer,
							  size_t sizeOfBuffer,
							  size_t count,
							  const char *format,
							  va_list argptr)
		{
#if defined(__MINGW32__)

            // (void)count;
            return vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr);
#elif defined(__GNUC__)
            STDOSD_UNUSED(count);
            return vsnprintf(buffer, sizeOfBuffer, format, argptr);

#elif _WIN32
#if _MSC_VER <= 1400
			int n = _vsnwprintf((wchar_t *)str.data(), size, fmt.c_str(), ap);
#else
			return _vsnprintf_s(buffer,sizeOfBuffer,count,format,argptr);
#endif // _WIN32
#endif // __GUNC__
		}
		inline int stdSprintF(wchar_t *buffer,
							  size_t sizeOfBuffer,
							  size_t count,
							  const wchar_t *format,
							  va_list argptr)
		{
#if defined(__MINGW32__)
            STDOSD_UNUSED(count);
            return vswprintf_s(buffer, sizeOfBuffer, format, argptr);
#elif defined(__GNUC__)
            STDOSD_UNUSED(count);
            return vswprintf(buffer,sizeOfBuffer,format,argptr);

#elif _WIN32
#if _MSC_VER <= 1400
			int n = _vsnwprintf((wchar_t *)str.data(), size, fmt.c_str(), ap);
#else
			return _vsnwprintf_s(buffer,sizeOfBuffer,count,format,argptr);
#endif // _WIN32
#endif // __GUNC__
		}


		template<typename C>
		inline std::basic_string<C> stdFormatHelper(const C* fmt, size_t size, ...)
		{
            int newsize = (static_cast<int>(size)) * 2 + 50;   // Use a rubric appropriate for your code
			std::basic_string<C> strRet;
			va_list ap;
			while (true) {     // Maximum two passes on a POSIX system...
				strRet.resize(newsize);
				va_start(ap, size);

                int n = stdSprintF(const_cast<C*>(strRet.data()), newsize, newsize - 1, fmt, ap);

				va_end(ap);

				if (n > -1 && n < newsize) {  // Everything worked
					strRet.resize(n);
					return strRet;
				}
				if (n > -1)  // Needed size returned
					newsize = n + 1;   // For null char
				else
					newsize *= 2;      // Guess at a larger size (OS specific)
			}
			return strRet;
		}

		// check argument of stdFormat is not std::string or std::wstring
        static void STDOSD_ATTR_UNUSED stdFormatTestForNotString() {}
		template<typename T, typename... ARGS>
		static void stdFormatTestForNotString(T, ARGS... args)
		{
            static_assert(!std::is_same<T, std::string>::value, "argument must not std::string");
            static_assert(!std::is_same<T, std::wstring>::value, "argument must not std::wstring");
			stdFormatTestForNotString(args...);
		}

        static void STDOSD_ATTR_UNUSED stdFormatTestForNotWChar() {}
		template<typename T, typename... ARGS>
        static void stdFormatTestForNotWChar(T, ARGS... args)
		{
            using NoConstT = typename std::add_pointer<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::type;
            static_assert(
                std::is_same<std::remove_cv_t<T>, wchar_t>::value ||
                !std::is_same<NoConstT, wchar_t*>::value,
                "argument must not wchar_t*");
            static_assert(!std::is_pointer<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::value,
                        "argument must not pointer of pointer");
            stdFormatTestForNotWChar(args...);
		}

        static void STDOSD_ATTR_UNUSED stdFormatTestForNotChar() {}
		template<typename T, typename... ARGS>
		static void stdFormatTestForNotChar(T, ARGS... args)
		{
            using NoConstT = typename std::add_pointer<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::type;
			static_assert(
                std::is_same<std::remove_cv_t<T>, char>::value ||
                !std::is_same<NoConstT, char*>::value,
				"argument must not char*");
            static_assert(!std::is_pointer<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::value,
                        "argument must not pointer of pointer");
			stdFormatTestForNotChar(args...);
		}


		template<typename... ARGS>
		inline std::string stdFormat(const std::string& fmt, ARGS... args)
		{
			stdFormatTestForNotString(args...);
			stdFormatTestForNotWChar(args...);
			return stdFormatHelper(fmt.c_str(), fmt.size(), args...);
		}
		template<typename... ARGS>
		inline std::wstring stdFormat(const std::wstring& fmt, ARGS... args)
		{
			stdFormatTestForNotString(args...);
			stdFormatTestForNotChar(args...);
			return stdFormatHelper(fmt.c_str(), fmt.size(), args...);
		}
#endif // __cplusplus_cli








		template<typename C>
		inline bool hasEnding(const C* fullString, const C* ending, 
			size_t fullLen = -1, size_t endLen = -1,
			bool ignoreCase = false) {
			if (fullString == nullptr)
				return false;
			
			if (ending == nullptr || ending[0] == 0)
				return true;

            if (fullLen == static_cast<size_t>(-1))
				fullLen = stdStringLength(fullString);
            if (endLen == static_cast<size_t>(-1))
				endLen = stdStringLength(ending);

			if (fullLen < endLen)
				return false;
			
			const C* pFullStartPos = fullString + (fullLen - endLen);
			return isCharEqual(pFullStartPos, ending, ignoreCase);
		}
		template<typename C>
		inline bool hasEndingI(const C* fullString, const C* ending,
			size_t fullLen = -1, size_t endLen = -1) {
			return hasEnding(fullString, ending,
				fullLen, endLen, true);
		}
		inline bool hasEnding(const std::string& fullString, const std::string& ending) {
			return hasEnding(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEndingI(const std::string& fullString, const std::string& ending) {
			return hasEndingI(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEnding(const std::wstring& fullString, const std::wstring& ending) {
			return hasEnding(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEndingI(const std::wstring& fullString, const std::wstring& ending) {
			return hasEndingI(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}


		template<typename C>
		inline bool hasStarting(const C* fullString, const C* starting,
			size_t fullLen = -1, size_t startLen = -1,
			bool ignoreCase = false) {
			if (fullString == nullptr)
				return false;

			if (starting == nullptr || starting[0] == 0)
				return true;

            if (fullLen == static_cast<size_t>(-1))
				fullLen = stdStringLength(fullString);
            if (startLen == static_cast<size_t>(-1))
				startLen = stdStringLength(starting);

			if (fullLen < startLen)
				return false;

			return isCharEqualN(fullString, starting, startLen, ignoreCase);
		}
		template<typename C>
		inline bool hasStartingI(const C* fullString, const C* starting,
			size_t fullLen = -1, size_t startLen = -1) {
			return hasStarting(fullString, starting,
				fullLen, startLen, true);
		}
		inline bool hasStarting(const std::string& fullString, const std::string& starting) {
			return hasStarting(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStartingI(const std::string& fullString, const std::string& starting) {
			return hasStartingI(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStarting(const std::wstring& fullString, const std::wstring& starting) {
			return hasStarting(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStartingI(const std::wstring& fullString, const std::wstring& starting) {
			return hasStartingI(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}

        std::basic_string<SYSTEM_CHAR_TYPE> stdGetFullPathName(const SYSTEM_CHAR_TYPE* p);
        inline std::basic_string<SYSTEM_CHAR_TYPE> stdGetFullPathName(const std::basic_string<SYSTEM_CHAR_TYPE>& s)
		{
			return stdGetFullPathName(s.c_str());
		}
        std::basic_string<SYSTEM_CHAR_TYPE> resolveLink(const std::basic_string<SYSTEM_CHAR_TYPE>& fullstring);

		template<typename C>
		inline bool stdIsDQNecessary(const C* fullString, size_t size = static_cast<size_t>(-1))
		{
			if (size == static_cast<size_t>(-1))
				size = stdStringLength(fullString);

			if (size == 0)
				return false;

			if (fullString[0] == stdLiterals<C>::NDoubleQuote &&
				fullString[size - 1] == stdLiterals<C>::NDoubleQuote)
				return false;

			bool hasSpace = false;
			for (size_t i = 0; i < size; ++i)
			{
				if (fullString[i] == stdLiterals<C>::NSpace)
				{
					hasSpace = true;
					break;
				}
			}

			if (!hasSpace)
				return false;

			return true;
		}
		template<typename C>
		inline bool stdIsDQNecessary(const std::basic_string<C>& fullString)
		{
			return stdIsDQNecessary(fullString.c_str(), fullString.size());
		}

		template<typename C>
        inline std::basic_string<C> stdAddDQIfNecessary(const C* fullString, size_t size = static_cast<size_t>(-1)) 
		{
            if (size == static_cast<size_t>(-1))
				size = stdStringLength(fullString);

			if (!stdIsDQNecessary(fullString, size))
				return fullString;

			return std::basic_string<C>() + stdLiterals<C>::NDoubleQuote + fullString + stdLiterals<C>::NDoubleQuote;
		}
		inline std::string stdAddDQIfNecessary(const std::string& fullString) {
			return stdAddDQIfNecessary(fullString.c_str(), fullString.size());
		}
		inline std::wstring stdAddDQIfNecessary(const std::wstring& fullString) {
			return stdAddDQIfNecessary(fullString.c_str(), fullString.size());
		}


		template<typename C>
		inline bool stdIsAsciiAlpha(const C c) {
			// Lowercase
			if (stdLiterals<C>::Na <= c && c <= stdLiterals<C>::Nz)
				return true;

			// Uppercase
			if (stdLiterals<C>::NA <= c &&c <= stdLiterals<C>::NZ)
				return true;

			return false;
		}

        template<typename C>
        inline bool stdIsAsciiAlphaString(const C* str, size_t len = -1)
        {
            if (isEmptyString(str, len))
                return false;

            if (len == static_cast<size_t>(-1))
                len = stdStringLength(str);

            for (size_t i = 0; i < len; ++i)
            {
                if (!stdIsAsciiAlpha(str[i]))
                    return false;
            }
            return true;
        }
        inline bool stdIsAsciiAlphaString(const std::wstring& s)
        {
            return stdIsAsciiAlphaString(s.c_str(), s.size());
        }
        inline bool stdIsAsciiAlphaString(const std::string& s)
        {
            return stdIsAsciiAlphaString(s.c_str(), s.size());
        }



		template<typename C>
		inline bool stdIsAsciiSpace(const C c) {
			return c == stdLiterals<C>::NSpace;
		}


		inline void stdCopyString(char* pDst, size_t size, const char* pSrc)
		{
#ifdef _WIN32
			strcpy_s(pDst, size, pSrc);
#else
            STDOSD_UNUSED(size);
            std::strcpy(pDst, pSrc);
#endif
		}

		inline void stdCopyString(wchar_t* pDst, size_t size, const wchar_t* pSrc)
		{
#ifdef _WIN32
			wcscpy_s(pDst, size, pSrc);
#else
            STDOSD_UNUSED(size);
            wcscpy(pDst, pSrc);
#endif
		}


		template<typename C>
		inline C* stdStringLower(C* pD1, size_t size)
		{
            STDOSD_UNUSED(pD1);
            STDOSD_UNUSED(size);
            static_assert(sizeof(C)==0, "false");
		}
		template<>
		inline char* stdStringLower(char* pc, size_t size)
		{
#ifdef _WIN32
			_strlwr_s(pc, size + 1);
#else
            STDOSD_UNUSED(size);
            for ( char* p = pc; *p; ++p)
                *p = tolower(*p);
#endif
			return pc;
		}

		template<>
		inline wchar_t* stdStringLower(wchar_t* pwc, size_t size)
		{
#ifdef _WIN32
			_wcslwr_s(pwc, size + 1);
#else
            STDOSD_UNUSED(size);
            for ( wchar_t* pw = pwc ; *pw; ++pw)
                *pw = towlower(*pw);
#endif
			return pwc;
		}

		template<class C>
		inline std::basic_string<C> stdStringLower(const std::basic_string<C>& str)
		{
			std::unique_ptr<C[]> ptr(new C[str.size() + 1]);
			stdCopyString(ptr.get(), str.size() + 1, str.c_str());
			return stdStringLower(ptr.get(), str.size());
		}



		template<typename C>
		inline C* stdStringUpper(C* pD1, size_t size)
		{
            STDOSD_UNUSED(pD1);
            STDOSD_UNUSED(size);
            static_assert(sizeof(C) == 0, "false");
		}
		template<>
		inline char* stdStringUpper(char* pc, size_t size)
		{
#ifdef _WIN32
			_strupr_s(pc, size + 1);
#else
            STDOSD_UNUSED(size);
            for ( char* p = pc; *p; ++p)
                *p = toupper(*p);
#endif
			return pc;
		}

		template<>
		inline wchar_t* stdStringUpper(wchar_t* pwc, size_t size)
		{
#ifdef _WIN32
			_wcsupr_s(pwc, size + 1);
#else
            STDOSD_UNUSED(size);
            for ( wchar_t* pw = pwc; *pw; ++pw)
                *pw = towupper(*pw);
#endif
			return pwc;
		}

		template<class C>
		inline std::basic_string<C> stdStringUpper(const std::basic_string<C>& str)
		{
			std::unique_ptr<C[]> ptr(new C[str.size() + 1]);
			stdCopyString(ptr.get(), str.size() + 1, str.c_str());
			return stdStringUpper(ptr.get(), str.size());
		}











		struct FILEITERATEMODE
		{
			enum Type {
				SKIP_NONE = 0x0,
				SKIP_DOT = 0x1,
				SKIP_DOTDOT = 0x2,
				SKIP_DOT_AND_DOTDOT = SKIP_DOT | SKIP_DOTDOT,
			};
		private:
			using U = typename std::underlying_type<Type>::type;
		public:
			constexpr FILEITERATEMODE(const U val) noexcept
				: val_(static_cast<Type>(val))
			{ }
			constexpr operator U() const { return val_; }
			Type val_;
		};
		struct GETFILESEMODE
		{
			enum Type {
				FILE = 0x1,
				DIRECTORY = 0x2,
				FILE_AND_DIRECTORY = 0x3,
			};
		private:
			using U = typename std::underlying_type<Type>::type;
		public:
			constexpr GETFILESEMODE(const U val) noexcept
				: val_(static_cast<Type>(val))
			{ }
			constexpr operator U() const { return val_; }
			Type val_;
		};

        HFILEITERATOR stdCreateFileIterator(
			const std::string& directory,
			FILEITERATEMODE fim = FILEITERATEMODE::SKIP_NONE,
			int depth = -1);
#ifdef _WIN32
		HFILEITERATOR stdCreateFileIterator(
			const std::wstring& directory,
			FILEITERATEMODE fim = FILEITERATEMODE::SKIP_NONE,
			int depth = -1);
#endif
		namespace detail {
			bool stdFileNextImpl(HFILEITERATOR hFileIterator, FileDirectoryInfo<char>* fi);
#ifdef _WIN32
			bool stdFileNextImpl(HFILEITERATOR hFileIterator, FileDirectoryInfo<wchar_t>* fi);
#endif
		}

		template<typename C>
		inline bool stdFileNext(HFILEITERATOR hFileIterator, FileDirectoryInfo<C>* fi)
		{
			if (!hFileIterator)
				return false;

			if (!detail::stdFileNextImpl(hFileIterator, fi))
				return false;

			return true;
		}
		bool stdCloseFileIterator(HFILEITERATOR hFileIterator);




		template<typename C>
		inline std::basic_string<C> stdTrimStart(
			const std::basic_string<C>& str,
			const C* whitespace = stdLiterals<C>::WHITESPACE(),
			size_t len = std::basic_string<C>::npos)
		{
            using ST = std::basic_string<C>;

			if (whitespace == nullptr || whitespace[0] == 0)
				return str;
			if (len == ST::npos)
				len = stdStringLength(whitespace);
			const typename ST::size_type strBegin =
				str.find_first_not_of(ST(whitespace, len));
			if (strBegin == ST::npos)
				return ST(); // no content

			return str.substr(strBegin);
		}
		template<typename C>
		inline std::basic_string<C> stdTrimStart(
			const std::basic_string<C>& str,
			const std::basic_string<C>& whitespace)
		{
			return stdTrimStart(str, whitespace.c_str(), whitespace.size());
		}
		template<typename C>
		inline std::basic_string<C> stdTrimStart(
			const std::basic_string<C>& str,
			C whitespace)
		{
			return stdTrimStart(str, &whitespace, 1);
		}

		template<typename C>
		inline std::basic_string<C> stdTrimEnd(
			const std::basic_string<C>& str,
			const C* whitespace = stdLiterals<C>::WHITESPACE(),
			size_t len = std::basic_string<C>::npos)
		{
			using ST = std::basic_string<C>;

			if (whitespace == nullptr || whitespace[0] == 0)
				return str;
			if (len == ST::npos)
				len = stdStringLength(whitespace);
			const typename ST::size_type strEnd =
				str.find_last_not_of(ST(whitespace, len));
			
			return str.substr(0, strEnd + 1);
		}
		template<typename C>
		inline std::basic_string<C> stdTrimEnd(
			const std::basic_string<C>& str,
			const std::basic_string<C>& whitespace)
		{
			return stdTrimEnd(str, whitespace.c_str(), whitespace.size());
		}
		template<typename C>
		inline std::basic_string<C> stdTrimEnd(
			const std::basic_string<C>& str,
			C whitespace)
		{
			return stdTrimEnd(str, &whitespace, 1);
		}

		template<typename C>
		inline std::basic_string<C> stdTrim(
			const std::basic_string<C>& str,
			const C* whitespace = stdLiterals<C>::WHITESPACE(),
			size_t len = std::basic_string<C>::npos)
		{
			using ST = std::basic_string<C>;

			if (whitespace == nullptr || whitespace[0] == 0)
				return str;
			if (len == ST::npos)
				len = stdStringLength(whitespace);
			const typename ST::size_type strBegin = 
				str.find_first_not_of(ST(whitespace, len));
			if (strBegin == ST::npos)
				return ST(); // no content

			const typename ST::size_type strEnd = str.find_last_not_of(whitespace);
			const typename ST::size_type strRange = strEnd - strBegin + 1;

			return str.substr(strBegin, strRange);
		}
		template<typename C>
		inline std::basic_string<C> stdTrim(
			const std::basic_string<C>& str,
			const std::basic_string<C>& whitespace)
		{
			return stdTrim(str, whitespace.c_str(), whitespace.size());
		}
		template<typename C>
		inline std::basic_string<C> stdTrim(
			const std::basic_string<C>& str,
			C whitespace)
		{
			return stdTrim(str, &whitespace, 1);
		}




		typedef void* HMODULEINSTANCE;
		namespace detail {
			size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, char* p, size_t size);
#if defined(_WIN32)
			size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, wchar_t* p, size_t size);
#endif
		}
		template<typename C = SYSTEM_CHAR_TYPE>
		inline std::basic_string<C> stdGetModuleFileName(HMODULEINSTANCE hInst = NULL)
		{
			C* p = nullptr;
			size_t size = 64;
			for (;;)
			{
				p = (C*)realloc(p, size * sizeof(C));
				if (detail::stdGetModuleFileNameImpl(hInst, p, size) < size)
					break;

				// Make double the size of required memory
				size *= 2;
			}

			std::basic_string<C> ret = p;
			free((void*)p);
			return ret;
		}


#if defined(_WIN32)
		typedef HWND HWINDOWHANDLE;
#endif

#if defined(_WIN32)
//		template<typename C>
//		inline bool stdGetClipboardText(HWINDOWHANDLE hWindow, std::basic_string<C>& result)
//		{
//            static_assert(sizeof(hWindow)==0, "needs specific implement");
//        }


		bool stdGetWindowText(HWINDOWHANDLE hWindow, std::wstring* result);
		bool stdSetWindowText(HWINDOWHANDLE hWindow, const std::wstring& text);

		//template<typename C>
		//inline C* stdStrDup(const C* p)
		//{
		//	if (!p)
		//		return nullptr;
		//	size_t count = getCharLength(p) + 1;
		//	C* ret = new C[count];
		//	stdCopyString(ret, count, p);
		//	return ret;
		//}
#endif
		template<typename C>
		inline int stdStrCmp(const C* p1, const C* p2, bool ignorecase = false)
		{
            STDOSD_UNUSED(p1)
            STDOSD_UNUSED(p2)
            STDOSD_UNUSED(ignorecase)
            static_assert(sizeof(C) == 0, "false");
            return 0;
		}
		template<>
		inline int stdStrCmp(const char* p1, const char* p2, bool ignorecase)
		{
#ifdef _WIN32
			return ignorecase ? _stricmp(p1, p2) : strcmp(p1, p2);
#else
            return ignorecase ? strcasecmp(p1, p2) : strcmp(p1, p2);
#endif
		}
#ifdef _WIN32
		template<>
		inline int stdStrCmp(const wchar_t* p1, const wchar_t* p2, bool ignorecase)
		{
			return ignorecase ? _wcsicmp(p1, p2) : wcscmp(p1, p2);

		}
#endif
		namespace detail {
			size_t stdExpandEnvironmentStringsImpl(const char* pIN, char* p, size_t size);
			size_t stdExpandEnvironmentStringsImpl(const wchar_t* pIN, wchar_t* p, size_t size);
		}
		template<typename C = wchar_t>
		inline std::basic_string<C> stdExpandEnvironmentStrings(const C* pIN)
		{
			C* p = nullptr;
			size_t size = 64;
			for (;;)
			{
				p = (C*)realloc(p, size * sizeof(C));
				if (detail::stdExpandEnvironmentStringsImpl(pIN, p, size) < size)
					break;

				// Make double the size of required memory
				size *= 2;
			}

			std::basic_string<C> ret = p;
			free((void*)p);
			return ret;
		}
		template<typename C = wchar_t>
		inline std::basic_string<C> stdExpandEnvironmentStrings(const std::basic_string<C>& str)
		{
			return stdExpandEnvironmentStrings(str.c_str());
		}

		template<typename C = wchar_t>
		std::basic_string<C> stdGetComputerName();
        extern template std::basic_string<char> stdGetComputerName();
        extern template std::basic_string<wchar_t> stdGetComputerName();

		template<typename S>
		inline S stdXmlEncode(const S& s)
		{
			using C = typename S::value_type;
			S result = s;
			result = stdStringReplace(result, S(1, stdLiterals<C>::NAmpasand), stdLiterals<C>::XMLAMP());
			result = stdStringReplace(result, S(1, stdLiterals<C>::NSingleQuote), stdLiterals<C>::XMLAPOS());
			result = stdStringReplace(result, S(1, stdLiterals<C>::NDoubleQuote), stdLiterals<C>::XMLQUOTE());
			result = stdStringReplace(result, S(1, stdLiterals<C>::NLess), stdLiterals<C>::XMLLESS());
			result = stdStringReplace(result, S(1, stdLiterals<C>::NGreater), stdLiterals<C>::XMLGREATER());

			return result;
		}
		template<typename C>
		inline std::basic_string<C> stdXmlEncode(const C* p)
		{
			if (!p)
				return std::basic_string<C>();
			return stdXmlEncode(std::basic_string<C>(p));
		}

        template<typename C = SYSTEM_CHAR_TYPE>
        std::basic_string<C> stdGetCurrentDirectory();
        extern template std::basic_string<char> stdGetCurrentDirectory<char>();
        extern template std::basic_string<wchar_t> stdGetCurrentDirectory<wchar_t>();


		namespace detail {
			bool stdGetDesktopDirectoryImpl(std::wstring* p);
			bool stdGetDesktopDirectoryImpl(std::string* p);
		}
		template<typename C = wchar_t>
		inline std::basic_string<C> stdGetDesktopDirectory()
		{
			std::basic_string<C> path;
			if (!detail::stdGetDesktopDirectoryImpl(&path))
				return L"";
			return path;
		}


		template<typename C = wchar_t>
		std::basic_string<C> stdRegexReplace(
			const std::basic_string<C>& input,
			const std::basic_regex<C>& regex,
			std::function< std::basic_string<C>(const std::match_results<typename std::basic_string<C>::const_iterator>& match) > format);
		extern template std::basic_string<char> stdRegexReplace<char>(
			const std::basic_string<char>& input,
			const std::basic_regex<char>& regex,
			std::function< std::basic_string<char>(const std::match_results<std::basic_string<char>::const_iterator>& match) > format);
		extern template std::basic_string<wchar_t> stdRegexReplace<wchar_t>(
			const std::basic_string<wchar_t>& input,
			const std::basic_regex<wchar_t>& regex,
			std::function< std::basic_string<wchar_t>(const std::match_results<std::basic_string<wchar_t>::const_iterator>& match) > format);


		template<class T>
		void stdGetRandomString(T* s, const size_t len);
		extern template void stdGetRandomString<char>(char* s, const size_t len);
		extern template void stdGetRandomString<wchar_t>(wchar_t* s, const size_t len);


		
		template<typename C = wchar_t>
		inline std::basic_string<C> stdRemoveFirstLine(const std::basic_string<C>& str)
		{
			size_t rpos = str.find(stdLiterals<C>::NCarriageReturn);
			size_t nlpos = str.find(stdLiterals<C>::NNewLine);

			if (rpos == std::basic_string<C>::npos && nlpos == std::basic_string<C>::npos)
				return std::basic_string<C>();
			if (rpos == std::basic_string<C>::npos && nlpos != std::basic_string<C>::npos)
				return str.substr(nlpos+1);
			if (rpos != std::basic_string<C>::npos && nlpos == std::basic_string<C>::npos)
				return str.substr(rpos+1);
			return str.substr(stdosd_min(rpos, nlpos)+1);
		}

		template<typename C = wchar_t>
		inline std::basic_string<C> stdGetFirstLine(const std::basic_string<C>& str, const bool skipEmptyLine = false)
		{
			if (skipEmptyLine)
			{
				std::basic_string<C> t = str;
				do
				{
					std::basic_string<C> t2 = stdGetFirstLine(t, false);
					if (!stdTrim(t2).empty())
						return t2;
					t = stdRemoveFirstLine(t);
				} while (!t.empty());
				
				return t;
			}

			size_t rpos = str.find(stdLiterals<C>::NCarriageReturn);
			size_t nlpos = str.find(stdLiterals<C>::NNewLine);

			if (rpos == std::basic_string<C>::npos && nlpos == std::basic_string<C>::npos)
				return str;
			if (rpos == std::basic_string<C>::npos && nlpos != std::basic_string<C>::npos)
				return str.substr(0, nlpos);
			if (rpos != std::basic_string<C>::npos && nlpos == std::basic_string<C>::npos)
				return str.substr(0, rpos);
			return str.substr(0, stdosd_min(rpos, nlpos));
		}


		template<typename C = wchar_t>
		inline std::basic_string<C> stdFillWithZero(const std::basic_string<C>& str, size_t nDigits)
		{
			if (str.size() >= nDigits)
				return str;

			std::basic_string<C> ret(str);
			while (ret.size() < nDigits)
			{
				ret = stdLiterals<C>::N0 + ret;
			}
			return ret;
		}

		template<typename C = wchar_t>
		inline std::basic_string<C> stdJoinStrings(const std::vector<std::basic_string<C>>& strs,
			const C* separator = stdLiterals<C>::JOINSEPARATOR(),
			const C* startparen = stdLiterals<C>::JOINSTARTPAREN(),
			const C* endparen = stdLiterals<C>::JOINENDPAREN())
		{
			std::basic_string<C> ret;
			for (size_t i = 0; i < strs.size(); ++i)
			{
				ret += startparen;
				ret += strs[i];
				ret += endparen;

				if ((i + 1) < strs.size())
				{
					ret += separator;
				}
			}
			return ret;
		}

#if defined(_MSC_VER)
		std::string stdGetenvImpl(const char* varname);
		std::wstring stdGetenvImpl(const wchar_t* varname);

		template<typename C>
		inline std::basic_string<C> stdGetenv(const C* varname)
		{
			return stdGetenvImpl(varname);
		}
#elif defined(__MINGW32__)
        inline std::string stdGetenv(const char* varname)
        {
            return getenv(varname);
        }
#else
        inline std::string stdGetenv(const char* varname)
        {
            return secure_getenv(varname);
        }
#endif

		enum ADD_TO_WHERE {
			TO_TOP,
			TO_TAIL,
		};
		template<typename C>
		inline std::basic_string<C> stdAddToPATHEnvString(const C* addingPath, ADD_TO_WHERE toWhere = TO_TAIL)
		{
			using S = std::basic_string<C>;
			S s = stdGetenv(stdLiterals<C>::PathEnvKey());
			if (s.empty())
				return addingPath;

			if (toWhere == TO_TOP)
			{
				s = S() + addingPath + stdLiterals<C>::defaultEnvPathSeparatorString() + s;
			}
			else if (toWhere == TO_TAIL)
			{
				s = s + stdLiterals<C>::defaultEnvPathSeparatorString() + addingPath;
			}
			return s;
		}


        inline bool stdFileExistsFromMode(unsigned short mode)
		{
			if ((S_IFREG & mode) == 0)
				return false;
			if((S_IFDIR & mode) != 0)
				return false;
			return true;
		}
		template<typename C>
		inline bool stdFileExists(const	C* file)
		{
            STDOSD_UNUSED(file);
            static_assert(sizeof(C) == 0, "false");
            return false;
		}
		template<>
		inline bool stdFileExists(const char* file)
		{
			struct stat buffer;
            return stat(file, &buffer) == 0 && stdFileExistsFromMode(buffer.st_mode);
		}
#ifdef _WIN32
		template<>
		inline bool stdFileExists(const wchar_t* file)
		{
			struct _stat  buffer;
            return _wstat(file, &buffer) == 0 && stdFileExistsFromMode(buffer.st_mode);
		}
#endif
		template<class C>
		inline bool stdFileExists(const std::basic_string<C>& file)
		{
			return stdFileExists(file.c_str());
		}

		template<typename C>
		inline bool stdDirectoryExists(const C* folder)
		{
            STDOSD_UNUSED(folder);
            static_assert(sizeof(C)==0, "false");
            return false;
		}
		template<>
		inline bool stdDirectoryExists(const char* folder)
		{
			struct stat buffer;
			return stat(folder, &buffer) == 0 && (S_IFDIR & buffer.st_mode) != 0;
		}
#ifdef _WIN32
		template<>
		inline bool stdDirectoryExists(const wchar_t* folder)
		{
			struct _stat  buffer;
			return _wstat(folder, &buffer) == 0 && (S_IFDIR & buffer.st_mode) != 0;
		}
#endif
		inline bool stdDirectoryExists(const std::basic_string<char>& path)
		{
			return stdDirectoryExists(path.c_str());
		}
#ifdef _WIN32
		inline bool stdDirectoryExists(const std::basic_string<wchar_t>& path)
		{
			return stdDirectoryExists(path.c_str());
		}
#endif


		template<typename C>
		inline std::basic_string<C> stdGetFullPathExecutable(const C* path)
		{
			std::basic_string<C> envpath = stdGetenv(stdLiterals<C>::PATH());
            std::vector<std::basic_string<C>> vPaths = stdSplitString(envpath, stdLiterals<C>::defaultEnvPathSeparatorString());
			for (auto&& onepath : vPaths)
			{
				std::basic_string<C> full = stdCombinePath(onepath, path);
				if (stdFileExists(full.c_str()))
					return full;
			}
			return std::basic_string<C>();
		}
		template<typename C>
		inline std::basic_string<C> stdGetFullPathExecutable(const std::basic_string<C>& path)
		{
			return stdGetFullPathExecutable(path.c_str());
		}

		template<typename C>
		inline std::vector<C> stdUniqueVector(const std::vector<C>& v)
		{
			std::vector<C> vRet;
			std::set<C> sc;
			for (auto&& item : v)
			{
				if (sc.find(item) != sc.end())
				{
					// already added to vt
					continue;
				}
				sc.insert(item);
				vRet.emplace_back(item);
			}
			return vRet;
		}

		inline bool stdIsFileSystemCaseSensitive()
		{
			return STDOSD_IS_CASESENSITIVE;
		}

		template<typename C>
		inline bool stdIsSamePath(const C* path1, const C* path2)
		{
			std::basic_string<C> full1 = stdGetFullPathName(path1);
			std::basic_string<C> full2 = stdGetFullPathName(path2);

			std::basic_string<C> trimmed1 = stdTrimEnd(full1, stdLiterals<C>::defaultPathSeparatorStrings());
			std::basic_string<C> trimmed2 = stdTrimEnd(full2, stdLiterals<C>::defaultPathSeparatorStrings());

			return stdStrCmp(trimmed1.c_str(), trimmed2.c_str(),
				!stdIsFileSystemCaseSensitive())==0;
		}
		template<typename C>
		inline bool stdIsSamePath(const std::nullptr_t* n1, const C* p2)
		{
            STDOSD_UNUSED(n1)
			if (p2 == nullptr)
				return true;
			if (p2[0] == 0)
				return true;
			return false;
		}
		template<typename C>
		inline bool stdIsSamePath(const C* p1, const std::nullptr_t* n2)
		{
			return stdIsSamePath(n2, p1);
		}
		inline bool stdIsSamePath(const std::nullptr_t*, const std::nullptr_t*)
		{
			return true;
		}

		template<typename C>
		inline bool stdIsSamePath(const std::basic_string<C>& path1, const std::basic_string<C>& path2)
		{
			return stdIsSamePath(path1.c_str(), path2.c_str());
		}
		template<typename C>
		inline bool stdIsSamePath(const std::basic_string<C>& path1, const C* path2)
		{
			return stdIsSamePath(path1.c_str(), path2);
		}
		template<typename C>
		inline bool stdIsSamePath(const C* path1, const std::basic_string<C>& path2)
		{
			return stdIsSamePath(path1, path2.c_str());
		}

		template<typename C>
		inline std::basic_string<C> stdRemoveDoubleQuote(const std::basic_string<C>& text)
		{
			if (text.size() < 2)
				return text;

			if (!(text[0] == stdLiterals<C>::NDoubleQuote &&
				text[text.size() - 1] == stdLiterals<C>::NDoubleQuote))
			{
				return text;
			}
			return text.substr(1, text.size() - 1 - 1);
		}

		inline bool stdIsSpace(char c)
		{
			return isspace(c) != 0;
		}
		inline bool stdIsSpace(wchar_t c)
		{
			return iswspace(c) != 0;
		}

		std::basic_string<SYSTEM_CHAR_TYPE> stdGetProgramName();


		template<typename C, typename T>
		struct class_stdToString
		{
			static_assert(sizeof(T) == 0, "char or wchar_t");
		};
		template<typename T>
		struct class_stdToString<char, T>
		{
			static std::basic_string<char> call(const T& t)
			{
				return std::to_string(t);
			}
		};
		template<>
		struct class_stdToString<char, bool>
		{
			static std::basic_string<char> call(const bool& t)
			{
				return t ? "true" : "false";
			}
		};
		template<typename T>
		struct class_stdToString<wchar_t, T>
		{
			static std::basic_string<wchar_t> call(const T& t)
			{
				return std::to_wstring(t);
			}
		};
		template<>
		struct class_stdToString<wchar_t, bool>
		{
			static std::basic_string<wchar_t> call(const bool& t)
			{
				return t ? L"true" : L"false";
			}
		};

		template<typename C = SYSTEM_CHAR_TYPE, typename T>
		std::basic_string<C> stdToString(const T& t)
		{
			return class_stdToString<C, T>::call(t);
		}

		template<typename R, typename C>
        inline R stdFromString(const C* pStr)
		{
            STDOSD_UNUSED(pStr);
			static_assert(sizeof(C) == 0, "Must be full specialized");
		}
		template<>
		inline int stdFromString<int, char>(const char* pStr)
		{
			return atoi(pStr);
        }
#ifdef _WIN32
		template<>
		inline int stdFromString<int, wchar_t>(const wchar_t* pStr)
        {
            return _wtoi(pStr);
        }
#endif
		template<>
        inline int64_t stdFromString<int64_t, char>(const char* pStr)
		{
#ifdef _WIN32
            return _atoi64(pStr);
#else
            return atoll(pStr);
#endif
		}
#ifdef _WIN32
		template<>
        inline int64_t stdFromString<int64_t, wchar_t>(const wchar_t* pStr)
		{
			return _wtoi64(pStr);
		}
#endif
		template<typename C>
        bool stdGetUnittedSize(const C* pStr, size_t len, int* nSign, int64_t* lResult, int* pUnit = nullptr);
        extern template bool stdGetUnittedSize(const char* pStr, size_t len, int* nSign, int64_t* lResult, int* pUnit);
#ifdef _WIN32
        extern template bool stdGetUnittedSize(const wchar_t* pStr, size_t len, int* nSign, int64_t* lResult, int* pUnit);
#endif
        inline bool stdGetUnittedSize(const std::string& s, int* nSign, int64_t* lResult, int* pUnit = nullptr)
		{
			return stdGetUnittedSize(s.c_str(), s.size(), nSign, lResult, pUnit);
		}
#ifdef _WIN32
        inline bool stdGetUnittedSize(const std::wstring& s, int* nSign, int64_t* lResult, int* pUnit = nullptr)
		{
			return stdGetUnittedSize(s.c_str(), s.size(), nSign, lResult, pUnit);
		}
#endif

		inline std::vector<std::basic_string<SYSTEM_CHAR_TYPE>> stdGetFiles(
			const std::basic_string<SYSTEM_CHAR_TYPE>& directory,
			FILEITERATEMODE fim = FILEITERATEMODE::SKIP_NONE,
			GETFILESEMODE gfm = GETFILESEMODE::FILE_AND_DIRECTORY,
			int depth = -1);

		inline std::vector<std::basic_string<SYSTEM_CHAR_TYPE>> stdGetFiles(
			const SYSTEM_CHAR_TYPE* pDirectory,
			FILEITERATEMODE fim = FILEITERATEMODE::SKIP_NONE,
			GETFILESEMODE gfm = GETFILESEMODE::FILE_AND_DIRECTORY,
			int depth = -1)
		{
			using V = std::vector<std::basic_string<SYSTEM_CHAR_TYPE>>;
			V ret;
			if (depth == 0)
				return ret;
			HFILEITERATOR hIt = stdCreateFileIterator(pDirectory, fim);
			if (!hIt)
				return ret;

			std::unique_ptr<void, std::function<bool(HFILEITERATOR)>> pFreer(hIt,
				stdCloseFileIterator);

			FileDirectoryInfo<SYSTEM_CHAR_TYPE> fi;
			--depth;
			while (stdFileNext(hIt, &fi))
			{
				if (fi.isDirectory())
				{
					// This is done in stdFileNext
					assert(!((fim & FILEITERATEMODE::SKIP_DOT) != 0 &&
						fi.name() == stdLiterals<SYSTEM_CHAR_TYPE>::dotString()));
					assert(!((fim & FILEITERATEMODE::SKIP_DOTDOT) != 0 &&
						fi.name() == stdLiterals<SYSTEM_CHAR_TYPE>::dotdotString()));

					if ((gfm & GETFILESEMODE::DIRECTORY) != 0)
						ret.push_back(stdCombinePath(pDirectory, fi.name()));
					if (depth != 0)
					{
						if (fi.name() != stdLiterals<SYSTEM_CHAR_TYPE>::dotString() &&
							fi.name() != stdLiterals<SYSTEM_CHAR_TYPE>::dotdotString())
						{
							V v = stdGetFiles(stdCombinePath(pDirectory, fi.name()), 
								fim, gfm, depth);
							ret.insert(ret.end(), v.begin(), v.end());
						}
					}
				}
				else
				{
					if ((gfm & GETFILESEMODE::FILE) != 0)
						ret.push_back(stdCombinePath(pDirectory, fi.name()));
				}
			}
			return ret;
		}
		inline std::vector<std::basic_string<SYSTEM_CHAR_TYPE>> stdGetFiles(
			const std::basic_string<SYSTEM_CHAR_TYPE>& directory,
			FILEITERATEMODE fim,
			GETFILESEMODE gfm,
			int depth)
		{
			return stdGetFiles(directory.c_str(), fim, gfm, depth);
		}
		inline bool stdDirectoryEmpty(const SYSTEM_CHAR_TYPE* pStr)
		{
			if (pStr == nullptr || pStr[0] == 0)
				return false;
			if (stdFileExists(pStr))
				return false;
			if (!stdDirectoryExists(pStr))
				return true;

			std::vector<std::basic_string<SYSTEM_CHAR_TYPE>> ret =
				stdGetFiles(
					pStr,
					FILEITERATEMODE::SKIP_NONE,
					GETFILESEMODE::FILE_AND_DIRECTORY,
					1);
			return ret.size() == 2;
		}
		inline bool stdDirectoryEmpty(const std::basic_string<SYSTEM_CHAR_TYPE>& str)
		{
			return stdDirectoryEmpty(str.c_str());
		}

		inline size_t stdGetFileCount(const SYSTEM_CHAR_TYPE* pStr)
		{
			if (pStr == nullptr || pStr[0] == 0)
				return 0;
			if (stdFileExists(pStr))
				return 0;
			if (!stdDirectoryExists(pStr))
				return 0;

			std::vector<std::basic_string<SYSTEM_CHAR_TYPE>> ret =
				stdGetFiles(
					pStr,
					FILEITERATEMODE::SKIP_DOT | FILEITERATEMODE::SKIP_DOTDOT,
					GETFILESEMODE::FILE,
					-1);
			return ret.size();
		}
		inline size_t stdGetFileCount(const std::basic_string<SYSTEM_CHAR_TYPE>& str)
		{
			return stdGetFileCount(str.c_str());
		}
		// Todo this stdGetFiles must be templated,
// but linux implementation of FileFind is tired
//		inline bool stdDirectoryEmpty(const std::string& path)
//		{
//			return stdDirectoryEmpty(path.c_str());
//		}
//#ifdef _WIN32
//		inline bool stdDirectoryEmpty(const std::string& path)
//		{
//			return stdDirectoryEmpty(path.c_str());
//		}
//#endif

		template<typename C>
		inline bool stdIsSubDirectory(const C* pParent, const C* pSub)
		{
			if (!pParent || pParent[0] == 0)
				return false;
			if (!pSub || pSub[0] == 0)
				return false;

			std::basic_string<C> parent = stdGetFullPathName(pParent);
			std::basic_string<C> sub = stdGetFullPathName(pSub);

			for (std::basic_string<C> dir = stdGetParentDirectory(sub);
				!dir.empty();
				dir = stdGetParentDirectory(dir))
			{
				if (stdIsSamePath(dir, parent))
					return true;
			}
			return false;
		}
		template<typename C>
		inline bool stdIsSubDirectory(const std::basic_string<C>& parent, const std::basic_string<C>& sub)
		{
			return stdIsSubDirectory(parent.c_str(), sub.c_str());
		}

		inline int stdMkDir(const char* pDir)
		{
            return
#ifdef _WIN32
                    _mkdir(pDir);
#else
                    mkdir(pDir, 0777);
#endif
		}
#ifdef _WIN32
		inline int stdMkDir(const wchar_t* pDir)
		{
			return _wmkdir(pDir);
		}
#endif
		template<class C>
		int stdMkDir(const std::basic_string<C>& dir)
		{
			return stdMkDir(dir.c_str());
		}

		inline int stdRmDir(const char* pDir)
		{
            return
#ifdef _WIN32
                    _rmdir(pDir);
#else
                    rmdir(pDir);
#endif
		}
#ifdef _WIN32
		inline int stdRmDir(const wchar_t* pDir)
		{
			return _wrmdir(pDir);
		}
#endif
		template<class C>
		int stdRmDir(const std::basic_string<C>& dir)
		{
			return stdRmDir(dir.c_str());
		}

		inline int stdUnlink(const char* pFile)
		{
            return
#ifdef _WIN32
                    _unlink(pFile);
#else
                    unlink(pFile);
#endif
		}
#ifdef _WIN32
		inline int stdUnlink(const wchar_t* pFile)
		{
			return _wunlink(pFile);
		}
#endif
		template<class C>
		int stdUnlink(const std::basic_string<C>& file)
		{
			return stdUnlink(file.c_str());
		}

		inline FILE* stdOpenFile(const char* pFile, const char* pMode)
		{
#ifdef _WIN32
			FILE* f = nullptr;
			if (0 != fopen_s(&f, pFile, pMode))
				return nullptr;
			return f;
#else
            return fopen(pFile, pMode);
#endif
		}
#ifdef _WIN32
		inline FILE* stdOpenFile(const wchar_t* pFile, const wchar_t* pMode)
		{
			FILE* f = nullptr;
			if (0 != _wfopen_s(&f, pFile, pMode))
				return nullptr;
			return f;
		}
#endif
        inline bool stdCreateCompleteDirectory(const std::string& dir);
#ifdef _WIN32
		inline bool stdCreateCompleteDirectory(const std::wstring& dir);
#endif
		template<class C>
		bool stdCreateCompleteDirectory(const C* pDir)
		{
			if (!pDir || !pDir[0])
				return false;
			if (stdDirectoryExists(pDir))
				return true;
			stdCreateCompleteDirectory(stdGetParentDirectory(pDir));
			return stdMkDir(pDir) == 0;
		};
        inline bool stdCreateCompleteDirectory(const std::string& dir)
        {
            return stdCreateCompleteDirectory(dir.c_str());
        }
#ifdef _WIN32
        inline bool stdCreateCompleteDirectory(const std::wstring& dir)
        {
            return stdCreateCompleteDirectory(dir.c_str());
        }
#endif
        template<class C>
		inline bool stdRemoveCompleteDirectory(const C* pDir)
		{
			if (!pDir || !pDir[0])
				return false;
			if (!stdDirectoryExists(pDir))
				return true;
			std::vector<std::basic_string<C>> allDirs = stdGetFiles(
				pDir,
				FILEITERATEMODE::SKIP_DOT_AND_DOTDOT,
				GETFILESEMODE::DIRECTORY);

			int success = 0;
			auto fRemoveAll = [&]() {
				for (auto&& d : allDirs)
				{
					if (0 == stdRmDir(d))
						++success;
				}
			};
			for (;;)
			{
				success = 0;
				fRemoveAll();
				if (success == 0)
					break;
			}

			return !stdDirectoryExists(pDir) || 0 == stdRmDir(pDir);
		};
		template<class S>
		bool stdRemoveCompleteDirectory(const S& dir)
		{
			return stdRemoveCompleteDirectory(dir.c_str());
		}

		template<class C>
		bool stdWriteAllText(const C* pFile, const unsigned char* pContent, size_t size,
			bool bCreateDir = false)
		{
			if (!pFile || !pFile[0])
				return false;
			if (bCreateDir)
			{
				if (!stdCreateCompleteDirectory(stdGetParentDirectory(pFile)))
					return false;
			}
			std::unique_ptr<FILE, std::function<int(FILE*)>> file(
				stdOpenFile(pFile,
					stdLiterals<C>::
#ifdef _WIN32
					FileModeWriteByte()
#else
					FileModeWrite()
#endif
				), fclose);
			if (file.get() == nullptr)
				return false;
			size_t written = fwrite(pContent, sizeof(unsigned char), size, file.get());
			return written == size;
		}
		template<class C>
		bool stdWriteAllText(const std::basic_string<C>& file, const unsigned char* pContent, size_t size,
			bool bCreateDir = false)
		{
			return stdWriteAllText(file.c_str(), pContent, size, bCreateDir);
		}
		// TODO: implement read

		template<class C>
		std::basic_string<C> stdCharToString(C c)
		{
			return std::basic_string<C>(1, c);
		}

		template<class C>
		bool stdHasVideoFileExtension(const C* file)
		{
			using S = std::basic_string<C>;
			S ext = stdGetFileExtension(file);
			ext = stdTrimStart(ext, stdLiterals<C>::dotString());
			for (auto&& e : stdSplitString(
				stdLiterals<C>::VideoFileExtensions(),
				stdCharToString(stdLiterals<C>::NSemiColon)))
			{
				if (isCharEqual(e.c_str(), ext.c_str(), true))
					return true;
			}
			return false;
		}
		template<class C>
		bool stdHasVideoFileExtension(std::basic_string<C>& file)
		{
			return stdHasVideoFileExtension(file.c_str());
		}

		enum class SplitEnvSeparator
		{
			SEPARATOR_OS_DEFAULT,
			SEPARATOR_COLON,
			SEPARATOR_SEMICOLON,
		};
		template<class C>
		std::vector<std::basic_string<C>> stdSplitEnvPath(const C* pPaths, 
			const SplitEnvSeparator sep = SplitEnvSeparator::SEPARATOR_OS_DEFAULT)
		{
			std::vector<std::basic_string<C>> ret;

			const C sepchar = sep == SplitEnvSeparator::SEPARATOR_OS_DEFAULT ? stdLiterals<C>::NDefaultEnvPathSeparator :
				(sep == SplitEnvSeparator::SEPARATOR_COLON ? stdLiterals<C>::NColon : stdLiterals<C>::NSemiColon);
			const C* p = pPaths;
			std::basic_string<C> cur;
			bool inq = false;
			for (; *p; ++p)
			{
				if (stdIsLeadChar(p))
				{
					cur += *p;
					++p;
					cur += *p;
					continue;
				}

				if (inq)
				{
					if (*p == stdLiterals<C>::NDoubleQuote)
					{
						if (!cur.empty())
						{
							ret.push_back(cur);
							cur = std::basic_string<C>();
						}
						inq = false;
					}
					else
					{
						cur += *p;
					}
				}
				else
				{ // not inq
					if (*p == stdLiterals<C>::NDoubleQuote)
					{
						inq = true;
					}
					else if (*p == sepchar)
					{
						if (!cur.empty())
						{
							ret.push_back(cur);
							cur = std::basic_string<C>();
						}
					}
					else
					{
						cur += *p;
					}
				}
			}

			if (!cur.empty())
			{
				ret.push_back(cur);
				cur = std::basic_string<C>();
			}

			return ret;
		}
		template<class C>
		std::vector<std::basic_string<C>> stdSplitEnvPath(const std::basic_string<C>& paths, const C sep)
		{
			return stdSplitEnvPath(paths.c_str(), sep);
		}


		template<class C = SYSTEM_CHAR_TYPE, class T>
		std::basic_string<C> stdGetHexString(const T& value, std::streamsize keta = 1)
		{
			std::basic_stringstream<C> ss;
			ss << std::setfill(stdLiterals<C>::N0) << std::setw(keta) << std::right << std::hex << value;

			return ss.str();
		}

		template<class C = SYSTEM_CHAR_TYPE, class T>
		bool stdStartWith(const T* pString, const T* pEnd)
		{
			if (!pString)
				return false;

			if (!pEnd || pEnd[0] == 0)
				return true;

			size_t psize = stdStringLength(pString);
			size_t endlen = stdStringLength(pEnd);
			if (psize < endlen)
				return false;

			while (*pEnd)
			{
				if (*pString != *pEnd)
					return false;

				++pString;
				++pEnd;
			}
			return true;
		}
		template<class C>
		bool stdStartWith(const C* ps, const std::basic_string<C>& end)
		{
			return stdStartWith(ps, end.c_str());
		}
		template<class C>
		bool stdStartWith(const std::basic_string<C>& s, const C* pEnd)
		{
			return stdStartWith(s.c_str(), pEnd);
		}
		template<class C>
		bool stdStartWith(const std::basic_string<C>& s, const std::basic_string<C>& end)
		{
			return stdStartWith(s.c_str(), end.c_str());
		}



		template<class C = SYSTEM_CHAR_TYPE, class T>
		bool stdEndWith(const T* pString, const T* pEnd)
		{
			if (!pString)
				return false;

			if (!pEnd || pEnd[0] == 0)
				return true;

			size_t psize = stdStringLength(pString);
			size_t endlen = stdStringLength(pEnd);
			if (psize < endlen)
				return false;

			const T* pS = pString;
			const T* pSS = pS + psize - endlen;
			while (*pEnd)
			{
				if (*pSS != *pEnd)
					return false;

				++pSS;
				++pEnd;
			}
			return true;
		}
		template<class C>
		bool stdEndWith(const C* ps, const std::basic_string<C>& end)
		{
			return stdEndWith(ps, end.c_str());
		}
		template<class C>
		bool stdEndWith(const std::basic_string<C>& s, const C* pEnd)
		{
			return stdEndWith(s.c_str(), pEnd);
		}
		template<class C>
		bool stdEndWith(const std::basic_string<C>& s, const std::basic_string<C>& end)
		{
			return stdEndWith(s.c_str(), end.c_str());
		}


		inline long stdGetFileSize(const char* pFile)
		{
			struct stat stat_buf;
			int rc = stat(pFile, &stat_buf);
			return rc == 0 ? stat_buf.st_size : -1;
		}
#ifdef _WIN32
		inline long stdGetFileSize(const wchar_t* pFile)
		{
			struct _stat  stat_buf;
			int rc = _wstat(pFile, &stat_buf);
			return rc == 0 ? stat_buf.st_size : -1;
		}
#endif

		// Compare memory and file content
		template<typename C = SYSTEM_CHAR_TYPE>
		inline bool stdIsFileSameWithMemory(const C* pFileName, const void* pData, const size_t size)
		{
			if (stdGetFileSize(pFileName) != size)
				return false;

			std::unique_ptr<FILE, std::function<int(FILE*)>> f(stdOpenFile(pFileName, stdLiterals<C>::FileModeReadByte()),
				fclose);
			if (!f)
				return false;

			unsigned char buff[256];
			const unsigned char* pByte = reinterpret_cast<const unsigned char*>(pData);
			size_t pos = 0;
			do {
                size_t red = fread(buff, sizeof(buff[0]), stdCountOf(buff), f.get());
				if (red == 0)
					return false;
				if (0 != memcmp(buff, pByte + pos, red))
					return false;
				pos += red;
			} while (pos < size);

			return true;
		}
		template<typename C = SYSTEM_CHAR_TYPE>
		inline bool stdIsFileSameWithMemory(const std::basic_string<C> file, const void* pData, const size_t size)
		{
			return stdIsFileSameWithMemory(file.c_str(), pData, size);
		}


        
		std::basic_string<SYSTEM_CHAR_TYPE> stdGetLocalAppDirectory(
			const std::basic_string<SYSTEM_CHAR_TYPE>& organization,
			const std::basic_string<SYSTEM_CHAR_TYPE>& appName,
			bool bCreate = false);

		std::basic_string<SYSTEM_CHAR_TYPE> stdGetHomeDirectory();
	}
}


#undef stdosd_max
#undef stdosd_min

#endif //#ifndef STDOSD_INCLUDED
