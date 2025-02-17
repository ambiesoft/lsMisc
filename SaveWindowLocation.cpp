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

#include <Windows.h>

#include "DebugNew.h"
using namespace std;

#include "../profile/cpp/Profile/include/ambiesoft.profile.h"
#include "SaveWindowLocation.h"

namespace Ambiesoft {
	namespace {
		constexpr const char* APP_LOCATION = "Location";
		constexpr const char* LOCATION_X = "X";
		constexpr const char* LOCATION_Y = "Y";
		constexpr const char* LOCATION_WIDTH = "Width";
		constexpr const char* LOCATION_HEIGHT = "Height";
	}
	bool LoadWindowLocation(HWND hWnd, HashIniHandle ini)
	{
		if (!IsWindow(hWnd))
			return false;

		if (!ini)
			return false;

		int x, y, width, height;
		bool read = true;
		read &= Profile::GetInt(APP_LOCATION, LOCATION_X, 0, x, ini);
		read &= Profile::GetInt(APP_LOCATION, LOCATION_Y, 0, y, ini);
		read &= Profile::GetInt(APP_LOCATION, LOCATION_WIDTH, 640, width, ini);
		read &= Profile::GetInt(APP_LOCATION, LOCATION_HEIGHT, 480, height, ini);
		if (!read)
			return false;
		return !!SetWindowPos(hWnd, nullptr, x, y, width, height, SWP_NOZORDER);
	}
	bool LoadWindowLocation(HWND hWnd, LPCTSTR pIni)
	{
		if (!IsWindow(hWnd))
			return false;

		Profile::CHashIni ini(Profile::ReadAll(wstring(pIni), false));
		if (!ini)
			return false;

		return LoadWindowLocation(hWnd, ini);
	}

	bool SaveWindowLocation(HWND hWnd, HashIniHandle ini)
	{
		if (!IsWindow(hWnd))
			return false;
		if (IsIconic(hWnd) || IsZoomed(hWnd))
			return true;
		RECT rect;
		if (!GetWindowRect(hWnd, &rect))
			return false;

		bool ok = true;
		ok &= Profile::WriteInt(APP_LOCATION, LOCATION_X, rect.left, ini);
		ok &= Profile::WriteInt(APP_LOCATION, LOCATION_Y, rect.top, ini);
		ok &= Profile::WriteInt(APP_LOCATION, LOCATION_WIDTH, rect.right - rect.left, ini);
		ok &= Profile::WriteInt(APP_LOCATION, LOCATION_HEIGHT, rect.bottom - rect.top, ini);

		return ok;
	}
	bool SaveWindowLocation(HWND hWnd, LPCTSTR pIni)
	{
		Profile::CHashIni ini(Profile::ReadAll(wstring(pIni), false));
		if (!ini)
			return false;

		if (!SaveWindowLocation(hWnd, ini))
			return false;

		return Profile::WriteAll(ini, wstring(pIni));
	}
}