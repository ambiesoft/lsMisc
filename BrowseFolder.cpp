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



#include <windows.h>
#include <comdef.h> 
#include <tchar.h>
#include <objbase.h>

// warning C4091: 'typedef ': ignored on left of 'tagGPFIDL_FLAGS' when no variable is declared
#pragma warning(push)
#pragma warning(disable: 4091)
#include <ShlObj.h>
#pragma warning(pop)

#include <io.h>

#include "tstring.h"
#include "browseFolder.h"

//#define TRACE(t1,t2) (void)0

#pragma comment(lib,"Shell32.lib")


#ifdef __cplusplus_cli
#include <vcclr.h>
#endif

#ifdef UNICODE
#if _WIN32_WINNT >= 0x0600
#define ISOVERVISTA
#endif // _WIN32_WINNT >= 0x0600
#endif // UNICODE

#ifdef ISOVERVISTA
	#include <ShObjIdl.h>
#endif

#include "DebugNew.h"

namespace Ambiesoft {





#ifdef ISOVERVISTA

_COM_SMARTPTR_TYPEDEF(IFileOpenDialog, __uuidof(IFileOpenDialog));
_COM_SMARTPTR_TYPEDEF(IShellItem, __uuidof(IShellItem));
_COM_SMARTPTR_TYPEDEF(IFileDialog, __uuidof(IFileDialog));

static IShellItemPtr getShellItemFromPath(LPWSTR pPath)
{
	typedef HRESULT  (STDAPICALLTYPE*fnSHCreateItemFromParsingName)(
		PCWSTR   pszPath,
		IBindCtx *pbc,
		REFIID   riid,
		void     **ppv
		);
	IShellItemPtr pRet;
	HMODULE h = LoadLibrary(_T("shell32.dll"));
	if(h)
	{
		fnSHCreateItemFromParsingName fn = (fnSHCreateItemFromParsingName)
			GetProcAddress(h, "SHCreateItemFromParsingName");
		if(fn)
		{
			fn(pPath, NULL, IID_IShellItem, (void**)&pRet);
		}
		FreeLibrary(h);
	}

	return pRet;
}
static bool bfVista(HWND hWnd, LPCTSTR lpszTitle, LPTSTR pFolder, bool& handled)
{
	handled = false;
	if (FAILED(CoInitializeEx(NULL,
		COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
	{
		return false;
	}
	struct CoUn {
		~CoUn() {
			CoUninitialize();
		}
	} coUn;

	IFileDialogPtr spFileDialog;
	HRESULT hr = spFileDialog.CreateInstance(__uuidof(FileOpenDialog));
	if (FAILED(hr))
		return false;

	DWORD dwOptions;
	if (FAILED(spFileDialog->GetOptions(&dwOptions)))
		return false;

	if (FAILED(spFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM )))
		return false;

	if (FAILED(spFileDialog->SetTitle(lpszTitle)))
		return false;


	IShellItemPtr pDefault;
	if(pFolder[0])
	{
		DWORD dwAttr = GetFileAttributes(pFolder);
		if(dwAttr != 0xFFFFFFFF && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
		{
			pDefault = getShellItemFromPath(pFolder);
		}
	}


	if(pDefault != NULL)
	{
		// spFileDialog->SetDefaultFolder(pDefault);
		spFileDialog->SetFolder(pDefault);
	}

	bool ret;
	hr = spFileDialog->Show(hWnd);
	if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
	{
		handled = true;
		return false;
	}
	else if (SUCCEEDED(hr)) 
	{
		ret = true;
	}
	else
	{
		return false;
	}




	IShellItemPtr psi;
	if (FAILED(spFileDialog->GetResult(&psi)))
		return false;

	LPWSTR szFileName = NULL;
	if (FAILED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &szFileName)))
		return false;

	lstrcpy(pFolder, szFileName);
	CoTaskMemFree(szFileName);

	handled = true;
	return ret;
}
#endif // #if ISOVERVISTA

struct FOLDER_PROPS
{
	LPCTSTR lpszTitle;
	LPCTSTR lpszInitialFolder;
	UINT ulFlags;
};

static int CALLBACK BrowseCallbackProc(HWND hWnd,		// Window handle to the browse dialog box
									   UINT uMsg,		// Value identifying the event
									   LPARAM lParam,	// Value dependent upon the message 
									   LPARAM lpData)	// Application-defined value that was 
									   // specified in the lParam member of the 
									   // BROWSEINFO structure
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:		// sent when the browse dialog box has finished initializing. 
		{
			// TRACE(_T("hWnd=%X\n"), hWnd);

			// remove context help button from dialog caption
			LONG lStyle = ::GetWindowLong(hWnd, GWL_STYLE);
			lStyle &= ~DS_CONTEXTHELP;
			::SetWindowLong(hWnd, GWL_STYLE, lStyle);
			lStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			lStyle &= ~WS_EX_CONTEXTHELP;
			::SetWindowLong(hWnd, GWL_EXSTYLE, lStyle);

			FOLDER_PROPS *fp = (FOLDER_PROPS *)lpData;
			if (fp)
			{
				if (fp->lpszInitialFolder && (fp->lpszInitialFolder[0] != _T('\0')))
				{
					// set initial directory
					::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)fp->lpszInitialFolder);
				}

				if (fp->lpszTitle && (fp->lpszTitle[0] != _T('\0')))
				{
					// set window caption
					::SetWindowText(hWnd, fp->lpszTitle);
				}
			}

