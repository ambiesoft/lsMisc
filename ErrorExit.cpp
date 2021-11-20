


#include <Windows.h>
#include <tchar.h>

#include <string>

#include "stdosd/stdosd.h"
#include "GetLastErrorString.h"
#include "ErrorExit.h"

using namespace std;
using namespace Ambiesoft::stdosd;

namespace Ambiesoft {
	void ErrorExit(LPCTSTR pMessage)
	{
		MessageBox(NULL, 
			pMessage, 
			stdGetFileNameWitoutExtension(stdGetModuleFileName()).c_str(),
			MB_ICONERROR);
		ExitProcess(1);
	}
	void ErrorExit(const wstring& message)
	{
		ErrorExit(message.c_str());
	}
	void ErrorExit(DWORD le)
	{
		ErrorExit(GetLastErrorString(le).c_str());
	}
}