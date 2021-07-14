


#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif




// https://msdn.microsoft.com/en-us/library/aa382380%28VS.85%29.aspx
#include <windows.h>
#include <tchar.h>
#include <Wincrypt.h>
#include <stdio.h>
#include <string>
using namespace std;

#include "md5.h"

#define BUFSIZE 1024
#define MD5LEN  16



DWORD md5A(LPCTSTR filename, string& ret)
{
//    BOOL bResult = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    HANDLE hFile = NULL;
    DWORD cbRead = 0;
    DWORD cbHash = 0;
    CHAR rgbDigits[] = "0123456789abcdef";
//    LPCTSTR filename=_T("filename.txt");
    // Logic to check usage goes here.

    hFile = CreateFile(filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN,
        NULL);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        return GetLastError();
    }

	struct Filecloser{
		HANDLE h_;
		Filecloser(HANDLE h){h_=h;}
		~Filecloser(){CloseHandle(h_);}
	} filecloser(hFile);

	return md5A(hFile,ret);
}

DWORD md5A(HANDLE hFile, string& ret)
{
    BOOL bResult = FALSE;
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE rgbFile[BUFSIZE];
    DWORD cbRead = 0;
    BYTE rgbHash[MD5LEN];
    DWORD cbHash = 0;
    CHAR rgbDigits[] = "0123456789abcdef";


    // Get handle to the crypto provider
    if (!CryptAcquireContext(&hProv,
        NULL,
        NULL,
        PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT))
    {
        return GetLastError();
    }
	struct CryptReleaser{
		HCRYPTPROV h_;
		CryptReleaser(HCRYPTPROV h){h_=h;}
		~CryptReleaser() { CryptReleaseContext(h_,0); }
	} cryptReleaser(hProv);


    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
    {
        return GetLastError();
    }
	struct CryptDestor {
		HCRYPTHASH h_;
		CryptDestor(HCRYPTHASH h) { h_ = h; }
		~CryptDestor() { CryptDestroyHash(h_); }
	} cryptDestor(hHash);


    while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, 
        &cbRead, NULL))
    {
        if (0 == cbRead)
        {
            break;
        }

        if (!CryptHashData(hHash, rgbFile, cbRead, 0))
        {
            return GetLastError();
        }
    }

    if (!bResult)
    {
        return GetLastError();
    }

    cbHash = MD5LEN;

    if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
    {
        // printf("MD5 hash of file %s is: ", filename);
		CHAR szT[16];
        for (DWORD i = 0; i < cbHash; i++)
        {

            wsprintfA(szT,
				("%c%c"), 
				rgbDigits[rgbHash[i] >> 4],
                rgbDigits[rgbHash[i] & 0xf]);
			ret += szT;
        }
        // printf("\n");
    }
    else
    {
        return GetLastError();
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    CloseHandle(hFile);

    return 0;
}


DWORD string2hashA(const char *stro)
{
	const unsigned char* str = (const unsigned char*)stro;
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

DWORD FileHandleToDwordHash(HANDLE hFile, DWORD* hash)
{
	string s;
	DWORD error = md5A(hFile, s);
	if(error != 0)
		return error;

	*hash = string2hashA(s.c_str());
	return 0;
}
