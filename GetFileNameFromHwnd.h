
#pragma once
#include <Windows.h>
#include <string>
namespace Ambiesoft {
	BOOL GetFileNameFromHwnd(HWND hWnd, LPTSTR lpszFileName, DWORD nSize);
	std::wstring GetFileNameFromHwndAsWstring(HWND hWnd);
}