
#include <cassert>

#include "../os_traits.h"

using namespace std;
using namespace Ambiesoft;
void testOSTraits()
{
	wstring module = mGetModuleFileName();
	assert(mIsFullPath(module));

	wstring a = L"C:\\aaa\\bbb";
	wstring b = L"ccc";
	wstring c = mCombinePath(a, b);
	assert(c == L"C:\\aaa\\bbb\\ccc");
}