
#pragma once
#include <Windows.h>
#include <set>
namespace Ambiesoft {
	// https://stackoverflow.com/a/39290139
	std::set<HWND> FindTopWindowFromPID(DWORD pid);
}