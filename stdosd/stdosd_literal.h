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

#ifndef STDOSD_LITERAL_INCLUDED
#define STDOSD_LITERAL_INCLUDED

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

// undef these at the end of this file
#if defined(_MSC_VER) || defined(__MINGW32__)
	#define STDOSD_DEFAULTSEPARATORSTRING "\\"
	#define STDOSD_PATHSEPARATORSTRINGS "/\\"
	#define STDOSD_NEWLINE "\r\n"
	#define STDOSD_ENVPATHSEPARATOR ';'
	#define STDOSD_ENVPATHSEPARATORSTRING ";"
	#define STDOSD_HOME_ENVKEY "HOMEPATH"
#else
	#define STDOSD_DEFAULTSEPARATORSTRING "/"
	#define STDOSD_PATHSEPARATORSTRINGS "/"
	#define STDOSD_NEWLINE "\n"
	#define STDOSD_ENVPATHSEPARATOR ':'
	#define STDOSD_ENVPATHSEPARATORSTRING ":"
	#define STDOSD_HOME_ENVKEY "home"
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
			STDOSD_DEFINE_CHAR(Na,'a');										\
			STDOSD_DEFINE_CHAR(Nb,'b');										\
			STDOSD_DEFINE_CHAR(Nc,'c');										\
			STDOSD_DEFINE_CHAR(Nd,'d');										\
			STDOSD_DEFINE_CHAR(Ne,'e');										\
			STDOSD_DEFINE_CHAR(Nf,'f');										\
			STDOSD_DEFINE_CHAR(Ng,'g');										\
			STDOSD_DEFINE_CHAR(Nh,'h');										\
			STDOSD_DEFINE_CHAR(Ni,'i');										\
			STDOSD_DEFINE_CHAR(Nj,'j');										\
			STDOSD_DEFINE_CHAR(Nk,'k');										\
			STDOSD_DEFINE_CHAR(Nl,'l');										\
			STDOSD_DEFINE_CHAR(Nm,'m');										\
			STDOSD_DEFINE_CHAR(Nn,'n');										\
			STDOSD_DEFINE_CHAR(No,'o');										\
			STDOSD_DEFINE_CHAR(Np,'p');										\
			STDOSD_DEFINE_CHAR(Nq,'q');										\
			STDOSD_DEFINE_CHAR(Nr,'r');										\
			STDOSD_DEFINE_CHAR(Ns,'s');										\
			STDOSD_DEFINE_CHAR(Nt,'t');										\
			STDOSD_DEFINE_CHAR(Nu,'u');										\
			STDOSD_DEFINE_CHAR(Nv,'v');										\
			STDOSD_DEFINE_CHAR(Nw,'w');										\
			STDOSD_DEFINE_CHAR(Nx,'x');										\
			STDOSD_DEFINE_CHAR(Ny,'y');										\
			STDOSD_DEFINE_CHAR(Nz,'z');										\
			STDOSD_DEFINE_CHAR(NA,'A');										\
			STDOSD_DEFINE_CHAR(NB,'B');										\
			STDOSD_DEFINE_CHAR(NC,'C');										\
			STDOSD_DEFINE_CHAR(ND,'D');										\
			STDOSD_DEFINE_CHAR(NE,'E');										\
			STDOSD_DEFINE_CHAR(NF,'F');										\
			STDOSD_DEFINE_CHAR(NG,'G');										\
			STDOSD_DEFINE_CHAR(NH,'H');										\
			STDOSD_DEFINE_CHAR(NI,'I');										\
			STDOSD_DEFINE_CHAR(NJ,'J');										\
			STDOSD_DEFINE_CHAR(NK,'K');										\
			STDOSD_DEFINE_CHAR(NL,'L');										\
			STDOSD_DEFINE_CHAR(NM,'M');										\
			STDOSD_DEFINE_CHAR(NN,'N');										\
			STDOSD_DEFINE_CHAR(NO,'O');										\
			STDOSD_DEFINE_CHAR(NP,'P');										\
			STDOSD_DEFINE_CHAR(NQ,'Q');										\
			STDOSD_DEFINE_CHAR(NR,'R');										\
			STDOSD_DEFINE_CHAR(NS,'S');										\
			STDOSD_DEFINE_CHAR(NT,'T');										\
			STDOSD_DEFINE_CHAR(NU,'U');										\
			STDOSD_DEFINE_CHAR(NV,'V');										\
			STDOSD_DEFINE_CHAR(NW,'W');										\
			STDOSD_DEFINE_CHAR(NX,'X');										\
			STDOSD_DEFINE_CHAR(NY,'Y');										\
			STDOSD_DEFINE_CHAR(NZ,'Z');										\
																			\
			STDOSD_DEFINE_CHAR(NPlus			,'+');						\
			STDOSD_DEFINE_CHAR(NMinus			,'-');						\
			STDOSD_DEFINE_CHAR(NAsterisk		,'*');						\
			STDOSD_DEFINE_CHAR(NSlash			,'/');						\
			STDOSD_DEFINE_CHAR(NBackSlash		,'\\');						\
			STDOSD_DEFINE_CHAR(NColon			,':');						\
			STDOSD_DEFINE_CHAR(NSemiColon		,';');						\
			STDOSD_DEFINE_CHAR(NSpace			,' ');						\
			STDOSD_DEFINE_CHAR(NDoubleQuote		,'"');						\
			STDOSD_DEFINE_CHAR(NAmpasand		,'&');						\
			STDOSD_DEFINE_CHAR(NSingleQuote		,'\'');						\
			STDOSD_DEFINE_CHAR(NLess			,'<');						\
			STDOSD_DEFINE_CHAR(NGreater			,'>');						\
																			\
			STDOSD_DEFINE_CHAR(NCarriageReturn	,'\r');						\
			STDOSD_DEFINE_CHAR(NNewLine			,'\n');						\
																			\
			STDOSD_DEFINE_CHAR(NSquareBlacketBegin	,'[');					\
			STDOSD_DEFINE_CHAR(NSquareBlacketEnd	,']');					\
																			\
			STDOSD_DEFINE_CHAR(NVerticalBar,		'|');					\
																			\
			STDOSD_DEFINE_CHAR(NDefaultEnvPathSeparator, STDOSD_ENVPATHSEPARATOR);					\
																			\
			STDOSD_DEFINE_STRINGFUNCTION(defaultPathSeparatorString, STDOSD_DEFAULTSEPARATORSTRING);	\
            STDOSD_DEFINE_STRINGFUNCTION(defaultPathSeparatorStrings, STDOSD_PATHSEPARATORSTRINGS);		\
            STDOSD_DEFINE_STRINGFUNCTION(defaultEnvPathSeparatorString, STDOSD_ENVPATHSEPARATORSTRING);	\
            STDOSD_DEFINE_STRINGFUNCTION(nulString, "");								\
			STDOSD_DEFINE_STRINGFUNCTION(spaceString, " ");								\
			STDOSD_DEFINE_STRINGFUNCTION(semicolonString, ";");							\
			STDOSD_DEFINE_STRINGFUNCTION(dotString, ".");								\
			STDOSD_DEFINE_STRINGFUNCTION(dotdotString, "..");							\
			STDOSD_DEFINE_STRINGFUNCTION(num0String, "0");								\
			STDOSD_DEFINE_STRINGFUNCTION(num1String, "1");								\
			STDOSD_DEFINE_STRINGFUNCTION(noString, "no");								\
			STDOSD_DEFINE_STRINGFUNCTION(yesString, "yes");								\
			STDOSD_DEFINE_STRINGFUNCTION(offString, "off");								\
			STDOSD_DEFINE_STRINGFUNCTION(onString, "on");								\
			STDOSD_DEFINE_STRINGFUNCTION(falseString, "false");							\
			STDOSD_DEFINE_STRINGFUNCTION(trueString, "true");							\
			STDOSD_DEFINE_STRINGFUNCTION(CR,"\r");										\
			STDOSD_DEFINE_STRINGFUNCTION(LF,"\n");										\
			STDOSD_DEFINE_STRINGFUNCTION(CRLF,"\r\n");									\
			STDOSD_DEFINE_STRINGFUNCTION(WHITESPACE," \t\r\n");							\
			STDOSD_DEFINE_STRINGFUNCTION(XMLAMP,"&amp;");								\
			STDOSD_DEFINE_STRINGFUNCTION(XMLAPOS,"&apos;");								\
			STDOSD_DEFINE_STRINGFUNCTION(XMLQUOTE,"&quot;");							\
			STDOSD_DEFINE_STRINGFUNCTION(XMLLESS,"&lt;");								\
			STDOSD_DEFINE_STRINGFUNCTION(XMLGREATER,"&gt;");							\
			STDOSD_DEFINE_STRINGFUNCTION(JOINSEPARATOR,", ");							\
			STDOSD_DEFINE_STRINGFUNCTION(JOINSTARTPAREN,"'");							\
			STDOSD_DEFINE_STRINGFUNCTION(JOINENDPAREN,"'");								\
			STDOSD_DEFINE_STRINGFUNCTION(PATH,"PATH");									\
			STDOSD_DEFINE_STRINGFUNCTION(program,"program");							\
			STDOSD_DEFINE_STRINGFUNCTION(commandlineseparator,"  ");					\
			STDOSD_DEFINE_STRINGFUNCTION(commandlineonearg, "Arg");						\
			STDOSD_DEFINE_STRINGFUNCTION(commandlinezerooronearg, "[Arg]");				\
			STDOSD_DEFINE_STRINGFUNCTION(commandlinetwoargs, "Arg1 Arg2");				\
			STDOSD_DEFINE_STRINGFUNCTION(commandlinezeroortwoargs, "[Arg1 Arg2]");		\
			STDOSD_DEFINE_STRINGFUNCTION(commandlineoneortwoargs, "Arg1 [Arg2]");		\
			STDOSD_DEFINE_STRINGFUNCTION(commandlinezerooroneortwoargs, "[Arg1 [Arg2 [Arg3]]]");\
			STDOSD_DEFINE_STRINGFUNCTION(commandlinezerotoinfiniteargs, "[Arg1 [Arg2 [...]]]");\
			STDOSD_DEFINE_STRINGFUNCTION(commandlineonetoinfiniteargs, "Arg1 [Arg2 [...]]");\
			STDOSD_DEFINE_STRINGFUNCTION(commandlinetwotoinfiniteargs, "Arg1 Arg2 [Arg3 [...]]");\
			STDOSD_DEFINE_STRINGFUNCTION(Arg,"Arg");									\
			STDOSD_DEFINE_STRINGFUNCTION(OptionPrefix,"-/");							\
			STDOSD_DEFINE_STRINGFUNCTION(FileModeRead,"r");								\
			STDOSD_DEFINE_STRINGFUNCTION(FileModeReadByte,"rb");						\
			STDOSD_DEFINE_STRINGFUNCTION(FileModeWrite,"w");							\
			STDOSD_DEFINE_STRINGFUNCTION(FileModeWriteByte,"wb");						\
			STDOSD_DEFINE_STRINGFUNCTION(VideoFileExtensions,"3g2;3gp;amv;asf;avi;avs;divx;drc;f4a;f4b;f4p;f4v;flv;m2v;m4p;m4v;mkv;mng;mov;mp2;mp4;mpe;mpeg;mpg;mpv;mxf;nsv;ogm;ogv;qt;rm;rmvb;roq;svi;swf;vob;webm;wmv;yuv");						\
			STDOSD_DEFINE_STRINGFUNCTION(HomeEnvKey,STDOSD_HOME_ENVKEY);						\
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

#undef STDOSD_DEFAULTSEPARATORSTRING
#undef STDOSD_PATHSEPARATORSTRINGS
#undef STDOSD_NEWLINE
#undef STDOSD_ENVPATHSEPARATOR
#undef STDOSD_ENVPATHSEPARATORSTRING
#undef STDOSD_HOME_ENVKEY
	}
}

#endif // #ifndef STDOSD_LITERAL_INCLUDED
