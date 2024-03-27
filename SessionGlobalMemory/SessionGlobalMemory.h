//BSD 3-Clause License
//
//Copyright (c) 2017, Ambiesoft
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//* Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//* Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//* Neither the name of the copyright holder nor the names of its
//  contributors may be used to endorse or promote products derived from
//  this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <Windows.h>

#include <cassert>
#include <string>

#define size_t size_is_not_used_since_its_size_is_different_between_32_and_64

#ifdef _DEBUG
#define AMBIESOFT_VERIFY(e) assert(e)
#define AMBIESOFT_VERIFY_ZERO(e) assert(0==(e))
#else
#define AMBIESOFT_VERIFY(e)			((void)(e))
#define AMBIESOFT_VERIFY_ZERO(e)	((void)(e))
#endif

#if _MSC_VER < 1600		// less than VC2010
#define AMBIESOFT_NO_RVALUE_
#endif

namespace Ambiesoft {

	typedef unsigned int size_t32;
	template<class T, class C = char >
	class CSessionGlobalMemory
	{
		typedef typename CSessionGlobalMemory<T,C> MYT;
#ifdef _DEBUG
		int initialized_;
#endif

	public:
		explicit CSessionGlobalMemory(const C* pName) {
#ifdef _DEBUG
			initialized_ = 0;
#endif
			init(pName);
			ensure();
		}
		CSessionGlobalMemory(const C* pName, const T& t)
		{
#ifdef _DEBUG
			initialized_ = 0;
#endif

			init(pName);
			set(t);
		}
		virtual ~CSessionGlobalMemory() {
			release();
		}
		CSessionGlobalMemory()
		{
#ifdef _DEBUG
			initialized_ = 1;
#endif
		}

		// Using constructor and assign operator in std::container
		// will lose it value because destructor was called.
		// Use move()
//		CSessionGlobalMemory(const MYT& rhv)
//		{
//#ifdef _DEBUG
//			initialized_ = 0;
//#endif
//
//			init(rhv.m_pName);
//		}
		//MYT& operator=(const MYT& rhv) {
		//	if (this != reinterpret_cast<MYT*>(const_cast<MYT*>(&rhv)))
		//	{
		//		release();
		//		init(rhv.m_pName);
		//	}
		//	return *this;
		//}

#if !defined(AMBIESOFT_NO_RVALUE_)
		CSessionGlobalMemory(MYT&& rhv) noexcept
		{
#ifdef _DEBUG
			initialized_ = 0;
#endif
			move(std::move(rhv));
#ifdef _DEBUG
			initialized_ = 1;
#endif
		}
		MYT& operator=(MYT&& rhv) {
			if (this != reinterpret_cast<MYT*>(&rhv))
			{
				release();
				move(std::move(rhv));
#ifdef _DEBUG
				initialized_ = 1;
#endif
			}
			return *this;
		}


		void move(MYT&& rhv) noexcept {
			m_pName = rhv.m_pName;
			rhv.m_pName = NULL;

			m_pNameFileMap = rhv.m_pNameFileMap;
			rhv.m_pNameFileMap = NULL;

			m_pMutexName = rhv.m_pMutexName;
			rhv.m_pMutexName = NULL;

			m_ = rhv.m_;
			rhv.m_ = NULL;

			h_ = rhv.h_;
			rhv.h_ = NULL;

			p_ = rhv.p_;
			rhv.p_ = NULL;
		}
#endif

		operator T() const {
			if (!ensure())
				return T();
			Locker l(this);
			//T t;
			//memcpy(&t, p_, sizeof(t));

			return *((T*)p_);
		}

		virtual void get(T& t) const {
			if (!ensure())
				return;
			Locker l(this);
			memcpy(&t, p_, size());
		}

		MYT& operator =(const T& t) {
			set(t);
			return *this;
		}

		std::basic_string<C> getName() const {
			return m_pName;
		}
		std::basic_string<C> getMapName() const {
			return m_pNameFileMap;
		}

