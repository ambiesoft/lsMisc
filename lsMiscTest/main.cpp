//#include "stdafx.h"

#include <memory>


#include <gtest/gtest.h>

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
void testExpandPath();

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
	mysandbox();

    testing::InitGoogleTest(&argc, argv);
	int gret = RUN_ALL_TESTS();

	
	//testExpandPath();
	//testOpenedFiles();
	//testStlMutex();



	//BOOL b = IsFileExists(_T("c:\\T\\aaa.txt"));
	//b = IsFileExistsA("c:\\T\\bbb.txt");
	//b = IsFileExistsW(L"c:\\T\\ccc");
    return gret;
}

