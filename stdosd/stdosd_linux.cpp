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
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/inotify.h>
#include <fcntl.h>

#include <regex>
#include <cassert>

#include "stdosd.h"

namespace Ambiesoft {
namespace stdosd {
using namespace std;

namespace {
std::wstring stdGetFullPathName(const wchar_t*)
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

wstring resolveSingleDir(const wstring&)
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
    STDOSD_UNUSED(instring);
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

#ifdef _WIN32
size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, char* pBuf, size_t len)
{
    // https://stackoverflow.com/a/198099
    size_t bytesWritten = (size_t)readlink("/proc/self/exe", pBuf, len);
    if((size_t)-1 == bytesWritten)
        return 0;
    pBuf[bytesWritten] = '\0';
    return bytesWritten;
}
#endif

class CFileIteratorInternal
{
    DIR* dir_ = nullptr;
    FILEITERATEMODE itmode_;
public:
    CFileIteratorInternal(DIR* dir, FILEITERATEMODE itmode) :
        dir_(dir), itmode_(itmode){}
    ~CFileIteratorInternal() {
        Close();
    }
    DIR* dir() const {
        return dir_;
    }
    bool Close() {
        if(!dir_)
            return false;
        bool ret = closedir(dir_)==0;
        dir_=nullptr;
        return ret;
    }
    bool IsIterateDot() const {
        return (itmode_ & FILEITERATEMODE::SKIP_DOT)==0;
    }
    bool IsIterateDotDot() const {
        return (itmode_ & FILEITERATEMODE::SKIP_DOTDOT)==0;
    }
};

HFILEITERATOR stdCreateFileIterator(
    const std::string& directory,
    FILEITERATEMODE fim,
    int depth)
{
    STDOSD_UNUSED(depth);

    DIR* dir=opendir(directory.c_str());
    if(!dir)
        return nullptr;
    return (HFILEITERATOR)(new CFileIteratorInternal(dir, fim));
}




namespace detail {
bool IsDot(const char* pName)
{
    if(!pName || !pName[0])
        return false;
    return pName[0]=='.' && pName[1]==0;
}
bool IsDotDot(const char* pName)
{
    if(!pName || !pName[0] || !pName[1])
        return false;
    return pName[0]=='.' && pName[1]=='.' && pName[2]==0;
}
bool stdFileNextImpl(HFILEITERATOR hFileIterator, FileDirectoryInfo<char>* fi)
{
    if(hFileIterator==nullptr)
        return false;
    CFileIteratorInternal* fit = (CFileIteratorInternal*)hFileIterator;
    unique_ptr<struct dirent64> dirEnt(new struct dirent64);
    struct dirent64* pResult = nullptr;
    if(0 != readdir64_r(fit->dir(), dirEnt.get(), &pResult) || pResult==nullptr)
    {
        return false;
    }

    const bool isDir = (pResult->d_type & DT_DIR) != 0;
    off_t size=0;
    if(isDir)
    {
        if(!fit->IsIterateDot() && IsDot(pResult->d_name))
            return stdFileNextImpl(hFileIterator, fi);
        if(!fit->IsIterateDotDot() && IsDotDot(pResult->d_name))
            return stdFileNextImpl(hFileIterator, fi);
    }
    else
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
}
bool stdCloseFileIterator(HFILEITERATOR hFileIterator)
{
    CFileIteratorInternal* fit = (CFileIteratorInternal*)hFileIterator;
    if(!fit)
        return false;
    bool ret = fit->Close();
    delete fit;
    return ret;
}

namespace detail {
size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, char* p, size_t size)
{
    assert(hInst==nullptr);
    size_t ret = readlink("/proc/self/exe", p, size) ;
    if(ret < size)
    {
        p[ret]=0;
        ++ret;
    }
    return ret;
}

size_t stdExpandEnvironmentStringsImpl(const char* pIN, char* p, size_t size)
{
    assert(false);
    return 0;
}
}

std::basic_string<SYSTEM_CHAR_TYPE> stdGetLocalAppDirectory(
    const std::basic_string<SYSTEM_CHAR_TYPE>& organization,
    const std::basic_string<SYSTEM_CHAR_TYPE>& appName,
    bool bCreate)
{
    using S = std::basic_string<SYSTEM_CHAR_TYPE>;
    S ret = stdGetHomeDirectory();
    if (ret.empty())
        return S();
    if (!stdDirectoryExists(ret))
        return S();

    ret = stdCombinePath(ret, ".local/share");
    if (!stdDirectoryExists(ret))
        return S();

    ret = stdCombinePath(ret, organization);
    if (bCreate)
    {
        stdMkDir(ret);
        if (!stdDirectoryExists(ret))
            return S();
    }
    ret = stdCombinePath(ret, appName);
    if (bCreate)
    {
        stdMkDir(ret);
        if (!stdDirectoryExists(ret))
            return S();
    }
    return ret;
}

std::basic_string<SYSTEM_CHAR_TYPE> stdGetHomeDirectory()
{
    return stdGetenv(stdLiterals<SYSTEM_CHAR_TYPE>::HomeEnvKey());
}

std::basic_string<SYSTEM_CHAR_TYPE> stdGetFullPathName(const SYSTEM_CHAR_TYPE* p)
{
    char resolvedPath[PATH_MAX]; // PATH_MAX is a constant for maximum path length

    if (realpath(p, resolvedPath) != NULL)
        return resolvedPath;
    return std::string();
}
std::basic_string<SYSTEM_CHAR_TYPE> resolveLink(const std::basic_string<SYSTEM_CHAR_TYPE>& fullstring)
{
    char resolvedPath[PATH_MAX];

    if (realpath(fullstring.c_str(), resolvedPath) != nullptr)
        return resolvedPath;
    return std::string();
}
STDOSD_PID stdGetCurrentProcessId()
{
    return getpid();
}
std::vector<STDOSD_PID> stdGetAllProcesses(const SYSTEM_CHAR_TYPE* pExecutable)
{
    DIR* dir;
    struct dirent* ent;
    char path[PATH_MAX];
    std::vector<STDOSD_PID> ret;
    const bool bIsFullPath = pExecutable ? stdIsFullPath(pExecutable) : false;
    if ((dir = opendir("/proc")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            sprintf(path, "%s\n", ent->d_name);
            if (stdIsDigitString(ent->d_name)) {
                if(pExecutable==nullptr || *pExecutable==0) {
                    ret.push_back(stoi(ent->d_name));
                } else {
                    snprintf(path, sizeof(path), "/proc/%s/cmdline", ent->d_name);
                    FILE* cmdline = fopen(path, "r");
                    if (cmdline) {
                        char buffer[PATH_MAX];
                        size_t bytesRead = fread(buffer, 1, sizeof(buffer), cmdline);
                        fclose(cmdline);
                        if (bytesRead > 0) {
                            if(bIsFullPath) {
                                if(strcmp(pExecutable, buffer)==0) {
                                    ret.push_back(stoi(ent->d_name));
                                }
                            } else {
                                string name = stdGetFileName(buffer);
                                if(name==pExecutable) {
                                    ret.push_back(stoi(ent->d_name));
                                }
                            }
                        }
                    }
                }
            }
        }
        closedir(dir);
    }
    return ret;
}
bool stdSuspendProcess(STDOSD_PID pid)
{
    return 0==kill(pid, SIGSTOP);
}
bool stdResumeProcess(STDOSD_PID pid)
{
    return 0==kill(pid, SIGCONT);
}

// https://stackoverflow.com/a/54765359
// return -1 on error, or 0 if everything went well
static int wait_for_pid(int pid)
{
    char path[32];
    int in_fd = inotify_init();
    sprintf(path, "/proc/%i/exe", pid);
    if (inotify_add_watch(in_fd, path, IN_CLOSE_NOWRITE) < 0) {
        close(in_fd);
        return -1;
    }
    sprintf(path, "/proc/%i", pid);
    int dir_fd = open(path, 0);
    if (dir_fd < 0) {
        close(in_fd);
        return -1;
    }

    int res = 0;
    while (1) {
        struct inotify_event event;
        if (read(in_fd, &event, sizeof(event)) < 0) {
            res = -1;
            break;
        }
        int f = openat(dir_fd, "fd", 0);
        if (f < 0) break;
        close(f);
    }

    close(dir_fd);
    close(in_fd);
    return res;
}

bool stdWaitProcess(STDOSD_PID pid)
{
    return -1 != wait_for_pid(pid);
}

}  // namespace stdosd
}  // namespace Ambiesoft
