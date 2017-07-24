#pragma warning(disable:4786)
#include "stdafx.h"
#pragma warning(disable:4786)



#include <windows.h>

#include <algorithm> 
#include <cassert>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <functional> 
#include <locale>
#include <string>
#include <vector>
#include <memory.h>

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")



#ifdef __cplusplus_cli
#include <vcclr.h>
#endif // __cplusplus_cli


#include "stdwin32.h"


namespace stdwin32 {

	using namespace std;

	static const wchar_t* Nil = L"";

	std::string stdGetModuleFileNameA(HINSTANCE hInst)
	{
		LPSTR p = NULL;
		DWORD size = 64;
		for (;;)
		{
			p = (LPSTR)realloc(p, size * sizeof(char));
			if (GetModuleFileNameA(hInst, p, size) < size)
				break;
			size *= 2;
		}

		std::string ret = p;
		free((void*)p);
		return ret;
	}
	std::wstring stdGetModuleFileNameW(HINSTANCE hInst)
	{
		LPWSTR p = NULL;
		DWORD size = 64;
		for (;;)
		{
			p = (LPWSTR)realloc(p, size * sizeof(wchar_t));
			if (GetModuleFileNameW(hInst, p, size) < size)
				break;
			size *= 2;
		}

		std::wstring ret = p;
		free((void*)p);
		return ret;
	}

	bool stdIsFullPath(LPCWSTR pD, bool allownetwork)
	{
		if (!pD || pD[0] == 0)
			return false;

		if (pD[0] == L'/')
			return true;

		if (!((L'a' <= pD[0] && pD[0] < +L'z') ||
			(L'A' <= pD[0] && pD[0] < +L'Z')))
		{
			if(!allownetwork)
				return FALSE;

			if (pD[1] == 0)
				return false;

			if (!(pD[0] == L'\\' && pD[1] == L'\\'))
				return false;

			return pD[2] != 0;
		}

		if (pD[1] == L':')
			return true;

		return false;
	}

	std::wstring stdCombinePath(const std::wstring& d1, const std::wstring& d2)
	{
		return stdCombinePath(d1.c_str(), d2.c_str());
	}

	std::wstring stdCombinePath(LPCWSTR pD1, LPCWSTR pD2)
	{

		if (!pD1 || !pD1[0])
			return pD2;

		if (!pD2 || !pD2[0])
			return pD1;

		if (stdIsFullPath(pD2))
			return pD2;

		std::wstring ret = pD1;
		std::wstring::iterator it = ret.end();
		--it;
		if (*it != L'\\')
			ret += L'\\';

		ret += pD2;
		return ret;
	}


	std::wstring stdGetParentDirectory(const std::wstring& path, bool bAddBackslach)
	{
		return stdGetParentDirectory(path.c_str(), bAddBackslach);
	}
	std::wstring stdGetParentDirectory(LPCWSTR pPath, bool bAddBackslach)
	{
		if (!pPath || pPath[0] == 0)
			return Nil;

		LPWSTR p = _wcsdup(pPath);

		size_t len = wcslen(p);
		if (p[len - 1] == L'\\')
			p[len - 1] = 0;

		LPWSTR pT = wcsrchr(p, L'\\');
		if (!pT)
		{
			free(p);
			return Nil;
		}

		if (!bAddBackslach)
			*pT = 0;
		else
			*(pT + 1) = 0;

		std::wstring ret(p);
		free(p);
		return ret;
	}

	std::wstring stdGetFileName(const std::wstring& full)
	{
		return stdGetFileName(full.c_str());
	}
	std::wstring stdGetFileName(LPCWSTR pFull)
	{
		WCHAR* p = _wcsdup(pFull);
		std::wstring ret = PathFindFileNameW(p);
		free((void*)p);
		return ret;
	}

