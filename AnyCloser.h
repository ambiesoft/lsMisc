//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.


//#include <boost/shared_ptr.hpp>
// general
template<typename CATEGORY, typename T> struct AnyCloser
{
	AnyCloser(T t) {
		static_assert(false, "This class should not be instanciated."); 
	};
	~AnyCloser();
};


typedef struct AC_CATEGORY_NORMAL_TYPE { int unused; } *AC_CATEGORY_NORMAL;
typedef struct AC_CATEGORY_CPPDELETE_TYPE { int unused; } *AC_CATEGORY_CPPDELETE;



// specialization for Windows Registory
template <> struct AnyCloser<AC_CATEGORY_NORMAL, HKEY>
{
	HKEY resource;
	AnyCloser(HKEY t) : resource(t) {}
	~AnyCloser() {
		RegCloseKey(resource);
	}
};
typedef AnyCloser<AC_CATEGORY_NORMAL, HKEY> RegCloser;


// specialization for CRT Free
typedef struct AC_UNUSED_CRT_FREE { int unused_000; } *AC_CRT_FREE;
template <> struct AnyCloser<AC_CATEGORY_NORMAL, AC_CRT_FREE>
{
	void* resource;
	AnyCloser(void* t) : resource(t) {}
	~AnyCloser() {
		free(resource);
	}
};
typedef AnyCloser<AC_CATEGORY_NORMAL, AC_CRT_FREE> CRTFreer;


// specialization for Windows Clipboard
typedef struct AC_UNSED_WIN32_CLIPBOARD { int unused; } *AC_WIN32_CLIPBOARD;
template <> struct AnyCloser<AC_CATEGORY_NORMAL, AC_WIN32_CLIPBOARD>
{
	//	AC_WIN32_CLIPBOARD resource;
	AnyCloser() {}
	~AnyCloser() {
		CloseClipboard();
	}
};
typedef AnyCloser<AC_CATEGORY_NORMAL, AC_WIN32_CLIPBOARD> ClipboardCloser;


// specialization for Windows BSTR
typedef struct AC_UNSED_WIN32_BSTR { int unused; } *AC_WIN32_BSTR;
template <> struct AnyCloser<AC_CATEGORY_NORMAL, AC_WIN32_BSTR>
{
	BSTR resource;
	AnyCloser(BSTR t) : resource(t) {}
	~AnyCloser() {
		SysFreeString(resource);
	}
};
typedef AnyCloser<AC_CATEGORY_NORMAL, AC_WIN32_BSTR> BSTRCloser;


typedef struct AC_UNSED_WIN32_TASKMEM { int unused333; } *AC_WIN32_TASKMEM;
template <> struct AnyCloser<AC_CATEGORY_NORMAL, AC_WIN32_TASKMEM>
{
	void* resource;
	AnyCloser(void* t) : resource(t) {}
	~AnyCloser() {
		CoTaskMemFree(resource);
	}
};
typedef AnyCloser<AC_CATEGORY_NORMAL, AC_WIN32_TASKMEM> TASKMEMCloser;

#include <stdio.h>
typedef struct AC_UNSED_WIN32_CRTFILE { int unused333; } *AC_WIN32_CRTFILE;
template <> struct AnyCloser<AC_CATEGORY_NORMAL, AC_WIN32_CRTFILE>
{
	void* resource;
	AnyCloser(void* t) : resource(t) {}
	~AnyCloser() {
		fclose((FILE*)resource);
	}
};
typedef AnyCloser<AC_CATEGORY_NORMAL, AC_WIN32_CRTFILE> CRTFileCloser;

/***
struct TEC {
	typedef  BOOL  (WINAPI*FT)(HANDLE);

	FT f_;
	TEC(FT f) {
		f_ = f;
	}
};
***/


// class for CPP delete
//template <typename T> struct CPPDeleterType
//{
//	T* resouce;
//	CPPDeleter(T* t) : resource(t) {}
//	~CPPDeleter() {
//		delete resource;
//	}
//};

/**
class CPPDeleter {
boost::shared_ptr< void > resource;
public:
template<typename T>
CPPDeleter(T* t) : resource(t) {}
~CPPDeleter() {}
}; 
**/


/**
class CPPDeleter{ 
	CPPDeleter(const CPPDeleter&); 
	CPPDeleter& operator=(const CPPDeleter&); 
	void *ptr;
	void (*delfn)(void*);
	

public: 
	template<typename T> CPPDeleter(T* t) : ptr(t), delfn(deleter<T>) {} 
	~CPPDeleter() {
		delfn(ptr);
	}  
	template<typename T>
	void deleter(void *p) {
		delete (T*)p;
	}

}; 
**/