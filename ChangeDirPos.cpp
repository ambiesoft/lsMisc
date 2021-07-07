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
#include <Commctrl.h>
#include <vector>
using namespace std;
#include "ChangeDirPos.h"
#include "GetSelItems.h"

#include "verify.h"

namespace Ambiesoft {

static BOOL ChangeDirPosStuff( HWND hWndList, int nSelItem, ChangePosType nType )
{
//	int nSelItem = FindSelectedItem(hWndList);
	if( nSelItem == -1 )
		return FALSE;

	if( nType==POS_TO_UP && nSelItem==0 )
		return FALSE;

	if( nType==POS_TO_DOWN && ListView_GetItemCount(hWndList) <= (1+nSelItem) )
		return FALSE;

	int nNewSelItem;
	if( nType==0 )
		nNewSelItem = nSelItem-1;	
	else
		nNewSelItem = nSelItem+1;

	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVITEM));
	lvi.iItem = nSelItem;
	lvi.mask = LVIF_PARAM | LVIF_STATE;
	lvi.stateMask = LVIS_SELECTED;
	VERIFY(SendMessage(hWndList, LVM_GETITEM, 0, (LPARAM)&lvi));

	LVITEM lvi2;
	memset(&lvi2, 0, sizeof(LVITEM));
	lvi2.iItem = nNewSelItem;
	lvi2.mask = LVIF_PARAM | LVIF_STATE;
	lvi.stateMask = LVIS_SELECTED;
	VERIFY(SendMessage(hWndList, LVM_GETITEM, 0, (LPARAM)&lvi2));
	
	// swap it
	lvi .iItem = nNewSelItem;
	lvi.stateMask = LVIS_SELECTED;
	lvi2.iItem = nSelItem;
	lvi2.stateMask = LVIS_SELECTED;
	VERIFY(SendMessage(hWndList, LVM_SETITEM, 0, (LPARAM)&lvi ));
	VERIFY(SendMessage(hWndList, LVM_SETITEM, 0, (LPARAM)&lvi2));

	return TRUE;
}

void ChangeDirPos( HWND hWndList, ChangePosType nType )
{
	std::vector<int> nSelItems;
	GetSelItems(hWndList, nSelItems);

	SendMessage(hWndList, WM_SETREDRAW, FALSE, 0);
	std::vector<int>::iterator it;
	std::vector<int>::reverse_iterator rit;

	if( nType == POS_TO_UP )
	{
		for( it = nSelItems.begin() ;
			 it != nSelItems.end() ;
			 ++it )
		{
			if(!ChangeDirPosStuff(hWndList, *it, nType))
				break;
		}
	}
	else
	{
		for( rit = nSelItems.rbegin() ;
			 rit != nSelItems.rend() ;
			 ++rit )
		{
			if(!ChangeDirPosStuff(hWndList, *rit, nType))
				break;
		}
	}
	SendMessage(hWndList, WM_SETREDRAW, TRUE, 0);
	UpdateWindow(hWndList);
}





}