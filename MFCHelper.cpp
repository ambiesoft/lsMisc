#include "stdafx.h"
#include <intshcut.h> 
#include <atlconv.h>
#include <afxtempl.h>
#include <stdlib.h>
#include <Mshtml.h>

#include "PathUtil.h"
#include "MFCHelper.h"

CString GetFavoritesFolder()
{
	TCHAR           sz[MAX_PATH];
	TCHAR           szPath[MAX_PATH];
	HKEY            hKey;
	DWORD			dwType;
	DWORD           dwSize;
	CString			strPath;
	CString         strType;
	CString         strEmpty;
	LONG            retVal;

	strType = _T("Favorites");

	//if(RegOpenKey(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"), &hKey) != ERROR_SUCCESS)
	if( RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"),
		0, //reserved
		KEY_READ,
		&hKey) != ERROR_SUCCESS )
	{
			goto ERROR_END;
	}
	dwSize = sizeof(sz);
	retVal = RegQueryValueEx(hKey, 
						strType, 
						NULL, //reserved
						&dwType, //NULL, 
						(LPBYTE)sz, 
						&dwSize);
	
	ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
	RegCloseKey(hKey);

	strPath = szPath;
	if( retVal != ERROR_SUCCESS )
	{//fail
		if( RegOpenKeyEx(HKEY_CURRENT_USER,
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"),
			0, //reserved
			KEY_READ,
			&hKey) != ERROR_SUCCESS )
		{
			goto ERROR_END;
		}
		dwSize = sizeof(sz);
		retVal = RegQueryValueEx(hKey, 
							strType, 
							NULL, //reserved
							&dwType, //NULL, 
							(LPBYTE)sz, 
							&dwSize);
		
		ExpandEnvironmentStrings(sz, szPath, MAX_PATH);
		RegCloseKey(hKey);

		if( retVal == ERROR_SUCCESS )
		{//success
			strPath = szPath;
		}
		else
		{
			goto ERROR_END;
		}
	}

//	PathAddBackslash(strPath);

	return strPath;

ERROR_END:
	return _T("");
}



HRESULT CreateInterShortcut (LPCTSTR pszURL, 
							 LPCTSTR pszURLfilename,
							 LPCTSTR szDescription,
							 LPCTSTR szIconFile,
							 int nIndex)
{
	CoInitialize(NULL);

	HRESULT hres;
	IUniformResourceLocator *pHook;

	hres = CoCreateInstance (CLSID_InternetShortcut, NULL, CLSCTX_INPROC_SERVER,
		IID_IUniformResourceLocator, (void **)&pHook);

	if (SUCCEEDED (hres))
	{
		IPersistFile *ppf;
		IShellLink *psl;

		// Query IShellLink for the IPersistFile interface for 
		hres = pHook->QueryInterface (IID_IPersistFile, (void **)&ppf);
		hres = pHook->QueryInterface (IID_IShellLink, (void **)&psl);

		if (SUCCEEDED (hres))
		{ 
//			WORD wsz [MAX_PATH]; // buffer for Unicode string

			// Set the path to the shortcut target.
			pHook->SetURL(pszURL,0);

			hres = psl->SetIconLocation(szIconFile,nIndex);

			if (SUCCEEDED (hres))
			{

				// Set the description of the shortcut.
				hres = psl->SetDescription (szDescription);

				if (SUCCEEDED (hres))
				{

					// Ensure that the string consists of ANSI characters.
//					MultiByteToWideChar (CP_ACP, 0, pszURLfilename, -1, wsz, MAX_PATH);

					// Save the shortcut via the IPersistFile::Save member function.
					USES_CONVERSION;
//					hres = ppf->Save (wsz, TRUE);
					LPTSTR pT = _tcsdup(pszURLfilename);

					hres = ppf->Save(T2OLE(pT), TRUE);

					free(pT);
				}

			}

			// Release the pointer to IPersistFile.
			ppf->Release ();
			psl->Release ();
		}

		// Release the pointer to IShellLink.
		pHook->Release ();

	}

	CoUninitialize();
	return hres;
} 



