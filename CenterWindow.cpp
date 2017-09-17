#include "stdafx.h"
#include <windows.h>
#include "CenterWindow.h"

namespace Ambiesoft {
	BOOL CenterWindow(HWND hwndChild, HWND hwndParent)
	{
		if (hwndParent == NULL)
			hwndParent = GetDesktopWindow();

		RECT    rChild, rParent, rWorkArea;
		int     wChild, hChild, wParent, hParent;
		int     xNew, yNew;
		BOOL  bResult;

		// Get the Height and Width of the child window
		GetWindowRect(hwndChild, &rChild);
		wChild = rChild.right - rChild.left;
		hChild = rChild.bottom - rChild.top;

		// Get the Height and Width of the parent window
		GetWindowRect(hwndParent, &rParent);
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