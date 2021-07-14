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
			static const T alphanum[] = {
				stdLiterals<T>::N0,
				stdLiterals<T>::N1,
				stdLiterals<T>::N2,
				stdLiterals<T>::N3,
				stdLiterals<T>::N4,
				stdLiterals<T>::N5,
				stdLiterals<T>::N6,
				stdLiterals<T>::N7,
				stdLiterals<T>::N8,
				stdLiterals<T>::N9,
				stdLiterals<T>::NA,
				stdLiterals<T>::NB,
				stdLiterals<T>::NC,
				stdLiterals<T>::ND,
				stdLiterals<T>::NE,
				stdLiterals<T>::NF,
				stdLiterals<T>::NG,
				stdLiterals<T>::NH,
				stdLiterals<T>::NI,
				stdLiterals<T>::NJ,
				stdLiterals<T>::NK,
				stdLiterals<T>::NL,
				stdLiterals<T>::NM,
				stdLiterals<T>::NN,
				stdLiterals<T>::NO,
				stdLiterals<T>::NP,
				stdLiterals<T>::NQ,
				stdLiterals<T>::NR,
				stdLiterals<T>::NS,
				stdLiterals<T>::NT,
				stdLiterals<T>::NU,
				stdLiterals<T>::NV,
				stdLiterals<T>::NW,
				stdLiterals<T>::NX,
				stdLiterals<T>::NY,
				stdLiterals<T>::NZ,
				stdLiterals<T>::Na,
				stdLiterals<T>::Nb,
				stdLiterals<T>::Nc,
				stdLiterals<T>::Nd,
				stdLiterals<T>::Ne,
				stdLiterals<T>::Nf,
				stdLiterals<T>::Ng,
				stdLiterals<T>::Nh,
				stdLiterals<T>::Ni,
				stdLiterals<T>::Nj,
				stdLiterals<T>::Nk,
				stdLiterals<T>::Nl,
				stdLiterals<T>::Nm,
				stdLiterals<T>::Nn,
				stdLiterals<T>::No,
				stdLiterals<T>::Np,
				stdLiterals<T>::Nq,
				stdLiterals<T>::Nr,
				stdLiterals<T>::Ns,
				stdLiterals<T>::Nt,
				stdLiterals<T>::Nu,
				stdLiterals<T>::Nv,
				stdLiterals<T>::Nw,
				stdLiterals<T>::Nx,
				stdLiterals<T>::Ny,
				stdLiterals<T>::Nz,
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
			size_t lenMinusOne = len - 1;
			for (size_t i = 0; i < lenMinusOne; ++i) {
				s[i] = alphanum[std::rand() % (_countof(alphanum))];
			}
			s[lenMinusOne] = 0;
		}
		template void stdGetRandomString<char>(char* s, const size_t len);
		template void stdGetRandomString<wchar_t>(wchar_t* s, const size_t len);

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

		std::basic_string<SYSTEM_CHAR_TYPE> stdGetProgramName()
		{
			return stdGetFileNameWitoutExtension(stdGetModuleFileName<SYSTEM_CHAR_TYPE>());
		}
	}
}
