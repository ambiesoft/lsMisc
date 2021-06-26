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

#include <windows.h>
#include <tchar.h>
#include <string>
#include <algorithm>


#include "tstring.h"
#include "MakeFilenamable.h"



tstring MakeFilenamable(LPCTSTR pch, bool bToSjisZen, LPCTSTR pDefault)
{
	if (!pch || !pch[0])
		return pDefault;
	tstring ret;
	while (*pch != '\0')
	{
		switch (*pch)
		{
		case _T('\\'):
			ret += bToSjisZen ? _T("￥") : _T("_");
			break;

		case _T(':'):
			ret += bToSjisZen ? _T("：") : _T("_");
			break;

		case _T('*'):
			ret += bToSjisZen ? _T("＊") : _T("_");
			break;

		case _T('?'):
			ret += bToSjisZen ? _T("？") : _T("_");
			break;

		case _T('\"'):
			ret += bToSjisZen ? _T("“") : _T("_");
			break;

		case _T('<'):
			ret += bToSjisZen ? _T("＜") : _T("_");
			break;

		case _T('>'):
			ret += bToSjisZen ? _T("＞") : _T("_");
			break;

		case _T('|'):
			ret += bToSjisZen ? _T("｜") : _T("_");
			break;

		case _T('/'):
			ret += bToSjisZen ? _T("／") : _T("_");
			break;

		default:
		{

#ifdef UNICODE
			TCHAR szT[2];
			szT[0] = *pch;
			szT[1] = 0;
			ret += szT;
#else
			if (IsDBCSLeadByte((BYTE)(*pch)))
			{
				TCHAR szT[3];
				szT[0] = *pch;
				szT[1] = *(pch + 1);
				szT[2] = 0;
				ret += szT;
			}
			else
			{
				if (0 <= *pch && *pch <= 0x1F)
				{
					// nothing
				}
				else
				{
					TCHAR szT[2];
					szT[0] = *pch;
					szT[1] = 0;
					ret += szT;
				}
			}
#endif
		}
		break;
		}
		pch = (TCHAR*)_tcsinc(pch);
	}

	if (ret.empty())
		return pDefault;

	// check illegal file names
	static const TCHAR* illegals[] = {
		_T("CON"),_T("PRN"), _T("AUX") , _T("NUL") , _T("COM0") , _T("COM1") ,_T("COM2")
		, _T("COM3") , _T("COM4") , _T("COM5") , _T("COM6") , _T("COM7") , _T("COM8") , _T("COM9") , _T("LPT0")
		, _T("LPT1") , _T("LPT2") , _T("LPT3") , _T("LPT4") ,_T("LPT5") , _T("LPT6") , _T("LPT7") , _T("LPT8")
		, _T("LPT9")
	};

	if (ret.size() <= 4)
	{
		tstring retupper(ret);
		std::transform(retupper.cbegin(), retupper.cend(), retupper.begin(), towupper);

		for (int i = 0; i < _countof(illegals); ++i)
		{
			if (retupper == illegals[i])
				return ret + _T("_");
		}
	}

	// check special case
	while (!ret.empty() && (
		ret[ret.size() - 1] == _T('.') ||
		ret[ret.size() - 1] == _T(' ') ||
		ret[ret.size() - 1] == _T('/')))
	{
		ret = MakeFilenamable(ret.substr(0, ret.size() - 1).c_str(),
			bToSjisZen, 
			pDefault);
	}
	if (ret.empty())
		return pDefault;

	if (ret.size() > MAX_PATH)
		return pDefault;

	return ret;
}








/*
template<class E, class T, class A>

static std::basic_string<E,T,A>
replace_all(
  const std::basic_string<E,T,A>& source,   // source中にある
  const std::basic_string<E,T,A>& pattern,  // patternを
  const std::basic_string<E,T,A>& placement // placementに置き換える
  ) {
  std::basic_string<E,T,A> result(source);
  for ( std::string::size_type pos = 0 ;
		std::string::npos != (pos = result.find(pattern,pos));
		pos += placement.size() )
	result.replace(pos, pattern.size(), placement);
  return result;
}

std::string ChangeFilenamable2(LPCTSTR pch)
{
	std::string ret=pch;
	ret=replace_all(
		ret,
		std::string(_T("\\")),
		std::string(_T("￥"))
		);
	ret=replace_all(
		ret,
		std::string(_T(":")),
		std::string(_T("："))
		);
	ret=replace_all(
		ret,
		std::string(_T("*")),
		std::string(_T("＊"))
		);
	ret=replace_all(
		ret,
		std::string(_T("?")),
		std::string(_T("？"))
		);
	ret=replace_all(
		ret,
		std::string(_T("\"")),
		std::string(_T("”"))
		);
	ret=replace_all(
		ret,
		std::string(_T("<")),
		std::string(_T("＜"))
		);
	ret=replace_all(
		ret,
		std::string(_T(">")),
		std::string(_T("＞"))
		);
	ret=replace_all(
		ret,
		std::string(_T("|")),
		std::string(_T("｜"))
		);
	ret=replace_all(
		ret,
		std::string(_T("/")),
		std::string(_T("／"))
		);




	return ret;
}
*/