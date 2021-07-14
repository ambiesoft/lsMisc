#include "stdafx.h"
#include <string>
#include "../../../../MyUtility/browseFolder.h"

using namespace std;
int main()
{
	TCHAR szFolder[MAX_PATH] = { 0 };
	bool ret = browseFolder(NULL, L"titleTest", szFolder);

	wstring message;
	if (ret)
		message = L"OK";
	else
		message = L"Cancel";
	
	message += L"\r\n";
	message += szFolder;
	MessageBox(NULL, message.c_str(), L"test", MB_OK);
	return 0;
}