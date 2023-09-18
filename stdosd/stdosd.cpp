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

#include <sstream>
#include <ostream>

#include "stdosd.h"

#ifndef _countof
#define _countof(A) (sizeof(A)/sizeof(A[0]))
#endif

namespace Ambiesoft {
	namespace stdosd {

		template<class T>
		void stdGetRandomString(T* s, const size_t len) {
            using NazoStdLiterals = typename ::Ambiesoft::stdosd::stdLiterals<T>;
			static const T alphanum[] = {
                NazoStdLiterals::N0,
                NazoStdLiterals::N1,
                NazoStdLiterals::N2,
                NazoStdLiterals::N3,
                NazoStdLiterals::N4,
                NazoStdLiterals::N5,
                NazoStdLiterals::N6,
                NazoStdLiterals::N7,
                NazoStdLiterals::N8,
                NazoStdLiterals::N9,
                NazoStdLiterals::NA,
                NazoStdLiterals::NB,
                NazoStdLiterals::NC,
                NazoStdLiterals::ND,
                NazoStdLiterals::NE,
                NazoStdLiterals::NF,
                NazoStdLiterals::NG,
                NazoStdLiterals::NH,
                NazoStdLiterals::NI,
                NazoStdLiterals::NJ,
                NazoStdLiterals::NK,
                NazoStdLiterals::NL,
                NazoStdLiterals::NM,
                NazoStdLiterals::NN,
                NazoStdLiterals::NO,
                NazoStdLiterals::NP,
                NazoStdLiterals::NQ,
                NazoStdLiterals::NR,
                NazoStdLiterals::NS,
                NazoStdLiterals::NT,
                NazoStdLiterals::NU,
                NazoStdLiterals::NV,
                NazoStdLiterals::NW,
                NazoStdLiterals::NX,
                NazoStdLiterals::NY,
                NazoStdLiterals::NZ,
                NazoStdLiterals::Na,
                NazoStdLiterals::Nb,
                NazoStdLiterals::Nc,
                NazoStdLiterals::Nd,
                NazoStdLiterals::Ne,
                NazoStdLiterals::Nf,
                NazoStdLiterals::Ng,
                NazoStdLiterals::Nh,
                NazoStdLiterals::Ni,
                NazoStdLiterals::Nj,
                NazoStdLiterals::Nk,
                NazoStdLiterals::Nl,
                NazoStdLiterals::Nm,
                NazoStdLiterals::Nn,
                NazoStdLiterals::No,
                NazoStdLiterals::Np,
                NazoStdLiterals::Nq,
                NazoStdLiterals::Nr,
                NazoStdLiterals::Ns,
                NazoStdLiterals::Nt,
                NazoStdLiterals::Nu,
                NazoStdLiterals::Nv,
                NazoStdLiterals::Nw,
                NazoStdLiterals::Nx,
                NazoStdLiterals::Ny,
                NazoStdLiterals::Nz,
			};

			// for debug
			//const char p[] = "0123456789"
			//	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			//	"abcdefghijklmnopqrstuvwxyz";
			//for (size_t i = 0; i < (sizeof(p) - 1); ++i) {
			//	assert(alphanum[i] == p[i]);
			//}

			if (len == 0)
				return;
			static bool initSeed = []() {
				std::srand((unsigned)std::time(nullptr));
				return true;
			}();
            STDOSD_UNUSED(initSeed);
			size_t lenMinusOne = len - 1;
			for (size_t i = 0; i < lenMinusOne; ++i) {
				s[i] = alphanum[std::rand() % (_countof(alphanum))];
			}
			s[lenMinusOne] = 0;
		}
		template void stdGetRandomString<char>(char* s, const size_t len);
#ifdef _WIN32
		template void stdGetRandomString<wchar_t>(wchar_t* s, const size_t len);
#endif

		template<typename C>
		inline std::basic_string<C> stdRegexReplace(
			const std::basic_string<C>& input,
			const std::basic_regex<C>& regex,
			std::function< std::basic_string<C>(const std::match_results<typename std::basic_string<C>::const_iterator>& match) > format)
		{
			using S = std::basic_string<C>;
			using OSS = typename std::basic_ostringstream<C, typename std::char_traits<C>, typename std::allocator<C> >;
			using SREGEX_ITERATOR = std::regex_iterator<typename S::const_iterator>;

			OSS output;
			SREGEX_ITERATOR begin(input.begin(), input.end(), regex), end;
			typename SREGEX_ITERATOR::difference_type lastPos = 0;

			for (; begin != end; begin++)
			{
				output << begin->prefix() << format(*begin);
				lastPos = begin->position() + begin->length();
			}
			output << input.substr(lastPos);
			return output.str();
		}
		template std::basic_string<char> stdRegexReplace<char>(
			const std::basic_string<char>& input,
			const std::basic_regex<char>& regex,
			std::function< std::basic_string<char>(const std::match_results<std::basic_string<char>::const_iterator>& match) > format);
		template std::basic_string<wchar_t> stdRegexReplace<wchar_t>(
			const std::basic_string<wchar_t>& input,
			const std::basic_regex<wchar_t>& regex,
			std::function< std::basic_string<wchar_t>(const std::match_results<std::basic_string<wchar_t>::const_iterator>& match) > format);


        std::basic_string<::Ambiesoft::stdosd::SYSTEM_CHAR_TYPE> stdGetProgramName()
		{
            return stdGetFileNameWithoutExtension(stdGetModuleFileName<Ambiesoft::stdosd::SYSTEM_CHAR_TYPE>());
		}

		template<typename C>
        bool stdGetUnittedSize(const C* pStr, size_t len, int* nSign, int64_t* lResult, int* pUnit)
		{
            using NazoStdLiterals = typename ::Ambiesoft::stdosd::stdLiterals<C>;
			if (pStr == NULL || *pStr == 0)
				return false;
            if (len == (size_t)-1)
				len = stdStringLength(pStr);
			if (len == 0)
				return false;

			C lastchar = pStr[len - 1];
			int unit = 1;
			bool unitted = true;
			switch (lastchar)
			{
            case NazoStdLiterals::Nk: unit = 1000; break;
            case NazoStdLiterals::Nm: unit = 1000 * 1000; break;
            case NazoStdLiterals::NM: unit = 1024 * 1024; break;
            case NazoStdLiterals::Ng: unit = 1000 * 1000 * 1000; break;
            case NazoStdLiterals::NK: unit = 1024; break;
            case NazoStdLiterals::NG: unit = 1024 * 1024 * 1024; break;
			default:
				unitted = false;
				break;
			}

			// need buffer, pStr may be system static
			std::basic_string<C> str(pStr);
			if (unitted)
				str = str.substr(0, len - 1);

			*nSign = 0;
			bool bsigned = true;
			switch (str[0])
			{
            case NazoStdLiterals::NPlus: *nSign = 1; break;
            case NazoStdLiterals::NMinus: *nSign = -1; break;
			default:
				bsigned = false;
			}
			if (bsigned)
				str = str.substr(1);

			if (pUnit)
				*pUnit = unit;

            int64_t r = stdFromString<int64_t, C>(str.c_str());

			*lResult = r * unit;
			if (*nSign < 0)
				*lResult = -*lResult;

			return true;
		}

        template bool stdGetUnittedSize(const char* pStr, size_t len, int* nSign, int64_t* lResult, int* pUnit);
#ifdef _WIN32
        template bool stdGetUnittedSize(const wchar_t* pStr, size_t len, int* nSign, int64_t* lResult, int* pUnit);
#endif

	}
}
