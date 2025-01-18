// testScreenSaver.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "../../../lsMisc/ScreenSaver.h"

#include "testScreenSaver.h"

constexpr wchar_t APP_NAME[] = L"testScreenSavoer";

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Sleep(5 * 1000);

    bool prevOperation = IsScreenSaverActive();
    bool result = RecoverFromScreenSavor(SCREEN_RECOVER::ALL);
    bool afterOperation = IsScreenSaverActive();

    Sleep(5 * 1000);

    MessageBox(nullptr,
        stdFormat(L"Previous Screen Saver State:%d\r\nResult:%d\r\nAfter Screen Saver State:%d",
            prevOperation,
            result,
            afterOperation).c_str(),
        APP_NAME,
        MB_ICONINFORMATION);
    return 0;
}
