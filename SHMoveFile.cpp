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

#include <windows.h>
#include <tchar.h>
#include <assert.h>

#if _MSC_VER <= 1200
#pragma warning(disable:4786)
#endif

#ifdef __cplusplus_cli
#if defined(_M_CEE_PURE) || defined(_M_CEE_SAFE)
#error("could not compile with /clr:safe or /clr:pure, use /clr instead.")
#endif
#endif // __cplusplus_cli


#include <vector>
#include <string>
#include <memory>
// #include <stlsoft/smartptr/scoped_handle.hpp>
#include <functional>

#include "GetLastErrorString.h"
#include "SHMoveFile.h"
#include "vc6securecrt.h"
#include "DebugNew.h"

#pragma comment(lib, "Shell32.lib")

namespace Ambiesoft {
	using std::vector;
	using std::wstring;

#define		DE_SAMEFILE			0x71
#define		DE_MANYSRC1DEST		0x72
#define		DE_DIFFDIR			0x73
#define		DE_ROOTDIR			0x74
#define		DE_OPCANCELLED		0x75
#define		DE_DESTSUBTREE		0x76
#define		DE_ACCESSDENIEDSRC	0x78
#define		DE_PATHTOODEEP		0x79
#define		DE_MANYDEST			0x7A
#define		DE_INVALIDFILES		0x7C
#define		DE_DESTSAMETREE		0x7D
#define		DE_FLDDESTISFILE	0x7E
#define		DE_FILEDESTISFLD	0x80
#define		DE_FILENAMETOOLONG	0x81
#define		DE_DEST_IS_CDROM	0x82
#define		DE_DEST_IS_DVD		0x83
#define		DE_DEST_IS_CDRECORD	0x84
#define		DE_FILE_TOO_LARGE	0x85
#define		DE_SRC_IS_CDROM		0x86
#define		DE_SRC_IS_DVD		0x87
#define		DE_SRC_IS_CDRECORD	0x88
#define		DE_ERROR_MAX		0xB7
//#define		0x402			0x402
#define		ERRORONDEST			0x10000

	wstring GetSHFileOpErrorString(int nRet)
	{
		wstring error;
		switch (nRet)
		{
		case DE_SAMEFILE: /*0x71*/error = L"The source and destination files are the same file."; break;
		case DE_MANYSRC1DEST:/*0x72*/error = L"Multiple file paths were specified in the source buffer, but only one destination file path."; break;
		case DE_DIFFDIR:/*0x73*/error = L"Rename operation was specified but the destination path is a different directory.Use the move operation instead."; break;
		case DE_ROOTDIR:/*0x74*/error = L"The source is a root directory, which cannot be moved or renamed."; break;
		case DE_OPCANCELLED:/*0x75*/error = L"The operation was canceled by the user, or silently canceled if the appropriate flags were supplied to SHFileOperation."; break;
		case DE_DESTSUBTREE:/*0x76*/error = L"The destination is a subtree of the source."; break;
		case DE_ACCESSDENIEDSRC:/*0x78*/error = L"Security settings denied access to the source."; break;
		case DE_PATHTOODEEP:/*0x79*/error = L"The source or destination path exceeded or would exceed MAX_PATH."; break;
		case DE_MANYDEST:/*0x7A*/error = L"The operation involved multiple destination paths, which can fail in the case of a move operation."; break;
		case DE_INVALIDFILES:/*0x7C*/error = L"The path in the source or destination or both was invalid."; break;
		case DE_DESTSAMETREE:/*0x7D*/error = L"The source and destination have the same parent folder."; break;
		case DE_FLDDESTISFILE:/*0x7E*/error = L"The destination path is an existing file."; break;
		case DE_FILEDESTISFLD:/*0x80*/error = L"The destination path is an existing folder."; break;
		case DE_FILENAMETOOLONG:/*0x81*/error = L"The name of the file exceeds MAX_PATH."; break;
		case DE_DEST_IS_CDROM:/*0x82*/error = L"The destination is a read - only CD - ROM, possibly unformatted."; break;
		case DE_DEST_IS_DVD:/*0x83*/error = L"The destination is a read - only DVD, possibly unformatted."; break;
		case DE_DEST_IS_CDRECORD:/*0x84*/error = L"The destination is a writable CD - ROM, possibly unformatted."; break;
		case DE_FILE_TOO_LARGE:/*0x85*/error = L"The file involved in the operation is too large for the destination media or file system."; break;
		case DE_SRC_IS_CDROM:/*0x86*/error = L"The source is a read - only CD - ROM, possibly unformatted."; break;
		case DE_SRC_IS_DVD:/*0x87*/error = L"The source is a read - only DVD, possibly unformatted."; break;
		case DE_SRC_IS_CDRECORD:/*0x88*/error = L"The source is a writable CD - ROM, possibly unformatted."; break;
		case DE_ERROR_MAX:/*0xB7*/error = L"MAX_PATH was exceeded during the operation."; break;
		case 0x402:/*0x402*/error = L"An unknown error occurred.This is typically due to an invalid path in the source or destination.This error does not occur on Windows Vista and later."; break;
		case ERRORONDEST:/*0x10000*/error = L"An unspecified error occurred on the destination."; break;
		case DE_ROOTDIR | ERRORONDEST:/*0x10074*/error = L"Destination is a root directory and cannot be renamed."; break;
		default:
			error = GetLastErrorString(nRet);
		}
		return error;
	}