			// SizeBrowseDialog(hWnd, fp);
		}
		break;

	case BFFM_SELCHANGED:		// sent when the selection has changed
		{
			TCHAR szDir[MAX_PATH * 2] = { 0 };

			// fail if non-filesystem
			BOOL bRet = SHGetPathFromIDList((LPITEMIDLIST)lParam, szDir);
			if (bRet)
			{
				// fail if folder not accessible
				if (_taccess(szDir, 00) != 0)
				{
					bRet = FALSE;
				}
				else
				{
					SHFILEINFO sfi;
					::SHGetFileInfo((LPCTSTR)lParam, 0, &sfi, sizeof(sfi),
						SHGFI_PIDL | SHGFI_ATTRIBUTES);
					// TRACE(_T("dwAttributes=0x%08X\n"), sfi.dwAttributes);

					// fail if pidl is a link
					if (sfi.dwAttributes & SFGAO_LINK)
					{
						// TRACE(_T("SFGAO_LINK\n"),0);
						bRet = FALSE;
					}
				}
			}

			// if invalid selection, disable the OK button
			if (!bRet)
			{
				::EnableWindow(GetDlgItem(hWnd, IDOK), FALSE);
			}

			// TRACE(_T("szDir=%s\n"), szDir);
		}
		break;
	}

	return 0;
}

static BOOL bfXp(HWND hParent, LPCTSTR lpszTitle, LPTSTR pFolder)
{
	BOOL bRet = FALSE;

	TCHAR szDisplayName[_MAX_PATH];
	TCHAR szBuffer[_MAX_PATH];

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hParent;
	// set root at Desktop
	browseInfo.pidlRoot = NULL;
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = lpszTitle;
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;   // also passed in

	FOLDER_PROPS fprs = { 0 };
	fprs.lpszInitialFolder = pFolder;
	fprs.lpszTitle = lpszTitle;
	browseInfo.lpfn = BrowseCallbackProc;
	browseInfo.lParam = (LPARAM)&fprs;

	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the
		// item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer) && szBuffer[0] != '\0')
		{
			lstrcpy(pFolder, szBuffer);
			bRet = TRUE;
		}

		// lpMalloc->Free(lpItemIDList);
		CoTaskMemFree(lpItemIDList);
	}

	return bRet;
}


BOOL browseFolder(HWND hParent, LPCTSTR lpszTitle, LPTSTR pFolder)
{
#ifdef ISOVERVISTA
	bool handled = false;
	bool ret = bfVista(hParent, lpszTitle, pFolder, handled);
	if (handled)
		return ret;
#endif
	return bfXp(hParent, lpszTitle, pFolder);
}
BOOL browseFolder(HWND hParent, LPCTSTR lpszTitle, tstring folder)
{
	return FALSE;
}

#ifdef __cplusplus_cli
BOOL browseFolder(System::Windows::Forms::IWin32Window^ win, System::String^ title, System::String^% folder)
{
	pin_ptr<const wchar_t> pTitle = PtrToStringChars(title);
	pin_ptr<const wchar_t> pFolder = PtrToStringChars(folder);
	TCHAR szFolder[MAX_PATH];
	if(pFolder)
		lstrcpyW(szFolder, pFolder);
	else
		szFolder[0]=0;

	bool handled = false;
	bool ret = bfVista(win ? (HWND)win->Handle.ToPointer():NULL, pTitle, szFolder, handled);
	if (handled)
	{
		if(ret)
			folder = gcnew System::String(szFolder);
		return ret;
	}
	System::Windows::Forms::FolderBrowserDialog fbd;
	if(!System::String::IsNullOrEmpty(folder))
		fbd.SelectedPath = folder;
	if(System::Windows::Forms::DialogResult::OK != fbd.ShowDialog(win))
		return false;

	folder = fbd.SelectedPath;
	return true;
}
#endif  // __cplusplus_cli

} // namespace