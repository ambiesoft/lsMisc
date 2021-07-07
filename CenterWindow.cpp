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
#include "CenterWindow.h"

#include "DebugNew.h"

namespace Ambiesoft {
	void CenterRect(const RECT& rChild, const RECT& rParent,
		int& xNew, int& yNew)
	{
		int wChild, hChild, wParent, hParent;
		BOOL bResult;
		RECT rWorkArea;

		wChild = rChild.right - rChild.left;
		hChild = rChild.bottom - rChild.top;

		wParent = rParent.right - rParent.left;
		hParent = rParent.bottom - rParent.top;

		// Get the limits of the 'workarea'
		bResult = SystemParametersInfo(
			SPI_GETWORKAREA,  // system parameter to query or set
			sizeof(RECT),
			&rWorkArea,
			0);
		if (!bResult) {
			rWorkArea.left = rWorkArea.top = 0;
			rWorkArea.right = GetSystemMetrics(SM_CXSCREEN);
			rWorkArea.bottom = GetSystemMetrics(SM_CYSCREEN);
		}

		// Calculate new X position, then adjust for workarea
		xNew = rParent.left + ((wParent - wChild) / 2);
		if (xNew < rWorkArea.left) {
			xNew = rWorkArea.left;
		}
		else if ((xNew + wChild) > rWorkArea.right) {
			xNew = rWorkArea.right - wChild;
		}

		// Calculate new Y position, then adjust for workarea
		yNew = rParent.top + ((hParent - hChild) / 2);
		if (yNew < rWorkArea.top) {
			yNew = rWorkArea.top;
		}
		else if ((yNew + hChild) > rWorkArea.bottom) {
			yNew = rWorkArea.bottom - hChild;
		}
	}

	BOOL CenterWindow(HWND hwndChild, HWND hwndParent)
	{
		if (hwndParent == NULL)
			hwndParent = GetDesktopWindow();

		RECT rChild, rParent;
		int	xNew, yNew;

		// Get the Height and Width of the child window
		GetWindowRect(hwndChild, &rChild);
		GetWindowRect(hwndParent, &rParent);

		CenterRect(rChild, rParent, xNew, yNew);

		// Set it, and return
		return SetWindowPos(hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}




	bool MoveWindowCommon(HWND hWnd, MOVEWINDOWTYPE type)
	{
		if (!hWnd || !IsWindow(hWnd))
		{
			return false;
		}

		RECT workingarea;
		if (!SystemParametersInfo(
			SPI_GETWORKAREA,
			0,
			&workingarea,
			0))
		{
			return false;
		}

		RECT rWin;
		if (!GetWindowRect(hWnd, &rWin))
			return false;

		POINT targetPos = { 0 };

		switch (type)
		{
		case MOVEWINDOW_TOPLEFT:
			// targetPos = screenRect.Location;
			targetPos.x = workingarea.left;
			targetPos.y = workingarea.top;
			break;
		case MOVEWINDOW_TOPRIGHT:
			// targetPos.X = screenRect.Right - curSize.Width;
			// targetPos.Y = screenRect.Location.Y;
			targetPos.x = workingarea.right - (rWin.right - rWin.left);
			targetPos.y = workingarea.top;
			break;
		case MOVEWINDOW_BOTTOMLEFT:
			// targetPos.X = screenRect.Location.X;
			// targetPos.Y = screenRect.Bottom - curSize.Height;
			targetPos.x = workingarea.left;
			targetPos.y = workingarea.bottom - (rWin.bottom - rWin.top);

			break;
		case MOVEWINDOW_BOTTOMRIGHT:
			// targetPos.X = screenRect.Right - curSize.Width;
			// targetPos.Y = screenRect.Bottom - curSize.Height;
			targetPos.x = workingarea.right - (rWin.right - rWin.left);
			targetPos.y = workingarea.bottom - (rWin.bottom - rWin.top);
			break;

		default:
			return false;
		}

		//if(screenRect.Contains(targetPos))
		//{
		//	f->Location = targetPos;
		//}

		if (!SetWindowPos(hWnd, NULL, targetPos.x, targetPos.y,
			0, 0,
			SWP_NOSIZE | SWP_NOZORDER))
		{
			return false;
		}

		return true;
	}

}