BOOL IsBrowserReady(IWebBrowser2* pBrowser)
{
	if(pBrowser==NULL)
		return FALSE;

	READYSTATE rs;
	return (SUCCEEDED(pBrowser->get_ReadyState(&rs))
	        && rs!=READYSTATE_LOADING );
}


int WalkIEDocument(WALKIEDOCFUNC f, void* pVoid)
{
	CoInitialize(NULL);
	CArray<IHTMLDocument2*, IHTMLDocument2*> ardoc;

	int nBusy = 0;
	IShellWindows* pShells = NULL;
	if(SUCCEEDED(CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL, 
		IID_IShellWindows, (LPVOID*)&pShells)))
	{
		long nCount = 0;
		if( SUCCEEDED(pShells->get_Count(&nCount)) )
		{
//			BOOL bContinue = TRUE;
			VARIANT vI;
//			for(int i=0 ; bContinue && (i < nCount) ; i++ )
			for(int i=0 ; i < nCount ; i++ )
			{
				VariantInit(&vI);
				vI.vt = VT_I4;
				vI.lVal = i;

				IDispatch* pDisp = NULL;
				if(SUCCEEDED(pShells->Item(vI, &pDisp)) && pDisp )
				{
					IWebBrowser2* pBrowser = NULL;
					if( SUCCEEDED(pDisp->QueryInterface(IID_IWebBrowser2, (void**)&pBrowser)) )
					{
						if(IsBrowserReady(pBrowser))
						{
							IDispatch* pDocDisp = NULL;
							if( SUCCEEDED(pBrowser->get_Document(&pDocDisp)) && pDocDisp )
							{
								IHTMLDocument2* pHTMLDoc = NULL;
								if( SUCCEEDED(pDocDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pHTMLDoc)) )
								{
									pHTMLDoc->AddRef();
									ardoc.Add(pHTMLDoc);

//									bContinue = f(pHTMLDoc, pVoid);
									pHTMLDoc->Release();
								}

								pDocDisp->Release();
							}
						}
						else
						{
							nBusy++;
						}
						pBrowser->Release();
					}
					pDisp->Release();
				}
			} // for
		} // get_count
		
		pShells->Release();
	}

	BOOL bContinue = TRUE;
	for ( int i=0 ; i < ardoc.GetSize() ; ++i )
	{
		if(bContinue)
			bContinue = f(ardoc[i], pVoid);

		ardoc[i]->Release();
	}

	CoUninitialize();
	return nBusy;
}

// moved to heler
/**
BOOL GetCountedDir( LPTSTR szBuff, LPCTSTR lpszDir, size_t sizeBuf, int nCount )
{
	TCHAR buf[_MAX_PATH];
	TCHAR* p = buf;
	int nLengOrg = lstrlen(lpszDir);
	if( nLengOrg > sizeof(buf) )
		return FALSE;

	_tcscpy( buf, lpszDir );
	int nFindCount = 0;
	while(*p)
	{
		if (*p == _T('/') || *p == _T('\\'))
		{
			nFindCount ++;
			if( nFindCount == nCount )
			{
				*(p+1) = _T('\0');
				if( (size_t)lstrlen(buf) > sizeBuf )
					return FALSE;
				else
				{
					_tcscpy(szBuff, buf);
					return TRUE;
				}
			}
		}

        if (IsDBCSLeadByte((BYTE)*p))
            p++;

		p++;
	}
	return FALSE;
}

BOOL CreateCompleteDir(LPCTSTR lpszDir, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	TCHAR szBuff[_MAX_PATH];
	int i;
	try
	{
		for( i = 0; GetCountedDir( szBuff, lpszDir, sizeof(szBuff), i+1 ); i++ )
		{
			if( GetFileAttributes(szBuff) == 0xFFFFFFFF )
			{//Dir not exist
				if ( !CreateDirectory(szBuff,lpSecurityAttributes) )
					return FALSE;
			}
		}
	}
	catch(...) { return FALSE; }

	return CreateDirectory(lpszDir,lpSecurityAttributes);
}

**/



