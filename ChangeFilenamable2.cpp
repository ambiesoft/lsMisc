#include <windows.h>
#include <tchar.h>
#include <string>



#include "tstring.h"
#include "ChangeFilenamable2.h"

using namespace std;

tstring ChangeFilenamable2(LPCTSTR pch)
{
	tstring ret;
	while( *pch != '\0' )
	{
		switch(*pch)
		{
		case _T('\\'):
			ret += _T("��");
			break;

		case _T(':'):
			ret += _T("�F");
			break;
			
		case _T('*'):
			ret += _T("��");
			break;
			
		case _T('?'):
			ret += _T("�H");
			break;
			
		case _T('\"'):
			ret += _T("�g");
			break;
			
		case _T('<'):
			ret += _T("��");
			break;

		case _T('>'):
			ret += _T("��");
			break;

		case _T('|'):
			ret += _T("�b");
			break;

		case _T('/'):
			ret += _T("�^");
			break;

		default:
			{
#ifdef UNICODE
				TCHAR szT[2];
				szT[0] = *pch;
				szT[1] = 0;
				ret += szT;
#else
				if(IsDBCSLeadByte((BYTE)(*pch)))
				{
					TCHAR szT[3];
					szT[0] = *pch;
					szT[1] = *(pch+1);
					szT[2] = 0;
					ret += szT;
				}
				else
				{
					TCHAR szT[2];
					szT[0] = *pch;
					szT[1] = 0;
					ret += szT;
				}
#endif
			}
			break;
		}
		pch = (TCHAR*)_tcsinc(pch);
	}
	
	return ret;
}








/*
template<class E, class T, class A>

static std::basic_string<E,T,A>
replace_all(
  const std::basic_string<E,T,A>& source,   // source���ɂ���
  const std::basic_string<E,T,A>& pattern,  // pattern��
  const std::basic_string<E,T,A>& placement // placement�ɒu��������
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
		std::string(_T("��"))
		);
	ret=replace_all(
		ret,
		std::string(_T(":")),
		std::string(_T("�F"))
		);
	ret=replace_all(
		ret,
		std::string(_T("*")),
		std::string(_T("��"))
		);
	ret=replace_all(
		ret,
		std::string(_T("?")),
		std::string(_T("�H"))
		);
	ret=replace_all(
		ret,
		std::string(_T("\"")),
		std::string(_T("�h"))
		);
	ret=replace_all(
		ret,
		std::string(_T("<")),
		std::string(_T("��"))
		);
	ret=replace_all(
		ret,
		std::string(_T(">")),
		std::string(_T("��"))
		);
	ret=replace_all(
		ret,
		std::string(_T("|")),
		std::string(_T("�b"))
		);
	ret=replace_all(
		ret,
		std::string(_T("/")),
		std::string(_T("�^"))
		);




	return ret;
}
*/