	typedef vector<wstring> STRINGVECTOR;
	
	static void* myAlloc(size_t size)
	{
		return calloc(1, size);
	}
	static void myFree(void* p)
	{
		free(p);
	}


	static BOOL SHCopyOrMoveFileImplEx(
		HWND hWnd,
		UINT func,
		bool ismultidest, 
		LPCTSTR lpzzFileFrom, 
		LPCTSTR lpzzFileTo,
		FILEOP_FLAGS fopFlags=FOF_ALLOWUNDO,
		int* pnRet=NULL)
	{
		SHFILEOPSTRUCT sfo = { 0 };
		sfo.hwnd = hWnd;
		sfo.wFunc = func;
		sfo.pFrom = lpzzFileFrom;
		sfo.pTo = lpzzFileTo;
		sfo.fFlags = fopFlags |
			(ismultidest ? FOF_MULTIDESTFILES : 0) |
			0;

		int nRet = SHFileOperation(&sfo);//&& !sfo.fAnyOperationsAborted;
		if (pnRet)
			*pnRet = nRet;
		// it seems SHFileOperation returns 113 when same path specified.
		return nRet == 0 || nRet == 113;
	}



	static LPTSTR CreateDNString(const vector<wstring>& files, int* pnRet)
	{
		size_t size = 0;
		STRINGVECTOR::const_iterator it;
		for ( it = files.begin(); it != files.end(); ++it)
		{
			size += (it->size() + 1) * sizeof(WCHAR);
		}

		size += sizeof(WCHAR);
		BYTE* p = (BYTE*)myAlloc(size);
		LPTSTR pStart = (LPTSTR)p;
		for ( it = files.begin(); it != files.end(); ++it)
		{
			size_t copysize = (it->size()+1) * sizeof(WCHAR);
			errno_t err = memcpy_s(p, size, it->c_str(), copysize);
			if (err != 0)
			{
				if (pnRet)
					*pnRet = err;
				myFree((void*)pStart);
				return NULL;
			}
			p += copysize;
			size -= copysize;
		}
		*p = 0;
		return pStart;
	}
	static LPTSTR CreateDNString(LPCWSTR pW, int* pnRet)
	{
		vector<wstring> vs;
		vs.push_back(pW);
		return CreateDNString(vs, pnRet);
	}

	
	static BOOL SHCopyOrMoveFileEx(HWND hWnd, 
		UINT func,
		LPCTSTR lpFileFrom, 
		LPCTSTR lpFileTo,
		FILEOP_FLAGS fopFlags,
		int* pnRet)
	{
		if (func != FO_DELETE)
		{
			assert(lpFileTo && lpFileFrom);
			if (!(lpFileTo && lpFileFrom))
				return FALSE;
		}
		else if (func == FO_DELETE)
		{
			assert(lpFileFrom);
			assert(!lpFileTo);
			if (!lpFileFrom)
				return FALSE;
		}

		std::unique_ptr<TCHAR, std::function<void(void*)>> from(CreateDNString(lpFileFrom, pnRet), myFree);

		std::unique_ptr<TCHAR, std::function<void(void*)>> to(nullptr, myFree);
		if (func != FO_DELETE)
		{
			to.reset(CreateDNString(lpFileTo, pnRet));
		}

		return SHCopyOrMoveFileImplEx(hWnd, func, false, from.get(), to.get(), fopFlags, pnRet);
	}
	int SHMoveFileEx(HWND hWnd, LPCTSTR lpFileFrom, LPCTSTR lpFileTo, FILEOP_FLAGS fopFlags)
	{
		int ret = -1;
		SHCopyOrMoveFileEx(hWnd, FO_MOVE, lpFileFrom, lpFileTo, fopFlags, &ret);
		return ret;
	}
	int SHMoveFileEx(LPCTSTR lpFileFrom, LPCTSTR lpFileTo, FILEOP_FLAGS fopFlags)
	{
		return SHMoveFileEx(NULL, lpFileFrom, lpFileTo, fopFlags);
	}
	
	int SHCopyFileEx(HWND hWnd, LPCTSTR lpFileFrom, LPCTSTR lpFileTo, FILEOP_FLAGS fopFlags)
	{
		int ret = -1;
		SHCopyOrMoveFileEx(hWnd, FO_COPY, lpFileFrom, lpFileTo, fopFlags, &ret);
		return ret;
	}
	int SHCopyFileEx( LPCTSTR lpFileFrom, LPCTSTR lpFileTo, FILEOP_FLAGS fopFlags)
	{
		return SHCopyFileEx(NULL,  lpFileFrom, lpFileTo, fopFlags);
	}
	
