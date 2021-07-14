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
#include <wininet.h>
#include <urlhist.h>
#include <shlguid.h>

#pragma comment(lib,"Wininet.lib")

BOOL DeleteIECache(BOOL bNormal, BOOL bCookie, BOOL bHistory) 
{
    BOOL bResult = FALSE;
    BOOL bDone = FALSE;
    LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
 
    DWORD  dwTrySize, dwEntrySize = 4096; // start buffer size    
    HANDLE hCacheDir = NULL;    
    DWORD  dwError = ERROR_INSUFFICIENT_BUFFER;
    
	if( bNormal || bCookie || bHistory)
	{
		do 
		{                               
			switch (dwError)
			{
				// need a bigger buffer
				case ERROR_INSUFFICIENT_BUFFER: 
					delete [] lpCacheEntry;
					lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
					lpCacheEntry->dwStructSize = dwEntrySize;
					dwTrySize = dwEntrySize;
					BOOL bSuccess;
					if (hCacheDir == NULL)                
                  
						bSuccess = (hCacheDir 
						  = FindFirstUrlCacheEntry(NULL, lpCacheEntry,
						  &dwTrySize)) != NULL;
					else
						bSuccess = FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize);

					if (bSuccess)
						dwError = ERROR_SUCCESS;    
					else
					{
						dwError = GetLastError();
						dwEntrySize = dwTrySize; // use new size returned
					}
					break;

				 // we are done
				case ERROR_NO_MORE_ITEMS:
					bDone = TRUE;
					bResult = TRUE;                
					break;

				 // we have got an entry
				case ERROR_SUCCESS:                       

					//Normal
					if( bNormal && bCookie )
						 DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
					else if( bNormal && !bCookie )
					{

						if( !(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY) )
						{//not cookie
							DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
						}
					}
					else if( !bNormal && bCookie )
					{
						if( (lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY) )
						{//cookie
							DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
						}
					}

					if( bHistory )
					{
						if( lpCacheEntry->CacheEntryType & URLHISTORY_CACHE_ENTRY )
						{// History
							DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
						}
					}

					dwTrySize = dwEntrySize;
					if (FindNextUrlCacheEntry(hCacheDir, lpCacheEntry, &dwTrySize))
						dwError = ERROR_SUCCESS;          
     
					else
					{
						dwError = GetLastError();
						dwEntrySize = dwTrySize; // use new size returned
					}                    
					break;

				// unknown error
				default:
					bDone = TRUE;                
					break;
			}

			if (bDone)
			{   
				delete [] lpCacheEntry; 
				if (hCacheDir)
					FindCloseUrlCache(hCacheDir);         
                                  
			}
		} while (!bDone);
	}
	else
	{
		bResult = TRUE;
	}

	if( bHistory )
	{
		OleInitialize(NULL);
		IUrlHistoryStg2* pUrlHistoryStg2 = NULL;
		HRESULT hr = CoCreateInstance(CLSID_CUrlHistory,
			NULL, CLSCTX_INPROC, IID_IUrlHistoryStg2,
			(void**)&pUrlHistoryStg2);
		if (SUCCEEDED(hr))
		{         
		   hr = pUrlHistoryStg2->ClearHistory(); 
		   pUrlHistoryStg2->Release();
		}
		
		bResult &= SUCCEEDED(hr);
		OleUninitialize();
	}

    return bResult;
}