	std::vector<std::wstring> stdSplitSCedPath(LPCWSTR pPath)
	{
		std::vector<std::wstring> ret;

		LPCWSTR p = pPath;
		std::wstring cur;
		bool inq = false;
		for (; *p; ++p)
		{
			if (inq)
			{
				if (*p == '\"')
				{
					if (!cur.empty())
					{
						ret.push_back(cur);
						cur = L"";
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
				if (*p == '\"')
				{
					inq = true;
				}
				else if (*p == L';')
				{
					if (!cur.empty())
					{
						ret.push_back(cur);
						cur = L"";
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
			cur = L"";
		}

		return ret;
	}

#ifndef __cplusplus_cli
	std::string string_formatA(const std::string fmt, ...)
	{
		int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
		std::string str;
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			str.resize(size);
			va_start(ap, fmt);

#if _MSC_VER <= 1400
			int n = _vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
#else
			int n = _vsnprintf_s((char *)str.data(), size, size - 1, fmt.c_str(), ap);
#endif	
			va_end(ap);
			if (n > -1 && n < size) {  // Everything worked
				str.resize(n);
				return str;
			}
			if (n > -1)  // Needed size returned
				size = n + 1;   // For null char
			else
				size *= 2;      // Guess at a larger size (OS specific)
		}
		return str;
	}
	std::wstring string_formatW(const std::wstring fmt, ...)
	{
		int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
		std::wstring str;
		va_list ap;
		while (1) {     // Maximum two passes on a POSIX system...
			str.resize(size);
			va_start(ap, fmt);

#if _MSC_VER <= 1400
			int n = _vsnwprintf((WCHAR *)str.data(), size, fmt.c_str(), ap);
#else
			int n = _vsnwprintf_s((WCHAR *)str.data(), size, size - 1, fmt.c_str(), ap);
#endif
			va_end(ap);
			if (n > -1 && n < size) {  // Everything worked
				str.resize(n);
				return str;
			}
			if (n > -1)  // Needed size returned
				size = n + 1;   // For null char
			else
				size *= 2;      // Guess at a larger size (OS specific)
		}
		return str;
	}
#endif __cplusplus_cli



	std::string trimA(const std::string& str,
		const std::string& whitespace)
	{
		const std::string::size_type strBegin = str.find_first_not_of(whitespace);
		if (strBegin == std::string::npos)
			return ""; // no content

		const std::string::size_type strEnd = str.find_last_not_of(whitespace);
		const std::string::size_type strRange = strEnd - strBegin + 1;

		return str.substr(strBegin, strRange);
	}
	std::wstring trimW(const std::wstring& str,
		const std::wstring& whitespace)
	{
		const std::wstring::size_type strBegin = str.find_first_not_of(whitespace);
		if (strBegin == std::string::npos)
			return L""; // no content

		const std::wstring::size_type strEnd = str.find_last_not_of(whitespace);
		const std::wstring::size_type strRange = strEnd - strBegin + 1;

		return str.substr(strBegin, strRange);
	}





	bool isTdigit(const tstring& str)
	{
		if (str.size() == 0)
			return false;

		for (tstring::const_iterator it = str.begin(); it != str.end(); ++it)
		{
			if (!isdigit(*it))
				return false;
		}
		return true;
	}


	std::wstring stdGetCurrentDirectory()
	{
		DWORD len = GetCurrentDirectory(0, NULL);
		WCHAR* p = new WCHAR[len];
		p[0] = 0;
		GetCurrentDirectoryW(len, p);
		std::wstring ret(p);
		delete[] p;
		return ret;
	}


	bool hasEndingA(std::string const &fullString, std::string const &ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		}
		else {
			return false;
		}
	}
	bool hasEndingW(std::wstring const &fullString, std::wstring const &ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		}
		else {
			return false;
		}
	}


	bool hasEndingIA(std::string const &fullString, std::string const &ending) {
		std::string fullI(fullString);
		std::transform(fullI.begin(), fullI.end(), fullI.begin(), ::tolower);

		std::string endI(ending);
		std::transform(endI.begin(), endI.end(), endI.begin(), ::tolower);

		return hasEndingA(fullI, endI);
	}
	bool hasEndingIW(std::wstring const &fullString, std::wstring const &ending) {
		std::wstring fullI(fullString);
		std::transform(fullI.begin(), fullI.end(), fullI.begin(), ::towlower);

		std::wstring endI(ending);
		std::transform(endI.begin(), endI.end(), endI.begin(), ::towlower);

		return hasEndingW(fullI, endI);
	}


	std::string stdItoA(int i) {
		// return string_formatA("%d", i);
		char szT[64]; szT[0]=0;
#if _MSC_VER > 1310
		if(0 != _itoa_s(i, szT, 10))
			return "";
		return szT;
#else
		return _itoa(i, szT, 10);
#endif
	}
	std::wstring stdItoW(int i) {
		// return string_formatW(L"%d", i);
		wchar_t szT[64]; szT[0]=0;
#if _MSC_VER > 1310
		if(0 != _itow_s(i, szT, 10))
			return L"";
		return szT;
#else
		return _itow(i, szT, 10);
#endif
	}


	std::string stdItoA64(__int64 i) {
		// return string_formatA("%I64d", i);
		char szT[64]; szT[0]=0;
#if _MSC_VER > 1310
		if(0 != _i64toa_s(i, szT, _countof(szT), 10))
			return "";
		return szT;
#else
		return _i64toa(i, szT, 10);
#endif
	}
	std::wstring stdItoW64(__int64 i) {
		// return string_formatW(L"%I64d", i);
		wchar_t szT[64]; szT[0]=0;
#if _MSC_VER > 1310
		if(0 != _i64tow_s(i, szT, _countof(szT), 10))
			return L"";
		return szT;
#else
		return _i64tow(i, szT, 10);
#endif
	}


	std::string stdExpandEnvironmentStringsA(LPCSTR pStr)
	{
		DWORD dwReq = ExpandEnvironmentStringsA(pStr, NULL, 0);
		LPSTR pBuff = new char[dwReq];

		DWORD dwRet = ExpandEnvironmentStringsA(pStr, pBuff, dwReq);
		if (dwReq != dwRet)
		{
			delete[] pBuff;
			return pStr;
		}
		std::string ret(pBuff);
		delete[] pBuff;
		return ret;
	}
	std::wstring stdExpandEnvironmentStringsW(LPCWSTR pStr)
	{
		DWORD dwReq = ExpandEnvironmentStringsW(pStr, NULL, 0);
		LPWSTR pBuff = new wchar_t[dwReq];

		DWORD dwRet = ExpandEnvironmentStringsW(pStr, pBuff, dwReq);
		if (dwReq != dwRet)
		{
			delete[] pBuff;
			return pStr;
		}
		std::wstring ret(pBuff);
		delete[] pBuff;
		return ret;
	}


	std::wstring stdGetEnvironmentVariableW(LPCWSTR pStr)
	{
		DWORD dwReq = GetEnvironmentVariableW(pStr, NULL, 0);
		if (dwReq == 0)
			return L"";
		LPWSTR pBuff = new wchar_t[dwReq];
		DWORD dwRet = GetEnvironmentVariableW(pStr, pBuff, dwReq);
		if (dwReq < dwRet)
		{
			delete[] pBuff;
			return pStr;
		}
		std::wstring ret(pBuff);
		delete[] pBuff;
		return ret;
	}

	bool stdGetUnittedSizeW(LPCWSTR pStr, int& nSign, __int64& lResult, int* pUnit)
	{
		if (pStr == NULL || *pStr == 0)
			return false;

		wstring str(pStr);
		WCHAR ch = *str.rbegin();
		int unit = 1;
		bool unitted = true;
		switch (ch)
		{
		case L'k': unit = 1000; break;
		case L'K': unit = 1024; break;
		case L'm': unit = 1000 * 1000; break;
		case L'M': unit = 1024 * 1024; break;
		case L'g': unit = 1000 * 1000 * 1000; break;
		case L'G': unit = 1024 * 1024 * 1024; break;
		default:
			unitted = false;
			break;
		}
		if (unitted)
			str = str.substr(0, str.size() - 1);


		nSign = 0;
		bool bsigned = true;
		switch (str[0])
		{
		case L'+': nSign = 1; break;
		case L'-': nSign = -1; break;
		default:
			bsigned = false;
		}
		if (bsigned)
			str = str.substr(1, str.size());

		if (pUnit)
			*pUnit = unit;

		__int64 r = _wtoi64(str.c_str());

		lResult = r * unit;
		if (nSign < 0)
			lResult = -lResult;

		return true;
	}


	wstring stdSplitCommandLineW(int argc, int startargc, LPCWSTR* argv)
	{
		wstring ret;
		if (argc < 1)
			return ret;

		for (int i = startargc; i < argc; ++i)
		{
			LPCWSTR p = argv[i];
			if (!p || !p[0])
				return L"";

			if (p[0] == L'"')
			{
				ret += p;
			}
			else if (wcschr(p, L' ') || wcschr(p, L'\t'))
			{
				ret += L"\"";
				ret += p;
				ret += L"\"";
			}
			else
			{
				ret += p;
			}
			ret += L" ";
		}

		ret = trimW(ret);
		return ret;
	}

	wstring stdAddBackSlash(const wstring& d)
	{
		if (d.empty())
			return L"\\";

		if (d[d.length() - 1] == L'\\')
			return d;

		return d + L'\\';
	}





#ifdef __cplusplus_cli
	std::wstring getStdWString(System::String^ s)
	{
		pin_ptr<const wchar_t> p = PtrToStringChars(s);
		return p;
	}
	std::string getStdString(System::String^ s)
	{
		std::string ret;
		if (s == nullptr)
			return ret;

		pin_ptr<const wchar_t> p = PtrToStringChars(s);
		size_t len = (s->Length + 1) * sizeof(wchar_t);
		char* pT = (char*)malloc(len);
		size_t retutrnvalue;

		if (0 != wcstombs_s(&retutrnvalue,
			pT,
			len,
			p,
			len))
		{
			free(pT);
			return ret;
		}
		ret = pT;
		free(pT);
		return ret;
	}
#endif // __cplusplus_cli



	// http://www.emoticode.net/c/urlencode-in-plain-c.html
	static char i2a(char code) 
	{
		static char hex[] = "0123456789ABCDEF";
		return hex[code & 15];
	}

	std::wstring stdEncodeUrl(const std::wstring& d)
	{
		return stdEncodeUrl(d.c_str());
	}
	std::wstring stdEncodeUrl(LPCWSTR pstr)
	{
		wstring ret;
		for( ; *pstr ;++pstr)
		{
			if ((*pstr >0 && iswalnum(*pstr)) || *pstr == L'-' || *pstr == L'_' || *pstr == L'.' || *pstr == L'~') 
			{
				ret += *pstr;
			}
			else if (*pstr == L' ') 
			{
				ret += L'+';
			}
			else 
			{
				ret+= '%';
				ret+= i2a(*pstr >> 4);
				ret+= i2a(*pstr & 15);
			}
		}
		return ret;
	}

} // namespace stdwin32