		static int lstrlenT(const char* p)
		{
			return lstrlenA(p);
		}
		static int lstrlenT(const wchar_t* p)
		{
			return lstrlenW(p);
		}

		static char* lstrcpyT(char* p1, const char* p2)
		{
			return lstrcpyA(p1, p2);
		}
		static wchar_t* lstrcpyT(wchar_t* p1, const wchar_t* p2)
		{
			return lstrcpyW(p1, p2);
		}

		static char* lstrcatT(char* p1, const char* p2)
		{
			return lstrcatA(p1, p2);
		}
		static wchar_t* lstrcatT(wchar_t* p1, const wchar_t* p2)
		{
			return lstrcatW(p1, p2);
		}

		template<class CLIT>
		static const CLIT* prefFileMap();
		template<>
		static const char* prefFileMap<char>()
		{
			return "_FileMap";
		}
		template<>
		static const wchar_t* prefFileMap<wchar_t>()
		{
			return L"_FileMap";
		}

		template<class CLIT>
		static const CLIT* prefMutex();
		template<>
		static const char* prefMutex<char>()
		{
			return "_Mutex";
		}
		template<>
		static const wchar_t* prefMutex<wchar_t>()
		{
			return L"_Mutex";
		}
	protected:
		void init(const C* pName) {
#ifdef _DEBUG
			assert(initialized_ == 0);
			initialized_ = 1;
#endif
			h_ = NULL;
			p_ = NULL;
			m_ = NULL;

			size_t32 len = lstrlenT(pName);
			m_pName = (C*)LocalAlloc(LMEM_FIXED, len * sizeof(C) + sizeof(C));
			lstrcpyT(m_pName, pName);

			// m_pNameFileMap = (C*)LocalAlloc(LMEM_FIXED, len + sizeof("_FileMap") - 1 + sizeof(char));
			m_pNameFileMap = (C*)LocalAlloc(LMEM_FIXED, 
				len * sizeof(C) +
				lstrlenT(prefFileMap<C>()) * sizeof(C) +
				sizeof(C));
			lstrcpyT(m_pNameFileMap, pName);
			lstrcatT(m_pNameFileMap, prefFileMap<C>());

			// m_pMutexName = (LPSTR)LocalAlloc(LMEM_FIXED, len + sizeof("_Mutex") - 1 + sizeof(char));
			m_pMutexName = (C*)LocalAlloc(LMEM_FIXED,
				len * sizeof(C) +
				lstrlenT(prefMutex<C>()) * sizeof(C) +
				sizeof(C));
			lstrcpyT(m_pMutexName, pName);
			lstrcatT(m_pMutexName, prefMutex<C>());
		}
		void release() {
#ifdef _DEBUG
			assert(initialized_ == 1);
			initialized_ = 0;
#endif
			if (p_)
			{
				AMBIESOFT_VERIFY(UnmapViewOfFile(p_));
				p_ = NULL;
			}

			if (h_)
			{
				AMBIESOFT_VERIFY(CloseHandle(h_));
				h_ = NULL;
			}

			if (m_)
			{
				AMBIESOFT_VERIFY(CloseHandle(m_));
				m_ = NULL;
			}
			AMBIESOFT_VERIFY_ZERO(LocalFree(m_pName));
			AMBIESOFT_VERIFY_ZERO(LocalFree(m_pNameFileMap));
			AMBIESOFT_VERIFY_ZERO(LocalFree(m_pMutexName));
		}
		static HANDLE CreateMutexT(const char* pMutexName)
		{
			return CreateMutexA(NULL, FALSE, pMutexName);
		}
		static HANDLE CreateMutexT(const wchar_t* pMutexName)
		{
			return CreateMutexW(NULL, FALSE, pMutexName);
		}
		static HANDLE CreateFileMappingT(const char* pName, size_t32 internalSize)
		{
			return CreateFileMappingA(
				INVALID_HANDLE_VALUE,
				NULL,
				PAGE_READWRITE,
				0, internalSize,
				pName);
		}
		static HANDLE CreateFileMappingT(const wchar_t* pName, size_t32 internalSize)
		{
			return CreateFileMappingW(
				INVALID_HANDLE_VALUE,
				NULL,
				PAGE_READWRITE,
				0, internalSize,
				pName);
		}
		bool ensure() const {
			if (!m_pMutexName)
				return false;

			if (!m_)
			{
				m_ = CreateMutexT(m_pMutexName);
				assert(m_);
			}

			bool first = false;
			if (!h_)
			{
				internalsize();
				h_ = CreateFileMappingT(m_pNameFileMap, internalsize());
				assert(h_);
				if (h_ && GetLastError() != ERROR_ALREADY_EXISTS)
				{
					first = true;
				}
			}
			if (!p_)
			{
				p_ = MapViewOfFile(
					h_,
					FILE_MAP_READ | FILE_MAP_WRITE,
					0, 0,
					internalsize());
				assert(p_);
			}

			if (first)
			{
				Locker l(this);
				if (first)
				{
					memset(p_, 0, internalsize());
					first = false;
				}
			}
			return true;
		}