	int SHDeleteFileEx(HWND hWnd, LPCTSTR lpFile, FILEOP_FLAGS fopFlags)
	{
		int ret = -1;
		SHCopyOrMoveFileEx(hWnd, FO_DELETE, lpFile, NULL, fopFlags, &ret);
		return ret;
	}
	int SHDeleteFileEx(LPCTSTR lpFile, FILEOP_FLAGS fopFlags)
	{
		return SHDeleteFileEx(NULL, lpFile, fopFlags);
	}



	// Move many file to 1 folder
	static BOOL SHCopyOrMoveFileEx(
		HWND hWnd,
		UINT func, 
		const std::vector<std::wstring>& sourcefiles,
		LPCTSTR lpFileTo, 
		FILEOP_FLAGS fopFlags,
		int* pnRet)
	{
		std::unique_ptr<TCHAR, std::function<void(void*)>> from(CreateDNString(sourcefiles, pnRet), myFree);
		std::unique_ptr<TCHAR, std::function<void(void*)>> to(nullptr, myFree);
		if (func != FO_DELETE)
		{
			to.reset(CreateDNString(lpFileTo, pnRet));
		}

		return SHCopyOrMoveFileImplEx(hWnd, func, false,  from.get(), to.get(), fopFlags, pnRet);
	}
	int SHMoveFileEx(
		HWND hWnd,
		const std::vector<std::wstring>& sourcefiles,
		LPCTSTR lpFileTo,
		FILEOP_FLAGS fopFlags)
	{
		int ret = -1;
		SHCopyOrMoveFileEx(hWnd, FO_MOVE,  sourcefiles, lpFileTo, fopFlags, &ret);
		return ret;
	}
	int SHMoveFileEx(
		const std::vector<std::wstring>& sourcefiles,
		LPCTSTR lpFileTo,
		FILEOP_FLAGS fopFlags)
	{
		return SHMoveFileEx(NULL, sourcefiles, lpFileTo, fopFlags);
	}

	// Copy many files to 1 folder
	int SHCopyFileEx(
		HWND hWnd,
		const std::vector<std::wstring>& sourcefiles,
		LPCTSTR lpFileTo,
		FILEOP_FLAGS fopFlags)
	{
		int ret = -1;
		SHCopyOrMoveFileEx(hWnd, FO_COPY,  sourcefiles, lpFileTo, fopFlags, &ret);
		return ret;
	}
	int SHCopyFileEx(
		const std::vector<std::wstring>& sourcefiles,
		LPCTSTR lpFileTo,
		FILEOP_FLAGS fopFlags)
	{
		return SHCopyFileEx(NULL,  sourcefiles, lpFileTo, fopFlags);
	}
	
	int SHDeleteFileEx(
		HWND hWnd,
		const std::vector<std::wstring>& files,
		FILEOP_FLAGS fopFlags)
	{
		int ret = -1;
		SHCopyOrMoveFileEx(hWnd, FO_DELETE,  files, NULL, fopFlags, &ret);
		return ret;
	}
	int SHDeleteFileEx(const std::vector<std::wstring>& files,
		FILEOP_FLAGS fopFlags)
	{
		return SHDeleteFileEx(NULL, files, fopFlags);
	}

	// move multiple files to multiple files
	static BOOL SHCopyOrMoveFileEx(
		HWND hWnd,
		UINT func, 
		const vector<wstring>& sourcefiles, 
		const vector<wstring>& destfiles,
		FILEOP_FLAGS fopFlags,
		int* pnRet)
	{
		std::unique_ptr<TCHAR, std::function<void(void*)>> from(CreateDNString(sourcefiles, pnRet), myFree);
		std::unique_ptr<TCHAR, std::function<void(void*)>> to(CreateDNString(destfiles, pnRet), myFree);
		
		return SHCopyOrMoveFileImplEx(hWnd, func, true,  from.get(), to.get(), fopFlags, pnRet);
	}


	int SHMoveFileEx(
		HWND hWnd,
		const vector<wstring>& sourcefiles,
		const vector<wstring>& destfiles,
		FILEOP_FLAGS fopFlags)
	{
		int ret = -1;
		SHCopyOrMoveFileEx(hWnd, FO_MOVE,  sourcefiles, destfiles, fopFlags, &ret);
		return ret;
	}
	int SHMoveFileEx(
		const vector<wstring>& sourcefiles,
		const vector<wstring>& destfiles,
		FILEOP_FLAGS fopFlags)
	{
		return SHMoveFileEx(NULL,  sourcefiles, destfiles, fopFlags);
	}

	int SHCopyFileEx(
		HWND hWnd,
		const vector<wstring>& sourcefiles,
		const vector<wstring>& destfiles,
		FILEOP_FLAGS fopFlags)
	{
		int ret;
		SHCopyOrMoveFileEx(hWnd, FO_COPY,  sourcefiles, destfiles, fopFlags, &ret);
		return ret;
	}
	int SHCopyFileEx(
		const vector<wstring>& sourcefiles,
		const vector<wstring>& destfiles,
		FILEOP_FLAGS fopFlags)
	{
		return SHCopyFileEx(NULL,  sourcefiles, destfiles, fopFlags);
	}

} // namespace