// waitinput.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>

int main(int argc, const char* argv[])
{
    std::cout << "7CC65F45-805D-427E-B69A-0DB6FE17E173\n"; 
	std::string s;
	std::cin >> s;
	std::cout << "Input is " << s << std::endl;
	
	std::cout << "CommandLine is ";
	for (int i = 0; i < argc; ++i)
	{
		std::cout << argv[i];
	}
	std::cout << std::endl;
}
