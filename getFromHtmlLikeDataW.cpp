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
#include <windows.h>
#include <map>
#include <comdef.h>
#include "DebugNew.h"
#include "getFromHtmlLikeDataW.h"
using namespace std;
void getFromHtmlLikeDataW(const WCHAR* pDataO, map<_bstr_t, _bstr_t>& headerMap,
						  WCHAR** ppHtml)
{
	if (ppHtml) 
		*ppHtml=0;
	if ( pDataO==NULL || pDataO[0]==0 )
		return;
	
//	size_t checkcount = 0;

	WCHAR* pDataOrig = _wcsdup(pDataO);
	WCHAR* pData = pDataOrig;
	WCHAR* pOUT = pDataOrig;

	if ( ( *((BYTE*)pData)==0xff && *((BYTE*)pData+1)==0xfe) )
	{ // little
		pData++;
		pOUT++;
	}
	else 	if ( ( *((BYTE*)pData)==0xfe && *((BYTE*)pData+1)==0xff) )
	{
		pData++;
		pOUT++;
		BYTE* pByte = (BYTE*)pData;
		while( !(*pByte==0 && *(pByte+1)==0) )
		{
			BYTE tmp = *pByte;
			*pByte = *(pByte+1);
			*(pByte+1) = tmp;
			pByte+=2;
		}
	}

	WCHAR* p = wcstok(pData, L"\r\n");
	while( p )
	{
		WCHAR* pT = wcsstr(p, L": ");
		pOUT = p;
		if ( pT==NULL )
		{
			
			break;
		}

		*pT=0;


		_bstr_t key = p;

		pT++;
		pT++;
		WCHAR* pTT = pT;
		for( ; *pTT ; ++pTT)
		{
			if(*pTT==L'\r' || *pTT==L'\n')
			{
				*pTT=0;
				break;
			}
		}
		_bstr_t val = pT;
		headerMap.insert(make_pair(key,val));

		p = wcstok(NULL, L"\r\n");
	}
	pOUT = p;
	if ( pOUT )
		pOUT = pOUT-pDataOrig+(WCHAR*)pDataO;
	free(pDataOrig);
	pData =NULL;

	if ( ppHtml )
	{
		*ppHtml = pOUT;
		/**
		do {

			list<LPWSTR> intlist;
			LPWSTR p1,p2,p3,p4;
			p1=wcsstr(pDataO + checkcount, L"\r\n\r");
			p2=wcsstr(pDataO + checkcount, L"\r\n\n");
			p3=wcsstr(pDataO + checkcount, L"\r\r");
			p4=wcsstr(pDataO + checkcount, L"\n\n");
			intlist.push_back(p1);
			intlist.push_back(p2);
			intlist.push_back(p3);
			intlist.push_back(p4);
			intlist.sort();
			
			list<LPWSTR>::iterator it = intlist.begin();
			for(; it != intlist.end() ; ++it)
			{
				if (*it==NULL)
					continue;
				else
				{
					*pN = *it-pDataO;
					break;
				}
			}
			if (*pN==0)
				*pN = checkcount;
		} while(0);
		**/

//		*pN = checkcount;
	}
	return;
}