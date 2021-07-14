#include <windows.h>
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <string>

#include "../../../../MyUtility/ReplaceFileHelper.h"
#include "../../../../MyUtility/tstring.h"


int main()
{
	tstring file(_T("file.txt"));

	tifstream ifs(file.c_str());
	tstring str;
	ifs >> str;
	tcout << str << std::endl;
	ifs.close();


	str += _T("X");


	tstring tmp = file + (_T(".tmp"));
	tstring back = file + (_T(".back"));

	tofstream ofs;
	ofs.open(tmp.c_str());
	ofs << str << std::endl;
	ofs.close();


	DWORD dwLE=0;
	if(!ReplaceFileHelper(file.c_str(), tmp.c_str(), back.c_str(), 0, &dwLE))
	{
		std::cerr << "error:" << dwLE << std::endl;
		return 1;
	}

	return 0;
}