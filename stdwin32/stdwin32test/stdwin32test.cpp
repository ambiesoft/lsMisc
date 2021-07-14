#include "stdafx.h"

#include <iostream>
#include <windows.h>

#include "../stdwin32.h"

using namespace std;
using namespace stdwin32;

#if 1 // check WINVER,_WIN32_*.
  #define STRING2(x) #x
  #define STRING(x) STRING2(x)
  #pragma message("WINVER        : " STRING(WINVER))
  #pragma message("_WIN32_WINNT  : " STRING(_WIN32_WINNT))
//  #pragma message("_WIN32_WINDOWS: " STRING(_WIN32_WINDOWS)) // Windows9x only
  #pragma message("_WIN32_IE     : " STRING(_WIN32_IE))
#endif

int main(int argc, char* argv[])
{
	cout << "_WIN32_WINNT=" << "0x" << std::hex << _WIN32_WINNT << endl;
	cout << "WINVER=" << "0x" << std::hex << WINVER << endl;
	cout << "_WIN32_IE=" << "0x" << std::hex << _WIN32_IE << endl;

	string exe = stdGetModuleFileName();
	wstring wexe = stdGetModuleFileNameW();

	std::wstring shortname = stdwin32::stdGetShortPath(L"C:\\Users\\zgjEr\\Desktop\\aaaaaaaaaaaaaaaaaaaaaaaaa.txt");

	std::string s=	string_format("%d-%s", 100, "aaa");
	wstring ws;

	s = stdItoA(1234);
	assert(s=="1234");

	s = stdItoA(-3334);
	assert(s=="-3334");


	s = stdItoA64(4444555522233444L);
	assert(s=="4444555522233444");

	ws = stdItoW64(-1010);
	assert(ws==L"-1010");
	
	ws = stdItoW(5);
	assert(ws==L"5");


	assert(stdGetFirstLine(L"") == L"");
	assert(stdGetFirstLine(L"a") == L"a");
	assert(stdGetFirstLine(L"a\r") == L"a");
	assert(stdGetFirstLine(L"a\r\n") == L"a");
	assert(stdGetFirstLine(L"a\n") == L"a");
	return 0;
}
