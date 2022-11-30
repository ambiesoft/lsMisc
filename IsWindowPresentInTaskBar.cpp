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


#include <windows.h>
#include "IsWindowPresentInTaskBar.h"

namespace Ambiesoft {
	bool IsWindowPresentInTaskBar(HWND hWnd)
	{
		if (!IsWindow(hWnd))
			return false;

		const LONG Style = GetWindowLong(hWnd, GWL_STYLE);
		const LONG ExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

		if (ExStyle & WS_EX_APPWINDOW)
			return true;

		if ((Style & WS_VISIBLE) == 0)
			return false;
		if (ExStyle & WS_EX_TOOLWINDOW)
			return false;
		if (Style & WS_CHILD)
			return false;
		if (Style & WS_POPUP)
			return false;
		if (ExStyle & WS_EX_CLIENTEDGE)
			return false;
		if (ExStyle & WS_EX_DLGMODALFRAME)
			return false;

		if (GetParent(hWnd) != nullptr)
			return false;
		if (GetWindow(hWnd, GW_OWNER) != nullptr)
			return false;

		if (Style & WS_OVERLAPPEDWINDOW)
			return true;
		if (ExStyle & WS_EX_OVERLAPPEDWINDOW)
			return true;

		return false;
	}
}