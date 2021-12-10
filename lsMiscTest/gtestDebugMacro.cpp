
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "../DebugNew.h"

#include "gtest/gtest.h"

using namespace std;
//using namespace Ambiesoft;
//using namespace Ambiesoft::stdosd;


TEST(DebugMacro, Basic)
{
	char* p = new char[100];
	strcpy(p, "ReportThis100bytes");
	// delete [] p;
}