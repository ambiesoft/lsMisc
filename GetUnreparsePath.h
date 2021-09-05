// https://docs.microsoft.com/ja-jp/windows/win32/memory/obtaining-a-file-name-from-a-file-handle?redirectedfrom=MSDN

#include <windows.h>
#include <string>
#include <vector>

namespace Ambiesoft {

	std::wstring GetUnreparsePath(const wchar_t* path);
	std::vector<std::wstring> GetUnreparsePath(const std::vector<std::wstring>& paths);
}