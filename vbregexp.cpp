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

#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE


#include <windows.h>
#include <tchar.h>
#include "vbregexp.h"

#import "vbregexp.tlb" no_namespace named_guids

static IRegExpPtr pReg;
BOOL vbregMatch(LPCTSTR subject, LPCTSTR regex)
{
	if(pReg==NULL)
	{
		CoInitialize(NULL); 
		HRESULT hr = pReg.CreateInstance(CLSID_RegExp);
		if(FAILED(hr))
		{
			return FALSE;
		} 
	}

	_bstr_t str(subject);
	_bstr_t pattern(regex);

	pReg->put_Global(VARIANT_TRUE); 
	pReg->put_Pattern(pattern); 
	
	IDispatchPtr pDisp=pReg->Execute(str); 
	if(pDisp)
	{ 
		IMatchCollectionPtr pCol;
		if(SUCCEEDED(pDisp->QueryInterface(IID_IMatchCollection,(void**)&pCol)))
		{ 
			LONG count=0; 
			if(SUCCEEDED(pCol->get_Count(&count)) && count > 0)
				return TRUE;
		}
	}

	return FALSE;
}


LPCTSTR vbregReplace(LPCTSTR source,
					 LPCTSTR replace, 
					 LPCTSTR regex,
					 BOOL bGlobal,
					 BOOL bCaseInsensitive)
{
	if(pReg==NULL)
	{
		CoInitialize(NULL); 
		HRESULT hr = pReg.CreateInstance(CLSID_RegExp);
		if(FAILED(hr))
		{
			return FALSE;
		} 
	}

	_bstr_t bstrSource(source);
	_bstr_t bstrReplace(replace);
	_bstr_t bstrRegex(regex);

	pReg->put_Global(bGlobal?VARIANT_TRUE:VARIANT_FALSE); 
	pReg->put_IgnoreCase(bCaseInsensitive?VARIANT_TRUE:VARIANT_FALSE); 
	pReg->put_Pattern(bstrRegex); 

	_bstr_t bstrRet = pReg->Replace(bstrSource, bstrReplace);

	return _tcsdup(bstrRet);
}


//			IDispatchPtr pDisp2;
//			for(long i=0; SUCCEEDED(pCol->get_Item(i, &pDisp2)) ; ++i)
//			{
//				IMatchPtr pMatch;
//				if(SUCCEEDED(pDisp2->QueryInterface(IID_IMatch,(void **)&pMatch)))
//				{ 
//					BSTR bstr; 
//					pMatch->get_Value(&bstr); 
//					_bstr_t value(bstr,false);

//					char *buf=_com_util::ConvertBSTRToString(value.GetBSTR()); 
//					_tprintf("all = %s\n",buf); 
//					delete[] buf; 
//				} 
//			}
