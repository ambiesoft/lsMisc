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


// #include "StdAfx.h"
#include <regex>
#include <cassert>
#include <dirent.h>
#include "stdosd.h"

namespace Ambiesoft {
	namespace stdosd {
		using namespace std;

		namespace {
			std::wstring stdGetFullPathName(const wchar_t* pPath)
			{
				// not implemented
				assert(false);
				return std::wstring();
			}

			std::vector<std::wstring> splitdir(const std::wstring& input, const std::wstring& regex) {
				// passing -1 as the submatch index parameter performs splitting
				std::wregex re(regex);
				std::wsregex_token_iterator
					first{ input.begin(), input.end(), re, -1 },
					last;
				return{ first, last };
			}

			wstring resolveSingleDir(const wstring& input)
			{
				// not implemented
				assert(false);
				return std::wstring();
			}
		}
//		std::wstring stdGetFullPathName(const std::wstring& ws)
//		{
//			return stdGetFullPathName(ws.c_str());
//		}
		std::wstring resolveLink(const std::wstring& instring)
		{
			// not implemented
			assert(false);
			return std::wstring();
		}

		template<typename C>
		std::basic_string<C> stdGetComputerName()
		{
            unique_ptr<char[]> p;
            size_t size = 16;
            while (true)
            {
                if (size > 4096)
                    return std::basic_string<char>();
                p.reset(new char[size]);
                int ret = gethostname(p.get(), size);
                switch (ret)
                {
                case 0:
                    return p.get();
                case -1:  // if buffer is not sufficient -1 is returned
                case ENAMETOOLONG:
                    size *= 2;
                    continue;
                default:
                    return std::basic_string<char>();
                }
                assert(false);
                break;
            }
            assert(false);
            return std::basic_string<char>();

		}
        template std::basic_string<char> stdGetComputerName();
//      template std::basic_string<wchar_t> stdGetComputerName();


        template<typename C>
        inline std::basic_string<C> stdGetCurrentDirectory()
        {
            unique_ptr<C[]> p;
            size_t size = 64;
            for (;;)
            {
                if(size > 4096)
                    return std::basic_string<C>();
                p.reset(new C[size * sizeof(C)]);
                const C* pRet = getcwd(p.get(), size);
                if(pRet==p.get())
                    break;
                if(pRet==nullptr && errno==ERANGE)
                    size *= 2;
                else
                    return std::basic_string<C>();
            }

            return p.get();
        }
        template std::basic_string<char> stdGetCurrentDirectory<char>();
        // template std::basic_string<wchar_t> stdGetCurrentDirectory<wchar_t>();

		wstring StdGetDesktopDirectory()
		{
			return L"";
		}

        size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, char* pBuf, size_t len)
        {
            // https://stackoverflow.com/a/198099
            size_t bytesWritten = (size_t)readlink("/proc/self/exe", pBuf, len);
            if((size_t)-1 == bytesWritten)
                return 0;
            pBuf[bytesWritten] = '\0';
            return bytesWritten;
        }

        HFILEITERATOR stdCreateFileIterator(const std::string& directory)
        {
            return (HFILEITERATOR)opendir(directory.c_str());
        }
        bool stdFileNextImpl(HFILEITERATOR hFileIterator, FileInfo<char>* fi)
        {
            if(hFileIterator==nullptr)
                return false;
            DIR* dir = (DIR*)hFileIterator;
            unique_ptr<struct dirent64> dirEnt(new struct dirent64);
            struct dirent64* pResult = nullptr;
            if(0 != readdir64_r(dir, dirEnt.get(), &pResult) || pResult==nullptr)
            {
                return false;
            }

            const bool isDir = (pResult->d_type & DT_DIR) != 0;
            off_t size=0;
            if(!isDir)
            {
                struct stat64 st;
                st.st_size = 0;
                stat64(pResult->d_name, &st);
                size = st.st_size;
            }

            fi->setAll(isDir,
                       pResult->d_name,
                       size);
            return true;
        }
        bool stdCloseFileIterator(HFILEITERATOR hFileIterator)
        {
            return 0==closedir((DIR*)hFileIterator);
        }
	}
}
