
#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "../../../sqliteserialize.h"
#include "stlsoft/smartptr/scoped_handle.hpp"

using namespace Ambiesoft;

std::wstring getKey()
{
	static int si;
	++si;


	std::wstringstream ss;

	ss << L"KEY" << si;
	if(si > 5)
		si=0;
	return ss.str();;
}

int main()
{
	int trycount = 1;
	for (int i=0 ; i< trycount; ++i)
	{
		int d = (int)GetTickCount();
		std::wstring key=getKey();
		if (!sqlWritePrivateProfileInt(L"APP", key.c_str(), d, L"file.ini"))
		{
			std::cerr << "WRITE ERROR" << std::endl;
			
		}

		
		int f = sqlGetPrivateProfileInt(L"APP", key.c_str(), 0, L"file.ini");
		if(d != f)
		{
			std::cerr << "RACING ERROR: " << d << "!=" << f << std::endl;
		}
		if(f==0)
		{ 
			std::cerr << "CRITICAL ERROR: " << d << "!=" << f << std::endl;
		}
		std::cout << i << std::endl;
	}

	return 0;
}

