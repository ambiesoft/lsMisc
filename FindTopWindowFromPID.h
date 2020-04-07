
#pragma once
#include <Windows.h>

namespace Ambiesoft {
	// https://stackoverflow.com/a/39290139
	HWND FindTopWindowFromPID(DWORD pid);
}