		//T get() {
		//}


		virtual void set(const T& t) {
			if (!ensure())
				return;
			Locker l(this);
			memcpy(p_, &t, size());
		}

		virtual size_t32 size() const {
			return sizeof(T);
		}
		virtual size_t32 internalsize() const {
			return size();
		}

		class Locker {
		public:
			HANDLE m_;
			Locker(const CSessionGlobalMemory* pThis) {
				m_ = pThis->m_;
				WaitForSingleObject(m_, INFINITE);
			}
			~Locker() {
				AMBIESOFT_VERIFY(ReleaseMutex(m_));
			}
		};
		friend class Locker;

	protected:
		mutable HANDLE h_ = NULL;
		mutable HANDLE m_ = NULL;

		C* m_pName = NULL;
		C* m_pNameFileMap = NULL;
		C* m_pMutexName = NULL;
		mutable void* p_ = NULL;
		// prohibitted, use NTS
		// T* operator &() {}
	};

	// Non Thread Safe
	template<class T, class C=char>
	class CSessionGlobalMemoryNTS : CSessionGlobalMemory<T,C>
	{
		typedef typename CSessionGlobalMemory<T,C> Parent;
	public:
		explicit CSessionGlobalMemoryNTS(const C* pName) : CSessionGlobalMemory<T,C>(pName) {

		}
		T* operator &() {
			Parent::ensure();
			return (T*)Parent::p_;
		}
	};


	class CDynamicSessionGlobalMemory : public CSessionGlobalMemory<size_t32,char>
	{
	private:
		size_t32 size_;
	public:
		// creator
		explicit CDynamicSessionGlobalMemory(const char* pName, size_t32 size) : CSessionGlobalMemory<size_t32,char>(pName) {
			size_ = size;
		}

		// user
		explicit CDynamicSessionGlobalMemory(const char* pName) : CSessionGlobalMemory<size_t32,char>(pName) {
			size_ = -1;
		}

		~CDynamicSessionGlobalMemory() {

		}
		size_t32 size() const {
			if (size_ == -1)
			{
				CDynamicSessionGlobalMemory tmp(m_pName, sizeof(size_));
				tmp.internalget(const_cast<size_t32*>(&size_));
			}
			return size_;
		}
		virtual size_t32 internalsize() const {
			return sizeof(size_) + size();
		}
		void get(unsigned char* p) {
			if (!ensure())
				return;
			Locker l(this);
			memcpy(p, (unsigned char*)(p_)+sizeof(size_), size());
		}
		void internalget(size_t32* ps) {
			if (!ensure())
			{
				*ps = 0;
				return;
			}
			Locker l(this);
			*ps = *(size_t32*)p_;
		}

		void set(const unsigned char* p) {
			// user can not set before get
			assert(size_ != -1);
			if (!ensure())
				return;
			Locker l(this);
			*(size_t32*)p_ = size();
			memcpy((unsigned char*)(p_)+sizeof(size_), p, size());
		}
	};
}  // namespace

#undef size_t