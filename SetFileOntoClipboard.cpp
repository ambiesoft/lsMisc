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
#include <afx.h>
#include <Windows.h>
#include <ShlObj.h>

#include "SetFileOntoClipboard.h"

namespace Ambiesoft {
	BOOL SetFileOntoClipboard(const CStringArray& arFiles, BOOL bCut)
	{
		int i;
		// COleDataSource ds;

		if (!OpenClipboard(NULL))
			return FALSE;
		if (!EmptyClipboard())
			return FALSE;


		{
			UINT uBuffSize = 0;
			HGLOBAL hgDrop = NULL;


			for (i = 0; i < arFiles.GetSize(); ++i)
			{
				uBuffSize += arFiles[i].GetLength() + 1;
			}

			uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

			hgDrop = GlobalAlloc(GHND | GMEM_SHARE, uBuffSize);
			if (NULL == hgDrop)
				return FALSE;

			DROPFILES* pDrop = (DROPFILES*)GlobalLock(hgDrop);
			if (NULL == pDrop)
			{
				GlobalFree(hgDrop);
				return TRUE;
			}

			pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
			pDrop->fWide = TRUE;
#endif

			LPTSTR pszBuff = (TCHAR*)(LPBYTE(pDrop) + sizeof(DROPFILES));
			for (i = 0; i < arFiles.GetSize(); ++i)
			{
				lstrcpy(pszBuff, (LPCTSTR)arFiles[i]);
				pszBuff = 1 + _tcschr(pszBuff, '\0');
			}

			//	while ( NULL != pos )
			//	{
			//		lstrcpy ( pszBuff, (LPCTSTR) lsDraggedFiles.GetNext ( pos ) );
			//		pszBuff = 1 + _tcschr ( pszBuff, '\0' );
			//	}

			GlobalUnlock(hgDrop);

			if (!SetClipboardData(CF_HDROP, hgDrop))
				return FALSE;

			// Put the data in the data source.
			// FORMATETC etcHDROP = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
			// ds.CacheGlobalData ( CF_HDROP, hgDrop, &etcHDROP );

		}


	{
		static UINT CF_PREFERREDDROPEFFECT = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);
		HGLOBAL hgDE = GlobalAlloc(GHND | GMEM_SHARE, sizeof(DWORD));
		if (NULL == hgDE)
			return FALSE;

		DWORD* pDE = (DWORD*)GlobalLock(hgDE);
		if (NULL == pDE)
		{
			GlobalFree(hgDE);
			return FALSE;
		}

		*pDE = bCut ? DROPEFFECT_MOVE : DROPEFFECT_COPY;

		GlobalUnlock(hgDE);

		if (!SetClipboardData(CF_PREFERREDDROPEFFECT, hgDE))
			return FALSE;

		//FORMATETC etcHDROP = { CF_PREFERREDDROPEFFECT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		//ds.CacheGlobalData ( CF_PREFERREDDROPEFFECT, hg4, &etcHDROP );
	}

	CloseClipboard();

	return TRUE;
	}

}