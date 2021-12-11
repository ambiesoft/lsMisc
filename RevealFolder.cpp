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








#include <windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

#include <Shlobj.h>
#pragma comment(lib,"Shell32.lib")

#include <Shlobj.h>
#include <Shobjidl.h>
#include <comdef.h>
#include <comdefsp.h>

#include <tchar.h>
#include <assert.h>
#include <string>
#include "DebugNew.h"
// #include "stlScopedClear.h"
#include "RevealFolder.h"
// #include "GetOpenFile.h"

using namespace std;
namespace Ambiesoft
{

	// http://www.kab-studio.biz/Programing/Codian/ShellExtension/05.html
	static BOOL PolulateDirectory(IShellFolderPtr pSF, LPITEMIDLIST pFolderIDList)
	{
		HRESULT hRes;
		
		
		
		IShellFolderPtr	pCurFolder;
		IEnumIDListPtr	pEnumIDList;

		if (pFolderIDList != NULL)
		{
			hRes = pSF->BindToObject(pFolderIDList, NULL, IID_IShellFolder, (LPVOID *)&pCurFolder);
			if (hRes != NOERROR)
				return TRUE;
		}
		else
		{
			hRes = ::SHGetDesktopFolder(&pCurFolder);
			if (hRes != NOERROR)
				return TRUE;
		}


		hRes = pCurFolder->EnumObjects(
			NULL,
			SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN | SHCONTF_FOLDERS, 
			&pEnumIDList);
		if (hRes != NOERROR)
			return FALSE;

		LPITEMIDLIST pFileIDList;
		ULONG ulRetNo;
		while (pEnumIDList->Next(1, &pFileIDList, &ulRetNo) == NOERROR)
		{
			STRRET stFileName = { 0 };
			stFileName.uType = -1;
			hRes = pCurFolder->GetDisplayNameOf(
				pFileIDList,
				SHGDN_FORPARSING,
				&stFileName);
			if (hRes != NOERROR)
				break;

			if (stFileName.uType == STRRET_WSTR)
				CoTaskMemFree(stFileName.pOleStr);
			CoTaskMemFree(pFileIDList);
		}

		return TRUE;
	}


	static LPITEMIDLIST GetItemIDList(IShellFolderPtr pSF, LPCWSTR pFolder)
	{
		if (pFolder == NULL || pFolder[0] == 0)
			return NULL;

		HRESULT		hRes;
		ULONG		chEaten;
		ULONG		dwAttributes;
		LPITEMIDLIST	pIDL = NULL;

		hRes = pSF->ParseDisplayName(NULL, NULL, (LPWSTR)pFolder, &chEaten, &pIDL, &dwAttributes);

		if (hRes != NOERROR)
			pIDL = NULL;

		return pIDL;
	}

	struct CItemIdList {
		LPITEMIDLIST pIIDL_ = nullptr;
		CItemIdList(LPITEMIDLIST piidl) : pIIDL_(piidl) {}
		~CItemIdList() {
			CoTaskMemFree(pIIDL_);
		}
		LPITEMIDLIST get() const {
			return pIIDL_;
		}
	};
	bool RevealFolder(LPCWSTR pFolder)
	{
		IShellFolderPtr pSF;
		HRESULT hRes = ::SHGetDesktopFolder(&pSF);
		if (!pSF)
			return false;

		CItemIdList iidl = GetItemIDList(pSF, pFolder);
		if (!iidl.get())
			return false;

		if (!PolulateDirectory(pSF, iidl.get()))
			return false;

		return true;
	}



























	//BOOL RevealFolder_obsolete(LPTSTR lpString)
	//{
	//	if (PathIsDirectory(lpString))
	//		return TRUE;

	//	if (PathFileExists(lpString))
	//		return TRUE;

	//	wstring fff(lpString);
	//	fff += L"*.*";
	//	WIN32_FIND_DATA fd;
	//	FindFirstFile(fff.c_str(), &fd);
	//	HRESULT hr;
	//	// hr = ShowShareFolderUI(nullptr, lpString);

	//	GetOpenFile(NULL,
	//		NULL,
	//		lpString,
	//		L"",
	//		NULL);
	//	ITEMIDLIST* pII = nullptr;
	//	hr = SHParseDisplayName(
	//		lpString,
	//		NULL,
	//		&pII,
	//		SFGAO_SHARE,
	//		NULL);
	//	
	//	if (SUCCEEDED(hr))
	//	{
	//		IShellItem* pShellItem = nullptr;
	//		hr = SHCreateShellItem(NULL, NULL, pII, &pShellItem);
	//		if (SUCCEEDED(hr))
	//			return TRUE;
	//	}
	//	return FALSE;
	//}
}