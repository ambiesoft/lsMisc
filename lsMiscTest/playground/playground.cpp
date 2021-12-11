// playground.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

//void* __cdecl operator new(size_t nSize, int nType, const char* lpszFileName, int nLine);
//
//
//void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
//{
//	return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
//}
//
//void* operator new[](size_t nSize, LPCSTR lpszFileName, int nLine)
//{
//	return ::operator new[](nSize, _NORMAL_BLOCK, lpszFileName, nLine);
//}
//
//void  operator delete(void* pData, LPCSTR /* lpszFileName */,
//	int /* nLine */)
//{
//	::operator delete(pData);
//}
//
//void  operator delete[](void* pData, LPCSTR /* lpszFileName */,
//	int /* nLine */)
//{
//	::operator delete(pData);
//}
//
//
//void* __cdecl operator new(size_t nSize)
//{
//	return _malloc_dbg(nSize, _NORMAL_BLOCK, NULL, 0);
//}
//
//void __cdecl operator delete(void* p)
//{
//	_free_dbg(p, _NORMAL_BLOCK);
//}
//
//void* __cdecl operator new[](size_t nSize)
//{
//	return ::operator new(nSize);
//}
//
//void __cdecl operator delete[](void* p)
//{
//	::operator delete(p);
//}
//
//void* __cdecl operator new(size_t nSize, int nType, LPCSTR lpszFileName, int nLine)
//{
//	return _malloc_dbg(nSize, nType, lpszFileName, nLine);
//
//}
//
//void __cdecl operator delete(void* p, int nType, LPCSTR /* lpszFileName */, int /* nLine */)
//{
//	_free_dbg(p, nType);
//}
//
//void* __cdecl operator new[](size_t nSize, int nType, LPCSTR lpszFileName, int nLine)
//{
//	return ::operator new(nSize, nType, lpszFileName, nLine);
//}
//
//void __cdecl operator delete[](void* p, int nType, LPCSTR lpszFileName, int nLine)
//{
//	::operator delete(p, nType, lpszFileName, nLine);
//}
//
//
//
#include "../../DebugNew.h"

int main()
{
	int* p = new int;
	*p = 333;
	delete p;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
