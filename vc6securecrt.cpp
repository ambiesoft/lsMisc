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

#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE

#include "vc6securecrt.h"

#if _MSC_VER <= 1200

namespace Ambiesoft {
	errno_t memcpy_s(
	   void *dest,  
	   size_t destSize,  
	   const void *src,  
	   size_t count   
	)
	{
		int ret=0;
		if(destSize < count)
		{
			count=destSize;
			ret=ERANGE;
		}
		if(dest==NULL)
		{
			return EINVAL;
		}
		if(src==NULL)
		{
			memset(dest,0,count);
			return EINVAL;
		}
		memcpy(dest, src, count);
		return ret;
	}
	errno_t wmemcpy_s(  
	   wchar_t *dest,  
	   size_t destSize,  
	   const wchar_t *src,  
	   size_t count  
	)
	{
		return memcpy_s(dest, destSize*sizeof(wchar_t), src, count*sizeof(wchar_t));
	}

}
#endif  // _MSC_VER <= 1200