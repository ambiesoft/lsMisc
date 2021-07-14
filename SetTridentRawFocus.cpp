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


//#include <comdef.h>
//#include <ShlGuid.h>
#include <exdisp.h>
// #import <mshtml.tlb> no_namespace
// #import <mshtml.tlb> rename_namespace("TridentMshtml") // rename("event", "mshtml_event") rename("value", "mshtml_value")

#pragma warning(push)
// warning C4192: automatically excluding '<xyz>' while importing type library 'mshtml.tlb'
#pragma warning(disable: 4192) 
#import <mshtml.tlb> rename("min", "mshtml_min") rename("max", "mshtml_max") rename("TranslateAccelerator", "mshtml_TranslateAccelerator")
#pragma warning(pop)

#include "SetTridentRawFocus.h"

#include "DebugNew.h"


namespace Ambiesoft {
	bool SetTridentRawFocus(IWebBrowser2Ptr pBrowser)
	{
		if (!pBrowser)
			return false;

		IDispatchPtr disp;
		pBrowser->get_Document(&disp);
		MSHTML::IHTMLDocument2Ptr doc = disp;
		if (!doc)
			return false;

		MSHTML::IHTMLWindow2Ptr win;
		doc->get_parentWindow(&win);
		if (!win)
			return false;

		MSHTML::IHTMLElementPtr active = doc->activeElement;
		if (active)
		{
			_bstr_t tagname = active->tagName;
			if (0 == lstrcmpiW(tagname, L"input"))
			{
				return false;
			}
			if (0 == lstrcmpiW(tagname, L"textarea"))
			{
				return false;
			}
		}
		return SUCCEEDED(win->focus());
	}
}