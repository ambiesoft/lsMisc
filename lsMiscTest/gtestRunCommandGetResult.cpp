#include "stdafx.h"
#include <string>

#include "gtest/gtest.h"


#include "../RunCommandGetResult.h"
#include "../CloseConsoleWin.h"

#include "../stdosd/stdosd.h"
using namespace std;
using namespace Ambiesoft::stdosd;
using namespace Ambiesoft;
static void cback(const char* pData, void* pUD)
{
	string* ps = (string*)pUD;
	*ps += pData;
}
TEST(RunCommandGetResult, Basic)
{

	wstring thisexe = stdGetModuleFileName<wchar_t>();
	DWORD lRet, dwLE;
	string output, error;

	BOOL bRet = RunCommandGetResult(
		thisexe.c_str(),
		L"--help",
		&lRet,
		&dwLE,
		&output,
		&error);
	EXPECT_EQ(TRUE, bRet);
	EXPECT_EQ(error, "");
	EXPECT_TRUE(output.find("Google Test") != std::string::npos);

}
TEST(RunCommandGetResult, CallBack)
{

	wstring thisexe = stdGetModuleFileName<wchar_t>();
	DWORD lRet;
	DWORD dwLE = 0;

	string output;
	BOOL bRet = RunCommandGetResultCallBack(
		thisexe.c_str(),
		L"--help",
		&lRet,
		&dwLE,
		nullptr,
		nullptr,
		cback,
		&output,
		nullptr,
		nullptr);
	EXPECT_EQ(TRUE, bRet);
	EXPECT_EQ(0, lRet);
	EXPECT_EQ(0, dwLE);

	EXPECT_TRUE(output.find("Google Test") != std::string::npos);

}
// CloseConsoleWin does not work in test environment, use RunCommandGetResultCtrlCTest
//TEST(RunCommandGetResult, Close)
//{
//	{
//		string waitinput = stdCombinePath(
//			stdGetParentDirectory(stdGetModuleFileName<char>()),
//			"waitinput.exe");
//		DWORD lRet, dwLE;
//		string output, error;
//
//		// "waitinput.exe" shows 'argPre', waits and shows argPost.
//		// This code closes 'waitinput.exe' by posting WM_CLOSE
//		string argPre = "7CC65F45-805D-427E-B69A-0DB6FE17E173";
//		string argPost = "CD26E3E9-2D42-4CF3-9BB8-2339D117976D3693FBD8-603C-49E0-B666-0FD06A173D7F";
//		DWORD pid;
//		BOOL bRet = RunCommandGetResultCallBack(
//			waitinput.c_str(),
//			(argPre + " " + argPost).c_str(),
//			&lRet,
//			&dwLE,
//			[](STARTUPINFOW* pSI, SECURITY_ATTRIBUTES* pSA, DWORD* pdwCreationFlags) {
//				//pSI->wShowWindow = SW_SHOW;
//				//*pdwCreationFlags |= CREATE_NEW_PROCESS_GROUP;
//				return TRUE;
//			},
//			[&](PROCESS_INFORMATION* ppi) {
//				pid = ppi->dwProcessId;
//				EXPECT_TRUE(pid != 0);
//			},
//				[&](const char* p, void* pUD) {
//				output += p;
//				assert(pid != 0);
//				if (false)
//				{
//					//Sleep(3000);
//					//BOOL bRet = GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, pid);
//					//DWORD dwLE = GetLastError();
//					//if (!bRet)
//					//{
//					//	wstring message = L"Failed " + GetLastErrorString(dwLE) + L"\r\nPress Ctrl+C manually.";
//					//	MessageBox(nullptr, message.c_str(), 0, 0);
//					//}
//				}
//				else
//				{
//					EXPECT_TRUE(!!CloseConsoleWin(pid));
//				}
//			},
//				nullptr,
//				nullptr,
//				nullptr);
//
//		EXPECT_TRUE(!!bRet);
//		EXPECT_TRUE(output.find(argPre) != std::string::npos);
//		EXPECT_TRUE(output.find(argPost) == std::string::npos);
//	}
//}
