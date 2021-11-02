#pragma once
#include <Windows.h>
#include <string>
#include <cassert>

// TODO: implem
// #define DVERIFY(a) assert(a)

void mytrace(LPCWSTR p);
void mytrace(const std::wstring& ss);
