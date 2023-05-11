
// this should not be included
// because Qt project can not include MFC
// #include "stdafx.h"

#include <memory>

#ifdef _WIN32
#include <Windows.h>
#include <mbctype.h>
#include <conio.h>
#endif
#include "../DebugNew.h"
#include "gtest/gtest.h"

using namespace std;

int gMyclassCount;
class Myclass
{
	int thiscount = 0;
public:
	Myclass() {
		++gMyclassCount;
		thiscount = gMyclassCount;
	}
	~Myclass() {
		--gMyclassCount;
	}
};
void mysandbox()
{
	// these code is not allowed.
	// calling same dtor two times.
	//unique_ptr<Myclass> pU(new Myclass());
	//unique_ptr<Myclass> pU1;
	//pU1.reset(pU.get());

	// these code is not allowed.
	// calling same dtor two times.
	//shared_ptr<Myclass> pU(new Myclass());
	//shared_ptr<Myclass> pU1(pU);
	//pU1.reset(pU.get());

	if(false)
	{
		// OK
		shared_ptr<Myclass> pU(new Myclass());
		shared_ptr<Myclass> pU1(pU);
	}

	if (false)
	{
		// OK
		shared_ptr<Myclass> pU(new Myclass());
		shared_ptr<Myclass> pU1;
		pU1 = pU;
		pU1 = pU;
		pU1 = pU;
	}

	if(false)
	{
		// OK
		shared_ptr<Myclass> pU(new Myclass());
		shared_ptr<Myclass> pU1;
		pU1 = pU;
		pU1 = pU;
		pU1 = pU1;
		pU = pU1;
	}
	{
		// OK
		shared_ptr<Myclass> pU(new Myclass());
		shared_ptr<Myclass> pU1;
		pU1 = pU;
		pU1 = pU;
		pU1 = pU1;
		pU = pU1;
		pU = nullptr;
		pU1 = pU;  // dtor called
		pU1 = nullptr;
	}
}



void testStlMutex();
void testOpenedFiles();

struct CBeforeMain
{
	CBeforeMain()
	{
#ifdef _DEBUG
		_CrtSetDbgFlag(
			_CRTDBG_ALLOC_MEM_DF |
			_CRTDBG_LEAK_CHECK_DF |
			_CRTDBG_CHECK_ALWAYS_DF |
			_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif
	}
} gBeforeMain;

int main(int argc, char* argv[])
{
#ifdef _WIN32
	//CHAR szLocaleData[1024] = {};
	//GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNAME, szLocaleData, _countof(szLocaleData));
	//std::setlocale(LC_ALL, szLocaleData);
	//std::locale loc(".ACP");
	//std::locale::global(loc);
	
	// setlocale(LC_ALL, ".ACP");
	//_setmbcp(GetACP());
#endif
	mysandbox();

    testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	//if (argc == 1)
	//	_getch();
	return ret;
}

