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

#include <vector>
#include <string>
#include <memory>
#include <regex>
#include <functional>
#include <set>

#include <cassert>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <wctype.h>

#ifdef _WIN32
    #include <Windows.h>
#endif
#if __GNUC__
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

#include "stdosd_literal.h"

#define STDOSD_WCHARLITERAL_INNER(x) L ## x
#define STDOSD_WCHARLITERAL(x) STDOSD_WCHARLITERAL_INNER(x)

#define STDOSD_CHAR16TLITERAL_INNER(x) u ## x
#define STDOSD_CHAR16TLITERAL(x) STDOSD_CHAR16TLITERAL_INNER(x)

#if __GNUC__

    #define STDOSD_CONSTEXPR const constexpr
    #define CHAR16T_AVAILABLE

#elif _WIN32 // not __GNUC__ but _WIN32

    #if _MSC_VER <= 1800  // less than or equal to VC2013 ( or VC12 )
    #define STDOSD_CONSTEXPR const
    #else
    #define STDOSD_CONSTEXPR const constexpr
    #endif

    #if _MSC_VER >= 1900
    #define CHAR16T_AVAILABLE
    #endif

	//#ifndef NOMINMAX
	//#error NOMINMAX must be defined
	//#endif

#endif // _WIN32 __GNUC__


#ifndef stdosd_max
#define stdosd_max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef stdosd_min
#define stdosd_min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
#define STDOSD_DEFAULTSEPARATOR "\\"
#define STDOSD_PATHSEPARATORS "/\\"
#define STDOSD_NEWLINE "\r\n"
#define STDOSD_SYSTEM_CHAR_LITERAL(s) STDOSD_WCHARLITERAL(s)
#define STDOSD_IS_CASESENSITIVE false
#else
#define STDOSD_DEFAULTSEPARATOR "/"
#define STDOSD_PATHSEPARATORS "/"
#define STDOSD_NEWLINE "\n"
#define STDOSD_SYSTEM_CHAR_LITERAL(s) s
#define STDOSD_IS_CASESENSITIVE true
#endif

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
            using S = std::basic_string<C>;
            S path_;
        public:
            PathString() {}
            PathString(const S& path):path_(path){}

            bool operator==(const S& that) {
                if (this->path_.empty() && that.path_.empty())
                    return true;
                return false;
            }
        };
	}
}


#undef stdosd_max
#undef stdosd_min
