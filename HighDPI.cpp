//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#ifndef PCH_INCLUDE_FILE
// Define PCH_INCLUDE_FILE outside of the code
#define PCH_INCLUDE_FILE "StdAfx.h"
#endif
#include PCH_INCLUDE_FILE

#include <windows.h>
#include "HighDPI.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Gdi32.lib")


EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#ifndef WM_DPICHANGED
#define WM_DPICHANGED                   0x02E0
#endif

namespace Ambiesoft {
	HMODULE ghUser32;

	typedef enum myDPI_AWARENESS {
		myDPI_AWARENESS_INVALID = -1,
		myDPI_AWARENESS_UNAWARE = 0,
		myDPI_AWARENESS_SYSTEM_AWARE = 1,
		myDPI_AWARENESS_PER_MONITOR_AWARE = 2
	} myDPI_AWARENESS;

	enum {
		InitRet_Uninitialized = -1,
		InitRet_Success = 0,
		InitRet_AlreadyInitialized = 1,
		InitRet_LoadLibraryFailed = 2,
		InitRet_ThreadOK = 3,
		InitRet_ProcessOK = 4,
		InitRet_NotAvailable = 5,
	};

	typedef void*(WINAPI *fnSetThreadDpiAwarenessContext)(void* dpiContext);
	static fnSetThreadDpiAwarenessContext mySetThreadDpiAwarenessContext;

	typedef void*(WINAPI *fnGetThreadDpiAwarenessContext)(VOID);
	static fnGetThreadDpiAwarenessContext myGetThreadDpiAwarenessContext;

	typedef myDPI_AWARENESS(WINAPI *fnGetAwarenessFromDpiAwarenessContext)(void*);
	fnGetAwarenessFromDpiAwarenessContext myGetAwarenessFromDpiAwarenessContext;

	typedef UINT(WINAPI* fnGetDpiForSystem)(VOID);
	static fnGetDpiForSystem myGetDpiForSystem;

	typedef UINT(WINAPI* fnGetDpiForWindow)(HWND hwnd);
	static fnGetDpiForWindow myGetDpiForWindow;

	typedef BOOL(WINAPI* fnSetProcessDPIAware)(VOID);
	static fnSetProcessDPIAware mySetProcessDPIAware;

	typedef BOOL(WINAPI* fnSystemParametersInfoForDpi)(
		UINT uiAction,
		UINT uiParam,
		PVOID pvParam,
		UINT fWinIni,
		UINT dpi);
	static fnSystemParametersInfoForDpi mySystemParametersInfoForDpi;

	typedef UINT(WINAPI* fnGetDpiFromDpiAwarenessContext)(void*);
	static fnGetDpiFromDpiAwarenessContext myGetDpiFromDpiAwarenessContext;

	typedef int (WINAPI* fnGetSystemMetricsForDpi)(
		int  nIndex,
		UINT dpi
		);
	static fnGetSystemMetricsForDpi myGetSystemMetricsForDpi;

	static int prepareFunctions()
	{
		static int ret = InitRet_Uninitialized;
		if (ret != InitRet_Uninitialized)
			return ret;

		if (ghUser32)
			return InitRet_AlreadyInitialized;

		ghUser32 = LoadLibraryA("user32.dll");

		if (!ghUser32)
			return InitRet_LoadLibraryFailed;

		mySetThreadDpiAwarenessContext =
			(fnSetThreadDpiAwarenessContext)GetProcAddress(
				ghUser32, "SetThreadDpiAwarenessContext");

		myGetThreadDpiAwarenessContext =
			(fnGetThreadDpiAwarenessContext)GetProcAddress(
				ghUser32, "GetThreadDpiAwarenessContext");

		myGetAwarenessFromDpiAwarenessContext =
			(fnGetAwarenessFromDpiAwarenessContext)GetProcAddress(
				ghUser32, "GetAwarenessFromDpiAwarenessContext");

		myGetDpiForSystem =
			(fnGetDpiForSystem)GetProcAddress(
				ghUser32, "GetDpiForSystem");

		myGetDpiForWindow =
			(fnGetDpiForWindow)GetProcAddress(
				ghUser32, "GetDpiForWindow");

		mySetProcessDPIAware =
			(fnSetProcessDPIAware)GetProcAddress(
				ghUser32, "SetProcessDPIAware");

		mySystemParametersInfoForDpi =
			(fnSystemParametersInfoForDpi)GetProcAddress(
				ghUser32, "SystemParametersInfoForDpi");

		myGetDpiFromDpiAwarenessContext =
			(fnGetDpiFromDpiAwarenessContext)GetProcAddress(
				ghUser32, "GetDpiFromDpiAwarenessContext");

		myGetSystemMetricsForDpi =
			(fnGetSystemMetricsForDpi)GetProcAddress(
				ghUser32, "GetSystemMetricsForDpi");

		return ret = InitRet_Success;
	}