CString dqIfSpace(const CString& s)
{
	if(s.IsEmpty())
		return s;

	if(s[0]==_T('"'))
		return s;

	if(s.Find(_T(" ")) == -1)
		return s;

	return _T("\"") + s + _T("\"");
}




//
//BOOL ChangeFilenamable( CString& strTitle )
//{
//	TCHAR* pch = _tcsdup(strTitle);
//	TCHAR* pTemp = pch;
//	try
//	{
//		while( *pch != _T('\0') )
//		{
//			TCHAR* pOrgch = pch;
//			if( *pch==_T('\\')
//			||  *pch==_T(':')
//			||  *pch==_T('*')
//			||  *pch==_T('?')
//			||  *pch==_T('\"')
//			||  *pch==_T('<')
//			||  *pch==_T('>')
//			||  *pch==_T('|') )
//			{
//				do
//				{
//					*pOrgch = *(pOrgch+1);
//					++pOrgch;
//				}
//				while( *pOrgch );
//			}
//			else
//				pch = (TCHAR*)_mbsinc((unsigned char*)pch);
//		}
//		
//		pch = pTemp;
//		while( *pch != _T('\0') )
//		{
//			if( *pch == _T('/') )
//				*pch = _T('-');
//
//			pch = (TCHAR*)_mbsinc((unsigned char*)pch);
//		}
//	}
//	catch(...)
//	{
//		return FALSE;
//	}
//	strTitle = pTemp;
//	delete pTemp;
//	return TRUE;
//}

//BOOL CreateInterShortcutHelper (LPCTSTR pURL, 
//								LPCTSTR pDir, 
//								LPCTSTR pTitle)
//{
//	if ( pURL==NULL || pDir==NULL || pTitle==NULL )
//		return FALSE;
//
//	if ( pURL[0] == _T('\0') || pDir[0] == _T('\0') || pTitle[0] == _T('\0') )
//		return FALSE;
//
//	DWORD dwTempAttr = ::GetFileAttributes(pDir);
//	if( dwTempAttr == 0xFFFFFFFF || !(dwTempAttr & FILE_ATTRIBUTE_DIRECTORY) )
//		return FALSE;
//
//	CString strNewTitle = pTitle;
//	VERIFY(ChangeFilenamable(strNewTitle));
//
//	CString strFileName = pDir;
//	strFileName += _T('\\');
//	strFileName += strNewTitle;
//	strFileName += _T(".url");
//
//	return SUCCEEDED(CreateInterShortcut(pURL, strFileName, NULL));
//}


bool HasDupPaths(LPCTSTR pLeft, const CStringArray& saPaths, int startindex, int& hitindex,CString& common)
{
	for (int i = startindex; i < saPaths.GetSize(); ++i)
	{
		CString tmp;
		wstring dupParent;
		if (myPathIsChildIncluded(pLeft, saPaths[i], &dupParent))
		{
			common = dupParent.c_str();
			hitindex = i;
			return true;
		}
	}
	return false;
}

bool checkDupPaths(const CStringArray& saPaths, CString&left, CString& right, CString&common)
{
	for (int i = 0; i < saPaths.GetSize(); ++i)
	{
		CString tmpleft = saPaths[i];
		int hitindex = 0;
		if (HasDupPaths(tmpleft, saPaths, i + 1, hitindex,common))
		{
			left = tmpleft;
			right=saPaths[hitindex];
			return false;
		}
	}
	return true;
}

void getLinesFromEdit(CEdit& edit, CStringArray& ar, bool bRaw)
{
	int lc = edit.GetLineCount();
	for (int i = 0; i < lc; ++i)
	{
		CString t;
		int len = edit.LineLength(edit.LineIndex(i));
		edit.GetLine(i, t.GetBuffer(len), len);
		t.ReleaseBuffer();
		if (t.IsEmpty())
			continue;



		if (!bRaw)
		{
			t.TrimLeft();
			t.TrimRight();
			if (t.IsEmpty())
				continue;
			if (t[0] != _T('"') && t.Find(_T(' ')) >= 0)
				t = _T('"') + t + _T('"');
		}
		ar.Add(t);
	}
}
