
#pragma once
#include <Windows.h>
#include <string>

namespace Ambiesoft {
	std::wstring GetFullFileNameFromPID(DWORD pid, DWORD* pLastError = nullptr);
}