//BSD 2-Clause License
//
//Copyright (c) 2018, Ambiesoft
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//* Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//* Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE

#include <Windows.h>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <string>
#include <memory>

#ifdef _DEBUG
#include <cassert>
#endif

#include "UTF16toUTF8.h"
#include "UrlEncode.h"

using namespace std;


namespace Ambiesoft {

	//static bool myiswalnum(wchar_t c)
	//{
	//	switch (c) {
	//	case L'0': return true;
	//	case L'1': return true;
	//	case L'2': return true;
	//	case L'3': return true;
	//	case L'4': return true;
	//	case L'5': return true;
	//	case L'6': return true;
	//	case L'7': return true;
	//	case L'8': return true;
	//	case L'9': return true;
	//	case L'a':case L'A': return true;
	//	case L'b':case L'B': return true;
	//	case L'c':case L'C': return true;
	//	case L'd':case L'D': return true;
	//	case L'e':case L'E': return true;
	//	case L'f':case L'F': return true;
	//	case L'g':case L'G': return true;
	//	case L'h':case L'H': return true;
	//	case L'i':case L'I': return true;
	//	case L'j':case L'J': return true;
	//	case L'k':case L'K': return true;
	//	case L'l':case L'L': return true;
	//	case L'm':case L'M': return true;
	//	case L'n':case L'N': return true;
	//	case L'o':case L'O': return true;
	//	case L'p':case L'P': return true;
	//	case L'q':case L'Q': return true;
	//	case L'r':case L'R': return true;
	//	case L's':case L'S': return true;
	//	case L't':case L'T': return true;
	//	case L'u':case L'U': return true;
	//	case L'v':case L'V': return true;
	//	case L'w':case L'W': return true;
	//	case L'x':case L'X': return true;
	//	case L'y':case L'Y': return true;
	//	case L'z':case L'Z': return true;
	//	}
	//	return false;
	//}


	namespace {
		// http://www.emoticode.net/c/urlencode-in-plain-c.html
		char i2aA(char code) {
			static char hex[] = "0123456789ABCDEF";
			return hex[code & 15];
		}
		char a2ibyte(char c)
		{
			switch (c) {
			case '0': return 0x0;
			case '1': return 0x1;
			case '2': return 0x2;
			case '3': return 0x3;
			case '4': return 0x4;
			case '5': return 0x5;
			case '6': return 0x6;
			case '7': return 0x7;
			case '8': return 0x8;
			case '9': return 0x9;
			case 'a':case 'A': return 0xa;
			case 'b':case 'B': return 0xb;
			case 'c':case 'C': return 0xc;
			case 'd':case 'D': return 0xd;
			case 'e':case 'E': return 0xe;
			case 'f':case 'F': return 0xf;
			}
			return 0x10;
		}
#ifdef _DEBUG
		void checkmoji()
		{
			assert('1' == L'1');
			assert('1' != L'2');
			assert('a' == L'a');
		}
#endif
	}


	////////////// Encoding ////////////////////
	char *UrlEncodeEx(const char *pstr, int size)
	{
		char
			*buf,
			*pbuf;

		if (pstr == NULL)
			return NULL;

		if (size == -1)
			size = (int)strlen(pstr);

		if (size == 0)
		{
			// return (char*)calloc(1, 1);
			char* p = new char[1];
			p[0] = 0;
			return p;
		}

		pbuf = buf = new char[size * 3 + 1];

		while (size--) {
			if (((*pstr & 0x80) == 0 && isalnum(*pstr)) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
				*pbuf++ = *pstr;
			}
			else if (*pstr == ' ') {
				*pbuf++ = '+';
			}
			else {
				*pbuf++ = '%';
				*pbuf++ = i2aA((*pstr & 0xFF) >> 4);
				*pbuf++ = i2aA(*pstr & 0xF);
			}
			pstr++;
		}
		*pbuf = '\0';

		return buf;
	}





	////////////// Decoding ////////////////////
	char* UrlDecodeEx(const char* penc, int* psize)
	{
		unsigned int size = 0;
		unsigned char* pOrig = new unsigned char[strlen(penc) + 1];
		unsigned char* p = pOrig;
		for (; *penc; ++penc, ++p, ++size)
		{
			char c = *penc;
			if (c == '%')
			{
				const char* pSave = penc;
				penc++;
				if (!*penc)
				{
					break;
				}
				char c1, c2;
				c1 = *penc;
				penc++;
				if (!*penc)
				{
					*p = c1;
					break;
				}
				c2 = *penc;

				char c1con = a2ibyte(c1);
				char c2con = a2ibyte(c2);
				if (c1con == 0x10 || c2con == 0x10)
				{
					//failed conversion
					penc = pSave;
					*p = *penc;
					continue;
				}

				c = ((c1con << 4) | c2con);
			}
			else if (c == '+')
			{
				c = ' ';
			}
			*p = c;
		}
		*p = 0;
		if (psize)
			*psize = size;
		return (char*)pOrig;
	}









} // namespace