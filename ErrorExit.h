
#pragma once
#include <Windows.h>
#include <string>

namespace Ambiesoft {
	void ErrorExit(LPCTSTR pMessage);
	void ErrorExit(const std::wstring& message);
	void ErrorExit(DWORD le);
}
