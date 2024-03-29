
#include <Windows.h>
#include "resource.h"

#include <gtest/gtest.h>

#include "../TrayIcon.h"
#include "../CreateSimpleWindow.h"
#include "../showballoon.h"
#include "../CHandle.h"

using namespace Ambiesoft;
using namespace std;

TEST(TrayIcon, Add)
{
	//EXPECT_TRUE(showballoon(
	//	NULL,
	//	L"tttitle",
	//	L"teeeeeeeext",
	//	NULL,
	//	3000,
	//	3332,
	//	FALSE,
	//	1));

	CHWnd h(CreateSimpleWindow());
	EXPECT_TRUE(::IsWindow(h));

	int id = 100;
	CHIcon hIcon(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_MAIN)));
	EXPECT_TRUE(!!hIcon);
	wstring appname = L"appname";
	wstring tip = L"this is a tip";
	EXPECT_TRUE(AddTrayIcon(h, id, hIcon, tip.c_str()));
	EXPECT_TRUE(PopupTrayIcon(h, id, hIcon, appname.c_str(), tip.c_str()));
	EXPECT_TRUE(RemoveTrayIcon(h, id));
}