
#include <vector>

#include "../OpenedFiles.h"

using namespace std;



void testOpenedFiles()
{
	wstring message;
	vector<OPENEDFILEINFO> v;
	GetOpenedFilesSimple(L"C:\\T\\aaa\\aaa.txt", v);
	for each(const OPENEDFILEINFO& t in v)
	{
		message.append(t.filename);
		message.append(L"\r\n");

		message.append(GetPathFromProcessID(t.dwPID));
		message.append(L"\r\n");
	}

	MessageBox(NULL, message.c_str(), L"testOpenedFiles", MB_ICONINFORMATION);
}