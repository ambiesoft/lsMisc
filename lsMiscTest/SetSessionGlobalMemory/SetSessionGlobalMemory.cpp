
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

#include <string>
#include <iostream>
#include <inttypes.h>

#include "../../SessionGlobalMemory/SessionGlobalMemory.h"
#include "../../CommandLineParser.h"
#include "../../stdosd/stdosd.h"
#include "../../CHandle.h"

using namespace Ambiesoft;
using namespace Ambiesoft::stdosd;
using namespace std;

int main(int argc, const char* argv[])
{
    CCommandLineParserA parser;
    string name;
    parser.AddOption("-name", 1, &name);

    int value;
    parser.AddOption("-value", 1, &value);

    string eventname;
    parser.AddOption("-event", 1, &eventname);

    parser.Parse(argc, argv);

    if (name.empty())
    {
        cerr << "name is empty" << endl;
        return 1;
    }
    if (eventname.empty())
    {
        cerr << "eventname is empty" << endl;
        return 1;
    }
    Ambiesoft::CKernelHandle event(CreateEventA(nullptr, TRUE, TRUE, eventname.c_str()));
    if (!event)
    {
        cerr << "failed to CreateEvent" << endl;
        return 1;
    }
    if (GetLastError() != ERROR_ALREADY_EXISTS)
    {
        cerr << "Event not found" << endl;
        return 1;
    }
    
    cout << stdFormat("event=%p, name='%s', value=%d",
        (HANDLE)event, name.c_str(), value) << endl;;

    CSessionGlobalMemory<int> sg(name.c_str(), value);
    SetEvent((HANDLE)event);


    cout << "Hit any key to quit...";
    getchar();
}

