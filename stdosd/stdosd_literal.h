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

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <wctype.h>

namespace Ambiesoft {
	namespace stdosd {


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

#endif // _WIN32 __GNUC__



#if defined(_MSC_VER) || defined(__MINGW32__)
#define STDOSD_DEFAULTSEPARATOR "\\"
#define STDOSD_PATHSEPARATORS "/\\"
#define STDOSD_NEWLINE "\r\n"
#else
#define STDOSD_DEFAULTSEPARATOR "/"
#define STDOSD_PATHSEPARATORS "/"
#define STDOSD_NEWLINE "\n"
#endif

        template<typename C>
		struct stdLiterals;


#define STRUCT_STDLITERALS(CHTYPE) template<>								\
		struct stdLiterals<CHTYPE>											\
		{																	\
			STDOSD_DEFINE_CHAR(period, '.');								\
			STDOSD_DEFINE_CHAR(N0,'0');										\
			STDOSD_DEFINE_CHAR(N1,'1');										\
			STDOSD_DEFINE_CHAR(N2,'2');										\
			STDOSD_DEFINE_CHAR(N3,'3');										\
			STDOSD_DEFINE_CHAR(N4,'4');										\
			STDOSD_DEFINE_CHAR(N5,'5');										\
			STDOSD_DEFINE_CHAR(N6,'6');										\
			STDOSD_DEFINE_CHAR(N7,'7');										\
			STDOSD_DEFINE_CHAR(N8,'8');										\
			STDOSD_DEFINE_CHAR(N9,'9');										\
																			\
			STDOSD_DEFINE_CHAR(Nz,'z');										\
			STDOSD_DEFINE_CHAR(Na,'a');										\
			STDOSD_DEFINE_CHAR(NA,'A');										\
			STDOSD_DEFINE_CHAR(NZ,'Z');										\
																			\
			STDOSD_DEFINE_CHAR(NSlash			,'/');						\
			STDOSD_DEFINE_CHAR(NBackSlash		,'\\');						\
			STDOSD_DEFINE_CHAR(NColon			,':');						\
			STDOSD_DEFINE_CHAR(NSpace			,' ');						\
			STDOSD_DEFINE_CHAR(NDoubleQuote		,'"');						\
			STDOSD_DEFINE_CHAR(NAmpasand		,'&');						\
			STDOSD_DEFINE_CHAR(NSingleQuote		,'\'');						\
			STDOSD_DEFINE_CHAR(NLess			,'<');						\
			STDOSD_DEFINE_CHAR(NGreater			,'>');						\
																			\
			STDOSD_DEFINE_STRINGFUNCTION(defaultSeparator, STDOSD_DEFAULTSEPARATOR);	\
			STDOSD_DEFINE_STRINGFUNCTION(pathSeparators, STDOSD_PATHSEPARATORS);		\
			STDOSD_DEFINE_STRINGFUNCTION(nulString, "");								\
			STDOSD_DEFINE_STRINGFUNCTION(spaceString, " ");								\
			STDOSD_DEFINE_STRINGFUNCTION(num0String, "0");								\
			STDOSD_DEFINE_STRINGFUNCTION(offString, "off");								\
			STDOSD_DEFINE_STRINGFUNCTION(falseString, "false");							\
			STDOSD_DEFINE_STRINGFUNCTION(CR,"\r");										\
			STDOSD_DEFINE_STRINGFUNCTION(LF,"\n");										\
			STDOSD_DEFINE_STRINGFUNCTION(CRLF,"\r\n");									\
			STDOSD_DEFINE_STRINGFUNCTION(WHITESPACE," \t\r\n");							\
			STDOSD_DEFINE_STRINGFUNCTION(XMLAMP,"&amp;");								\
			STDOSD_DEFINE_STRINGFUNCTION(XMLAPOS,"&apos;");								\
			STDOSD_DEFINE_STRINGFUNCTION(XMLQUOTE,"&quot;");							\
			STDOSD_DEFINE_STRINGFUNCTION(XMLLESS,"&lt;");								\
			STDOSD_DEFINE_STRINGFUNCTION(XMLGREATER,"&gt;");							\
		}

#define STDOSD_DEFINE_CHAR(name, chara) static STDOSD_CONSTEXPR CHART name = LITEL(chara)
#define STDOSD_DEFINE_STRINGFUNCTION(funcname, ret) static STDOSD_CONSTEXPR CHART* funcname() {return LITEL(ret);}

#define LITEL(lit) lit
#define CHART char
		STRUCT_STDLITERALS(CHART);
#undef LITEL
#undef CHART

#define LITEL(lit) STDOSD_WCHARLITERAL(lit)
#define CHART wchar_t
		STRUCT_STDLITERALS(CHART);
#undef LITEL
#undef CHART

#if defined(CHAR16T_AVAILABLE)
#define LITEL(lit) STDOSD_CHAR16TLITERAL_INNER(lit)
#define CHART char16_t
		STRUCT_STDLITERALS(CHART);
#undef LITEL
#undef CHART
#endif 

#undef STDOSD_DEFINE_CHAR
#undef STDOSD_DEFINE_STRINGFUNCTION

		

	}
}