	int GetDPIScale()
	{
		HDC screen = GetDC(0);

		int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
		int dpiY = GetDeviceCaps(screen, LOGPIXELSY);

		ReleaseDC(0, screen);
		return dpiX;
	}

	double GetDPIScaleX(HWND hWnd)
	{
		HMONITOR monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

		// Get the logical width and height of the monitor
		MONITORINFOEX monitorInfoEx;
		monitorInfoEx.cbSize = sizeof(monitorInfoEx);
		GetMonitorInfo(monitor, &monitorInfoEx);
		auto cxLogical = monitorInfoEx.rcMonitor.right - monitorInfoEx.rcMonitor.left;
		auto cyLogical = monitorInfoEx.rcMonitor.bottom - monitorInfoEx.rcMonitor.top;

		// Get the physical width and height of the monitor
		DEVMODE devMode;
		devMode.dmSize = sizeof(devMode);
		devMode.dmDriverExtra = 0;
		EnumDisplaySettings(monitorInfoEx.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
		auto cxPhysical = devMode.dmPelsWidth;
		auto cyPhysical = devMode.dmPelsHeight;

		// Calculate the scaling factor
		auto horizontalScale = ((double)cxPhysical / (double)cxLogical);
		auto verticalScale = ((double)cyPhysical / (double)cyLogical);

		return horizontalScale;
	}

	UINT GetWindowDPIHighDPISupport_For10(HWND hWnd)
	{
		UINT uDpi = 96;
		if (InitRet_Success != prepareFunctions())
			return uDpi;
		
		if (!myGetAwarenessFromDpiAwarenessContext || !myGetThreadDpiAwarenessContext)
			return uDpi;

		void* threadcontext = myGetThreadDpiAwarenessContext();
		UINT dpiAwareness = myGetAwarenessFromDpiAwarenessContext(threadcontext);

		switch (dpiAwareness)
		{
			// Scale the window to the system DPI
		case myDPI_AWARENESS_SYSTEM_AWARE:
			uDpi = myGetDpiForSystem();
			break;

			// Scale the window to the monitor DPI
		case myDPI_AWARENESS_PER_MONITOR_AWARE:
			if (IsWindow(hWnd))
			{
				if (myGetDpiForWindow)
					uDpi = myGetDpiForWindow(hWnd);
			}
			else
			{
				if (myGetDpiFromDpiAwarenessContext)
				{
					uDpi = myGetDpiFromDpiAwarenessContext(threadcontext);
					if (uDpi == 0)
					{
						if (!myGetDpiForSystem)
							uDpi = 96;
						else
							uDpi = myGetDpiForSystem();
					}
				}
			}
			break;
		}
		return uDpi;
	}

	//static HHOOK gHook;
	//static LRESULT CALLBACK CallWndProc(
	//	int    code,
	//	WPARAM wParam,
	//	LPARAM lParam
	//	)
	//{
	//	if (code != HC_ACTION)
	//	{
	//		return CallNextHookEx(gHook, code, wParam, lParam);
	//	}

	//	// MSG* pMsg = reinterpret_cast<MSG*>(lParam);
	//	CWPSTRUCT *p = reinterpret_cast<PCWPSTRUCT>(lParam);
	//	UINT m = p->message;
	//	HWND h = p->hwnd;
	//	if (m == WM_DPICHANGED || m ==WM_CREATE)
	//	{
	//		UINT dpi = getWindowDPI(h);
	//		RECT rcWindow = {};
	//		GetWindowRect(h, &rcWindow);
	//		LONG width = rcWindow.right - rcWindow.left;
	//		LONG height = rcWindow.bottom - rcWindow.top;

	//		rcWindow.right = rcWindow.left + MulDiv(width, dpi, 96);
	//		rcWindow.bottom = rcWindow.top + MulDiv(height, dpi, 96);
	//		SetWindowPos(h, NULL, 
	//			rcWindow.right, rcWindow.top, 
	//			rcWindow.right - rcWindow.left, 
	//			rcWindow.bottom - rcWindow.top, 
	//			SWP_NOZORDER | SWP_NOACTIVATE);
	//	}

	//	return CallNextHookEx(gHook, code, wParam, lParam);
	//}


	int InitHighDPISupport(bool bThread)
	{
		prepareFunctions();

		int ret = InitRet_NotAvailable;
		if (mySetThreadDpiAwarenessContext && bThread)
		{
			// DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
			mySetThreadDpiAwarenessContext((void*)-4);
			//gHook = SetWindowsHookExA(
			//	WH_CALLWNDPROC,
			//	CallWndProc,
			//	(HMODULE)&__ImageBase, 
			//	GetCurrentThreadId());

			ret = InitRet_ThreadOK;
		}
		else if (mySetProcessDPIAware)
		{
			mySetProcessDPIAware();
			ret = InitRet_ProcessOK;
		}

		return ret;
	}

	BOOL SystemParamInfoHighDPISupportA(
		HWND hWnd,
		UINT uiAction,
		UINT uiParam,
		PVOID pvParam,
		UINT fWinIni)
	{
		if (!mySystemParametersInfoForDpi)
			return SystemParametersInfoA(uiAction, uiParam, pvParam, fWinIni);

		UINT dpi = GetWindowDPIHighDPISupport_For10(hWnd);
		return mySystemParametersInfoForDpi(uiAction, uiParam, pvParam, fWinIni, dpi);
	}
	BOOL SystemParamInfoHighDPISupportW(
		HWND hWnd,
		UINT uiAction,
		UINT uiParam,
		PVOID pvParam,
		UINT fWinIni)
	{
		if (!mySystemParametersInfoForDpi)
			return SystemParametersInfoW(uiAction, uiParam, pvParam, fWinIni);

		UINT dpi = GetWindowDPIHighDPISupport_For10(hWnd);
		return mySystemParametersInfoForDpi(uiAction, uiParam, pvParam, fWinIni, dpi);
	}

	int GetSystemMetricsHighDPISupport(HWND hWnd, int nIndex)
	{
		if (!myGetSystemMetricsForDpi)
			return GetSystemMetrics(nIndex);

		UINT dpi = GetWindowDPIHighDPISupport_For10(hWnd);
		return myGetSystemMetricsForDpi(nIndex, dpi);
	}

	void AdjustWindowSizeHighDPISupport(HWND hWnd)
	{
		UINT dpi = GetWindowDPIHighDPISupport_For10(hWnd);
		RECT rcWindow = {};
		GetWindowRect(hWnd, &rcWindow);
		LONG width = rcWindow.right - rcWindow.left;
		LONG height = rcWindow.bottom - rcWindow.top;

		rcWindow.right = rcWindow.left + MulDiv(width, dpi, 96);
		rcWindow.bottom = rcWindow.top + MulDiv(height, dpi, 96);
		SetWindowPos(hWnd, NULL,
			rcWindow.right, rcWindow.top,
			rcWindow.right - rcWindow.left,
			rcWindow.bottom - rcWindow.top,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
}