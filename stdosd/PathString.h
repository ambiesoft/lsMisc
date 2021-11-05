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

#pragma once


#include <string>

#ifdef _WIN32
    #include <Windows.h>
#endif
#if __GNUC__
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

#include "stdosd_literal.h"
#include "stdosd.h"

namespace Ambiesoft {
    namespace stdosd {

#ifdef _WIN32
        using SYSTEM_CHAR_TYPE = wchar_t;
#else
        using SYSTEM_CHAR_TYPE = char;
#endif
        using osdstring = std::basic_string<SYSTEM_CHAR_TYPE>;

        template<class C>
        class PathString
        {
            using S = std::basic_string < C, std::char_traits<C>, std::allocator<C>>;
            S path_;
        public:
            PathString() {}
            PathString(const S& path){
                S s(path);
                S t(path);
                for(;;) {
                    s = t;
                    t = stdStringReplace(t, stdLiterals<C>::NBackSlash, stdLiterals<C>::NSlash);
                    if (t == s)
                        break;
                } 

                S dd(1, stdLiterals<C>::NSlash);
                dd += stdLiterals<C>::NSlash;
                S ss(1, stdLiterals<C>::NSlash);
                for (;;) {
                    s = t;
                    t = stdStringReplace(t, dd, ss);
                    if (t == s)
                        break;
                }
                path_ = stdTrimEnd(s, stdLiterals<C>::NSlash);
#ifdef _WIN32
                for (size_t i = 0; i < path_.size(); ++i)
                    path_[i] = std::tolower(path_[i]);
#endif
            }
            PathString(const PathString& that) {
                path_ = that.path_;
            }
            bool Equals(const PathString& that) const {
                return this->path_ == that.path_;
            }

            bool cdUp() {
                typename S::size_type pos = path_.find_last_of(stdLiterals<C>::NSlash);
                if (pos == S::npos)
                    return false;
                path_ = path_.substr(0, pos);
                return true;
            }
        };

        template<class C>
        bool operator==(
            const PathString<C>& left, const PathString<C>& right) noexcept
        {
            return left.Equals(right);
        }
        template<class C>
        bool operator!=(
            const PathString<C>& left, const PathString<C>& right) noexcept
        {
            return !left.Equals(right);
        }

        template<class C>
        bool stdIsSamePathString(const C* c1, const C* c2) {
            if (c1 == c2)
                return true;
            if (!c1 && !c2)
                return true;
            if (!c1)
                if (c2[0] == 0)
                    return true;
            if (!c2)
                if (c1[0] == 0)
                    return true;
            return PathString<C>(c1) == PathString<C>(c2);
        }
        template<class C>
        bool stdIsSamePathString(const std::basic_string<C>& c1, const std::basic_string<C>& c2) {
            return PathString<C>(c1) == PathString<C>(c2);
        }
    }
}


#undef stdosd_max
#undef stdosd_min
