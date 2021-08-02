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
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define TOMESSAGE(S) #S ":" STRING(S)

#pragma message(TOMESSAGE(WINVER))
#pragma message(TOMESSAGE(_WIN32_WINNT))
#pragma message(TOMESSAGE(_WIN32_WINDOWS))
#pragma message(TOMESSAGE(_WIN32_IE))
#pragma message(TOMESSAGE(_MSC_VER))
#pragma message(TOMESSAGE(_MSC_FULL_VER))
#pragma message(TOMESSAGE(_MSC_BUILD))
#pragma message(TOMESSAGE(_MSVC_LANG))
#pragma message(TOMESSAGE(__CLR_VER ))
#pragma message(TOMESSAGE(_ATL_VER))
#pragma message(TOMESSAGE(_MFC_VER))
#pragma message(TOMESSAGE(__cplusplus))
#pragma message(TOMESSAGE(__cplusplus_cli))

#undef TOMESSAGE
#undef STRING
#undef STRING2