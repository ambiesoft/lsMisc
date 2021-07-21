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

// do not include "stdafx.h" for Qt causes error
/*
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\ATLMFC\include\afx.h(24): fatal error C1189: #error:  Building MFC application with /MD[d] (CRT dll version) requires MFC shared dll version. Please #define _AFXDLL or do not use /MD[d]
Generating Code...
*/
// #include "stdafx.h"

#include <cassert>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif


#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include <winioctl.h>
#include <Shlobj.h>
#pragma comment(lib, "Shell32.lib")

#include <regex>
#include <locale>
#include <codecvt>
#include <string>
#include <functional>
#include "stdosd.h"


#if !defined(REPARSE_DATA_BUFFER_HEADER_SIZE)
typedef struct _REPARSE_DATA_BUFFER {
	ULONG  ReparseTag;
	USHORT ReparseDataLength;
	USHORT Reserved;
	union {
		struct {
			USHORT SubstituteNameOffset;
			USHORT SubstituteNameLength;
			USHORT PrintNameOffset;
			USHORT PrintNameLength;
			ULONG  Flags;
			WCHAR  PathBuffer[1];
		} SymbolicLinkReparseBuffer;
		struct {
			USHORT SubstituteNameOffset;
			USHORT SubstituteNameLength;
			USHORT PrintNameOffset;
			USHORT PrintNameLength;
			WCHAR  PathBuffer[1];
		} MountPointReparseBuffer;
		struct {
			UCHAR  DataBuffer[1];
		} GenericReparseBuffer;
	};
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#  define REPARSE_DATA_BUFFER_HEADER_SIZE  FIELD_OFFSET(REPARSE_DATA_BUFFER, GenericReparseBuffer)
#endif // !defined(REPARSE_DATA_BUFFER_HEADER_SIZE)


namespace Ambiesoft {
	namespace stdosd {
		using namespace std;

		namespace {
			std::wstring stdGetFullPathNameImpl(LPCWSTR pPath)
			{
				std::wstring ret;
				if (pPath == NULL || pPath[0] == 0)
					return ret;

				DWORD dwReqChar = GetFullPathNameW(pPath, 0, NULL, NULL);
				wchar_t* pBuffer = new wchar_t[dwReqChar];
				pBuffer[0] = 0;
				DWORD retChar = GetFullPathNameW(pPath, dwReqChar, pBuffer, NULL);
				if (retChar > dwReqChar)
				{
					delete[] pBuffer;
					return ret;
				}

				ret = pBuffer;
				delete[] pBuffer;

				return ret;
			}

			std::vector<std::wstring> splitdir(const std::wstring& input, const std::wstring& regex) {
				// passing -1 as the submatch index parameter performs splitting
				std::wregex re(regex);
				std::wsregex_token_iterator
					first{ input.begin(), input.end(), re, -1 },
					last;
				return { first, last };
			}

			wstring resolveSingleDir(const wstring& input)
			{
				wstring result;

				DWORD dwFA = GetFileAttributesW(input.c_str());
				if (dwFA == 0xFFFFFFFF)
					return input;
				if ((dwFA & FILE_ATTRIBUTE_REPARSE_POINT) == 0)
					return input;

				HANDLE handle = CreateFileW(input.c_str(),
					FILE_READ_EA,
					FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
					0,
					OPEN_EXISTING,
					FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
					0);
				if (handle != INVALID_HANDLE_VALUE) {
					DWORD bufsize = MAXIMUM_REPARSE_DATA_BUFFER_SIZE;
					REPARSE_DATA_BUFFER *rdb = (REPARSE_DATA_BUFFER*)malloc(bufsize);
					DWORD retsize = 0;
					if (::DeviceIoControl(handle, FSCTL_GET_REPARSE_POINT, 0, 0, rdb, bufsize, &retsize, 0)) {
						if (rdb->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT) {
							int length = rdb->MountPointReparseBuffer.SubstituteNameLength / sizeof(wchar_t);
							int offset = rdb->MountPointReparseBuffer.SubstituteNameOffset / sizeof(wchar_t);
							const wchar_t* PathBuffer = &rdb->MountPointReparseBuffer.PathBuffer[offset];
							result.assign(PathBuffer, length);
						}
						else if (rdb->ReparseTag == IO_REPARSE_TAG_SYMLINK) {
							int length = rdb->SymbolicLinkReparseBuffer.SubstituteNameLength / sizeof(wchar_t);
							int offset = rdb->SymbolicLinkReparseBuffer.SubstituteNameOffset / sizeof(wchar_t);
							const wchar_t* PathBuffer = &rdb->SymbolicLinkReparseBuffer.PathBuffer[offset];
							result.assign(PathBuffer, length);
						}
						// cut-off "//?/" and "/??/"
						if (result.size() > 4 && result.at(0) == L'\\' && result.at(2) == L'?' && result.at(3) == L'\\')
							result = result.substr(4); // mid(4);
					}
					free(rdb);
					CloseHandle(handle);
				}
				return result;
			}
		}
		std::wstring stdGetFullPathName(const wchar_t* p)
		{
			return stdGetFullPathNameImpl(p);
		}
		std::wstring resolveLink(const std::wstring& instring)
		{
			if (instring.empty())
				return instring;

			wstring full = stdGetFullPathName(instring.c_str());
			if (full.empty())
				return instring;

			//wchar_t drive[_MAX_DRIVE];
			//wchar_t dir[_MAX_DIR];
			//wchar_t name[FILENAME_MAX];
			//wchar_t ext[_MAX_EXT];
			//if (0 != _wsplitpath_s(full.c_str(),
			//	drive,
			//	dir,
			//	name,
			//	ext))
			//{
			//	return instring;
			//}

			if (PathIsUNCW(full.c_str()))
				return instring;

			vector<wstring> dirs = splitdir(full, L"\\\\");
			wstring resultdir = dirs[0];

			for (size_t i = 1; i < (dirs.size()); ++i)
			{
				resultdir += L"\\";
				resultdir += dirs[i];

				resultdir = resolveSingleDir(resultdir);
			}

			return resultdir;
		}


		static unsigned short* utf8toutf16(const LPBYTE pIN, size_t cbLen)
		{
			int nReqSize = MultiByteToWideChar(
				CP_UTF8,
				0,
				(const char*)pIN,
				(int)cbLen,
				NULL,
				0);

			if (nReqSize == 0)
				return NULL;

			// unsigned short* pOut = (unsigned short*)malloc(nReqSize * sizeof(unsigned short));
			unsigned short* pOut = new unsigned short[nReqSize+1];
			int nRet = MultiByteToWideChar(
				CP_UTF8,
				0,
				(const char*)pIN,
				(int)cbLen,
				(LPWSTR)pOut,
				nReqSize);

			if (nRet == 0 || nRet != nReqSize)
			{
				delete pOut;
				return NULL;
			}
			pOut[nReqSize] = 0;
			return pOut;
		}
  //      static wstring toWstring(const string& s)
  //      {
  //          std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  //          // std::string narrow = converter.to_bytes(wide_utf16_source_string);
  //          return converter.from_bytes(s);
  //      }
  //      static string toString(const wstring& w)
		//{
  //          std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		//	return converter.to_bytes(w);
		//}
		//static string toString(const char* p)
		//{
		//	return p;
		//}

		static unsigned short* toWstring2(const string& str)
		{
			return (unsigned short*)utf8toutf16((const LPBYTE)str.c_str(), str.size());
		}
		static string toString(const unsigned short* p)
		{
			int nReqSize = WideCharToMultiByte(
				CP_UTF8,
				0,
				(LPCWCH)p,
				-1,
				NULL,
				0,
				NULL,
				NULL);

			if (nReqSize == 0)
				return NULL;

			BYTE* pOut = new BYTE[nReqSize];
			int nRet = WideCharToMultiByte(CP_UTF8,
				0,
				(LPCWCH)p,
				-1,
				(char*)pOut,
				nReqSize,
				NULL,
				NULL);

			if (nRet == 0 || nRet != nReqSize)
			{
				delete pOut;
				return NULL;
			}

			return string((char*)pOut);
		}

        class CFileIteratorInternal
        {
            HANDLE hFF_ = INVALID_HANDLE_VALUE;
            WIN32_FIND_DATAW* pFindData_ = nullptr;
			FILEITERATEMODE fim_;
			GETFILESEMODE gfm_;
			bool loopStarted_ = false;
		private:
			static bool isDot(LPCWSTR pStr) {
				return (pStr[0] == L'.' && pStr[1] == 0);
			}
			static bool isDotDot(LPCWSTR pStr) {
				return (pStr[0] == L'.' && pStr[1] == L'.' && pStr[2] == 0);
			}
			static bool isDir(WIN32_FIND_DATAW* pFD) {
				return pFD->dwFileAttributes != 0xFFFFFFFF &&
					(pFD->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			}
			static bool isFile(WIN32_FIND_DATAW* pFD) {
				return pFD->dwFileAttributes != 0xFFFFFFFF &&
					(pFD->dwFileAttributes & FILE_ATTRIBUTE_NORMAL) != 0;
			}

		public:
            CFileIteratorInternal(HANDLE h, 
				const WIN32_FIND_DATAW* wfd, 
				FILEITERATEMODE fim,
				GETFILESEMODE gfm) :
                hFF_(h), fim_(fim), gfm_(gfm)
            {
                if(wfd)
                {
                    pFindData_ = new WIN32_FIND_DATAW;
                    *pFindData_ = *wfd;
                }
            }
            ~CFileIteratorInternal()
            {
                Close();
            }
            void Close()
            {
                if(hFF_ != INVALID_HANDLE_VALUE)
                {
                    FindClose(hFF_);
                    hFF_ = INVALID_HANDLE_VALUE;
                }
                if(pFindData_)
                {
                    delete pFindData_;
                    pFindData_=nullptr;
                }
            }

            bool valid() const {
                return hFF_ != INVALID_HANDLE_VALUE && !!pFindData_;
            }
            bool next()
            {
                if(!valid())
                    return false;
				if (!loopStarted_)
				{
					loopStarted_ = true;
					return true;
				}
                
				for (;;)
				{
					if (!FindNextFileW(hFF_, pFindData_))
					{
						Close();
						return false;
					}

					if ((fim_ & FILEITERATEMODE::SKIP_DOT) != 0 &&
						isDot(pFindData_->cFileName))
					{
						continue;
					}
					if ((fim_ & FILEITERATEMODE::SKIP_DOTDOT) != 0 &&
						isDotDot(pFindData_->cFileName))
					{
						continue;
					}

					if ((gfm_ & GETFILESEMODE::DIRECTORY) == 0 &&
						isDir(pFindData_))
					{
						continue;
					}
					if ((gfm_ & GETFILESEMODE::FILE) == 0 &&
						isFile(pFindData_))
					{
						continue;
					}

					return true;
				}
            }
			std::string name() const {
				std::string ret;
				if (!valid())
					return ret;
				return toString((unsigned short*)pFindData_->cFileName);
			}
			std::wstring wname() const {
				return pFindData_->cFileName;
			}
			bool isDir() const {
                if(!valid())
                    return false;
                return (pFindData_->dwFileAttributes != 0xFFFFFFFF) &&
                        (0 != (pFindData_->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
            }
            unsigned long long size() const {
                ULARGE_INTEGER size;
                size.HighPart = pFindData_->nFileSizeHigh;
                size.LowPart = pFindData_->nFileSizeLow;
                return size.QuadPart;
            }
        };

        HFILEITERATOR stdCreateFileIterator(
			const std::string& directory,
			FILEITERATEMODE fim, 
			GETFILESEMODE gfm,
			int depth)
        {
			std::string d(directory);
			d.erase(d.find_last_not_of("/\\")+1);
			d += "\\*";

			unique_ptr<unsigned short[]> pW(toWstring2(d));
            WIN32_FIND_DATAW wfd;
            HANDLE hFF = FindFirstFileW((LPCWSTR)pW.get(), &wfd);
			if (hFF == INVALID_HANDLE_VALUE)
			{
				return nullptr;
			}

            return new CFileIteratorInternal(hFF, &wfd, fim, gfm);
        }
		HFILEITERATOR stdCreateFileIterator(
			const std::wstring& directory,
			FILEITERATEMODE fim,
			GETFILESEMODE gfm,
			int depth)
		{
			std::wstring d(directory);
			d.erase(d.find_last_not_of(L"/\\") + 1);
			d += L"\\*";

			WIN32_FIND_DATAW wfd;
			HANDLE hFF = FindFirstFileW(d.c_str(), &wfd);
			if (hFF == INVALID_HANDLE_VALUE)
			{
				return nullptr;
			}

			return new CFileIteratorInternal(hFF, &wfd, fim, gfm);
		}
		bool stdFileNextImpl(HFILEITERATOR hFileIterator, FileInfo<char>* fi)
		{
			CFileIteratorInternal* pIterator = (CFileIteratorInternal*)hFileIterator;
			if (!pIterator->next())
				return false;

			fi->setAll(pIterator->isDir(),
				pIterator->name(),
				pIterator->size());

			return true;
		}
		bool stdFileNextImpl(HFILEITERATOR hFileIterator, FileInfo<wchar_t>* fi)
		{
			CFileIteratorInternal* pIterator = (CFileIteratorInternal*)hFileIterator;
			if (!pIterator->next())
				return false;

			fi->setAll(pIterator->isDir(),
				pIterator->wname(),
				pIterator->size());

			return true;
		}
        bool stdCloseFileIterator(HFILEITERATOR hFileIterator)
        {
            if(!hFileIterator)
                return false;
            CFileIteratorInternal* pIterator = (CFileIteratorInternal*)hFileIterator;
            pIterator->Close();

            return true;
        }

		size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, char* p, size_t size)
		{
			return GetModuleFileNameA((HINSTANCE)hInst, p, (DWORD)size);
		}
		size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, wchar_t* p, size_t size)
		{
			return GetModuleFileNameW((HINSTANCE)hInst, p, (DWORD)size);
		}


		size_t stdExpandEnvironmentStringsImpl(const char* pIN, char* p, size_t size)
		{
			return ExpandEnvironmentStringsA(pIN, p, (DWORD)size);
		}
		size_t stdExpandEnvironmentStringsImpl(const wchar_t* pIN, wchar_t* p, size_t size)
		{
			return ExpandEnvironmentStringsW(pIN, p, (DWORD)size);
		}

		static bool GetComputerNameT(char* p, size_t* pnLength)
		{
			DWORD dw = (DWORD)*pnLength;
			bool ret = !!GetComputerNameA(p, &dw);
			*pnLength = dw;
			return ret;
		}
		static bool GetComputerNameT(wchar_t* p, size_t* pnLength)
		{
			DWORD dw = (DWORD)*pnLength;
			bool ret = !!GetComputerNameW(p, &dw);
			*pnLength = dw;
			return ret;
		}

		template<typename C>
		bool stdGetComputerNameImpl(C* p, size_t size, size_t& outsize)
		{
			if (!GetComputerNameT(p, &size))
			{
				if (GetLastError() != ERROR_BUFFER_OVERFLOW)
					return false;
			}
			outsize = size;
			return true;
		}

		template<typename C>
		std::basic_string<C> stdGetComputerName()
		{
			C* p = nullptr;
			size_t size = 16;
			for (;;)
			{
				p = (C*)realloc(p, size * sizeof(C));
				size_t outsize;
				if (!stdGetComputerNameImpl(p, size, outsize))
					return std::basic_string<C>();
				if (size > outsize)
					break;

				// Make double the size of required memory
				size *= 2;
			}

			std::basic_string<C> ret = p;
			free((void*)p);
			return ret;
		}
		template std::basic_string<char> stdGetComputerName();
		template std::basic_string<wchar_t> stdGetComputerName();


        static size_t stdGetCurrentDirectoryImpl(char* p, size_t size)
		{
			return GetCurrentDirectoryA((DWORD)size, p);
		}
        static size_t stdGetCurrentDirectoryImpl(wchar_t* p, size_t size)
		{
			return GetCurrentDirectoryW((DWORD)size, p);
		}
        template<typename C>
        inline std::basic_string<C> stdGetCurrentDirectory()
        {
            C* p = nullptr;
            size_t size = 64;
            for (;;)
            {
                p = (C*)realloc(p, size * sizeof(C));
                if (stdGetCurrentDirectoryImpl(p, size) < size)
                    break;

                // Make double the size of required memory
                size *= 2;
            }

            std::basic_string<C> ret = p;
            free((void*)p);
            return ret;
        }
        template std::basic_string<char> stdGetCurrentDirectory<char>();
        template std::basic_string<wchar_t> stdGetCurrentDirectory<wchar_t>();

		bool stdGetDesktopDirectoryImpl(wstring* path)
		{
			wchar_t t[MAX_PATH];
			if(FAILED(SHGetFolderPathW(NULL, CSIDL_DESKTOP, NULL, 0, t)))
				return false;
			*path = t;
			return true;
		}
		bool stdGetDesktopDirectoryImpl(string* path)
		{
			char t[MAX_PATH];
			if (FAILED(SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, t)))
				return false;
			*path = t;
			return true;
		}

		bool stdGetWindowText(HWND, std::string*)
		{
			assert(false);
			return false;
		}
		bool stdGetWindowText(HWND hWindow, std::wstring* result)
		{
			int len = GetWindowTextLengthW(hWindow);
			if (GetLastError() != NO_ERROR)
				return false;
			if (len == 0)
			{
				*result = std::wstring();
				return true;
			}
			vector<wchar_t> v;
			v.resize(len + 1);
			if (len != GetWindowTextW(hWindow, v.data(), len + 1))
				return false;
			assert(v[len] == L'\0');
			*result = v.data();
			return true;
		}
		bool stdSetWindowText(HWINDOWHANDLE hWindow, const std::wstring& text)
		{
			return !!SetWindowTextW(hWindow, text.c_str());
		}

	}
}
