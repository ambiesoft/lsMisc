
#ifdef _WIN32
#ifdef _DEBUG
#include <Windows.h>
#include <crtdbg.h>

void*  operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
	return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}

void*  operator new[](size_t nSize, LPCSTR lpszFileName, int nLine)
{
	return ::operator new[](nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}

void  operator delete(void* pData, LPCSTR /* lpszFileName */,
	int /* nLine */)
{
	::operator delete(pData);
}

void  operator delete[](void* pData, LPCSTR /* lpszFileName */,
	int /* nLine */)
{
	::operator delete(pData);
}


void* operator new(size_t nSize)
{
	return _malloc_dbg(nSize, _NORMAL_BLOCK, NULL, 0);
}

void operator delete(void* p)
{
	_free_dbg(p, _NORMAL_BLOCK);
}

void*  operator new[](size_t nSize)
{
	return ::operator new(nSize);
}

void  operator delete[](void* p)
{
	::operator delete(p);
}

void* operator new(size_t nSize, int nType, LPCSTR lpszFileName, int nLine)
{
	return _malloc_dbg(nSize, nType, lpszFileName, nLine);
}

void operator delete(void* p, int nType, LPCSTR /* lpszFileName */, int /* nLine */)
{
	_free_dbg(p, nType);
}

void* operator new[](size_t nSize, int nType, LPCSTR lpszFileName, int nLine)
{
	return ::operator new(nSize, nType, lpszFileName, nLine);
}

void operator delete[](void* p, int nType, LPCSTR lpszFileName, int nLine)
{
	::operator delete(p, nType, lpszFileName, nLine);
}


#endif  // _DEBUG
#endif  // _WIN32
