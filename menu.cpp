
#include <windows.h>

#include "menu.h"
namespace Ambiesoft{

	BOOL IsMenuChecked(HMENU hMenu, UINT id)
	{
		MENUITEMINFO mii;
		ZeroMemory(&mii, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STATE;
		GetMenuItemInfo(hMenu, id, FALSE, &mii);
		return (mii.fState & MFS_CHECKED) ? TRUE : FALSE;
	}
	BOOL CheckMenu(HMENU hMenu, UINT id, bool bCheck)
	{
		MENUITEMINFO mii;
		ZeroMemory(&mii, sizeof(mii));
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STATE;
		if (!GetMenuItemInfo(hMenu, id, FALSE, &mii))
			return false;
		if (bCheck)
			mii.fState |= MFS_CHECKED;
		else
			mii.fState &= ~MFS_CHECKED;
		return SetMenuItemInfo(hMenu, id, FALSE, &mii);
	}
}