// RunCommandGetResultCtrlCTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <assert.h>

#include "../../RunCommandGetResult.h"
#include "../../GetLastErrorString.h"
#include "../../CloseConsoleWin.h"
#include "../../stdosd/stdosd.h"

using namespace std;
using namespace Ambiesoft::stdosd;
using namespace Ambiesoft;


/*

Failed to send Ctrl+c with API, given up

*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	{
		string waitinput = stdCombinePath(
			stdGetParentDirectory(stdGetModuleFileName<char>()),
			"waitinput.exe");
		DWORD lRet, dwLE;
		string output, error;
		string argPre = "7CC65F45-805D-427E-B69A-0DB6FE17E173";
		string argPost = "CD26E3E9-2D42-4CF3-9BB8-2339D117976D3693FBD8-603C-49E0-B666-0FD06A173D7F";
		DWORD pid;
		BOOL bRet = RunCommandGetResultCallBack(
			waitinput.c_str(),
			(argPre + argPost).c_str(),
			&lRet,
			&dwLE,
			[](STARTUPINFOW* pSI, SECURITY_ATTRIBUTES* pSA, DWORD* pdwCreationFlags) {
				//pSI->wShowWindow = SW_SHOW;
				//*pdwCreationFlags |= CREATE_NEW_PROCESS_GROUP;
				return TRUE;
			},
			[&](PROCESS_INFORMATION* ppi) {
				pid = ppi->dwProcessId;
				assert(pid != 0);
			},
			[&](const char* p, void* pUD) {
				output += p;
				assert(pid != 0);
				if (false)
				{
					Sleep(3000);
					BOOL bRet = GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, pid);
					DWORD dwLE = GetLastError();
					if (!bRet)
					{
						wstring message = L"Failed " + GetLastErrorString(dwLE) + L"\r\nPress Ctrl+C manually.";
						MessageBox(nullptr, message.c_str(), 0, 0);
					}
				}
				else
				{
					CloseConsoleWin(pid);
				}
			},
				nullptr,
				nullptr,
				nullptr);

		assert(bRet);
		assert(output.find(argPre) != std::string::npos);
		assert(output.find(argPost) == std::string::npos);
	}

	MessageBox(nullptr, L"OK", 0, MB_ICONINFORMATION);
	return 0;
}
