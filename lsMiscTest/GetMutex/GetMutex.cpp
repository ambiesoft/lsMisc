#include "framework.h"
#include "GetMutex.h"

#include "../../IsDuplicateInstance.h"

using namespace Ambiesoft;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    LPCWSTR pMutexName = L"lsmisctest_IsDuplicateInstance";
    bool ret = IsDuplicateInstance(pMutexName);
    Sleep(3000);
    return ret;
}