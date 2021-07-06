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

#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

#include <vector>
#include <string>
#include <cassert>

// #include "os_traits.h"
#include "stdosd/CBool.h"
#include "stdosd/stdosd.h"

#ifndef _countof
#define _countof(t) (sizeof(t)/sizeof(t[0]))
#endif


namespace Ambiesoft {


	template<class E>
	class CCommandLineStringBase
	{
		typedef CCommandLineStringBase<E> CommandLineStringType;
		typedef std::basic_string<E> myS;
		typedef std::vector<std::basic_string<E> > myVS;
		typedef typename std::vector<std::basic_string<E> >::iterator myVSIterator;
		typedef std::char_traits<E> myTr;

		const E* p_;
		std::vector<size_t> offsets_;

		std::vector<std::basic_string<E> > args_;
		Ambiesoft::stdosd::Cbool dirty_; // = false;



		template<class T>
		static bool myContainSpace(const T& s)
		{
			for (size_t i = 0; i < s.size(); ++i)
			{
				if (stdosd::stdIsSpace(s[i]))
					return true;
			}
			return false;
		}

		template<class T>
		static bool myContainDQ(const T& s)
		{
			for (size_t i = 0; i < s.size(); ++i)
			{
				if (myIsDQ(s[i]))
					return true;
			}
			return false;
		}
		static std::string myEncloseDQ(const std::string& s)
		{
			return "\"" + s + "\"";
		}
		static std::wstring myEncloseDQ(const std::wstring& s)
		{
			return L"\"" + s + L"\"";
		}

		static std::string myAddSpace(const std::string& s)
		{
			return s + " ";
		}
		static std::wstring myAddSpace(const std::wstring& s)
		{
			return s + L" ";
		}

		template<class T>
		static T myAddDQIfNeccesary(const T& s)
		{
			if (myContainDQ(s))
				return s;

			if (!myContainSpace(s))
				return s;

			return myEncloseDQ(s);
		}

		template<class T>
		static T myAddDQIfNeccesaryWithConsideringEqual(const T& s)
		{
			if (myContainDQ(s))
				return s;

			{
				T prevEq;
				T afterEq;
				const typename T::value_type* p = s.c_str();
				for (; *p; ++p)
				{
					if (stdosd::stdIsSpace(*p))
						break;
					prevEq += *p;
					if (myIsLead(*p))
					{
						++p;
						prevEq += *p;
					}

					if (myIsEqual(*p))
					{
						afterEq = p + 1;
						break;
					}
				}
				if (!afterEq.empty())
				{
					return prevEq + myAddDQIfNeccesary(afterEq);
				}
			}
			if (!myContainSpace(s))
				return s;

			return myEncloseDQ(s);
		}

		template<typename T>
		static const T* mySkipWS(const T* p)
		{
			while (*p && stdosd::stdIsSpace(*p))
				++p;
			return p;
		}

		template<typename T>
		static void myClearS(T& s)
		{
			s = T();
		}


		static const char* myNextP(const char* p)
		{
			// not assume double null
			if (*p == 0)
				return p;
#ifdef _WIN32
			return CharNextA(p);
#else
			return p + 1;
#endif
		}
		static const wchar_t* myNextP(const wchar_t* p)
		{
			// not assume double null
			if (*p == 0)
				return p;

			++p;
			return p;
		}

		static bool myIsDQ(char c)
		{
			return c == '"';
		}
		static bool myIsDQ(wchar_t c)
		{
			return c == L'"';
		}

		static bool myIsEqual(char c)
		{
			return c == '=';
		}
		static bool myIsEqual(wchar_t c)
		{
			return c == L'=';
		}

		static bool myIsESC(char c)
		{
			return c == '\\';
		}
		static bool myIsESC(wchar_t c)
		{
			return c == L'\\';
		}


		static void GetModuleFileNameT(char* p)
		{
			GetModuleFileNameA(NULL, p, MAX_PATH);
		}
		static void GetModuleFileNameT(wchar_t* p)
		{
			GetModuleFileNameW(NULL, p, MAX_PATH);
		}

		static bool myIsLead(char c)
		{
#ifdef _WIN32
			return !!IsDBCSLeadByte(c);
#else
			(void)c;
			return false;
#endif
		}
		static bool myIsLead(wchar_t c)
		{
			(void)c;
			return false;
		}


		static void myAddChar(std::string& now, const char*& p)
		{
			now += *p;
			if (myIsLead(*p))
			{
				++p;
				now += *p;
			}
		}
		static void myAddChar(std::wstring& now, const wchar_t*& p)
		{
			now += *p;
		}

		static char* GetCommandLineT(char*)
		{
			return GetCommandLineA();
		}
		static wchar_t* GetCommandLineT(wchar_t*)
		{
			return GetCommandLineW();
		}


		void parseV2(const E* p, const E* pCommandLine)
		{
			E prev = 0;
			E next = 0;
			bool inDQGroup = false;
			const E* afterClosedp = nullptr;
			const E* pStart = p;
			const E* pLastPushed = nullptr;
			std::basic_string<E> now;
			for (; *p; prev = *p, p = myNextP(p), next = *myNextP(p))
			{
				if (!inDQGroup)
				{
					if (myIsDQ(*p))
					{
						// DQ not in DQ
						if (stdosd::stdIsSpace(prev) || prev == 0)
						{
							inDQGroup = true;
							continue;
						}
						else
						{
							if (myIsDQ(prev) && (!afterClosedp || myNextP(afterClosedp) != p))
							{
								// after closer
								now += prev;
								afterClosedp = p;
								inDQGroup = true;
								continue;
							}
							inDQGroup = true;
							continue;
						}
					}
					else if (stdosd::stdIsSpace(*p))
					{
						// SPACE not in DQ
						if (!now.empty())
						{
							args_.push_back(now);
							offsets_.push_back(pStart - pCommandLine);
							pLastPushed = pStart;
							myClearS(now);
						}
						pStart = p;
						continue;
					}
					else
					{
						// not DQ not SPACE in NDQ
						myAddChar(now, p);
						continue;
					}
					assert(false);
				}
				else
				{
					// in DQGroup
					if (myIsESC(*p) && myIsDQ(next))
					{
						now += next;
						p++;
						continue;
					}
					if (myIsDQ(*p))
					{
						if (now.empty() && (stdosd::stdIsSpace(next) || next == 0))
						{
							offsets_.push_back(pStart - pCommandLine);
							args_.push_back(now);
							pLastPushed = pStart;
							myClearS(now);
						}
						inDQGroup = false;
						continue;
					}
					myAddChar(now, p);
					continue;
				}
			} // for(p)

			if (!now.empty())
			{
				offsets_.push_back(pStart - pCommandLine);
				args_.push_back(now);
			}
			else if (inDQGroup)
			{
				if (pStart != pLastPushed)
				{
					offsets_.push_back(pStart - pCommandLine);
					args_.push_back(now);
				}
			}
		}
		void init(const E* pCommandLine)
		{
			size_t clLen = myTr::length(pCommandLine);
			p_ = new E[clLen+1];
			myTr::copy((E*)p_, pCommandLine, clLen+1);

			if (!pCommandLine || !*pCommandLine)
				return;

			const E* p = mySkipWS(pCommandLine);
			if (!*p)
				return;

			parseV2(p, pCommandLine);
		}
		

	public:
		CCommandLineStringBase()
		{
            init(GetCommandLineT((E*)NULL));
		}
		CCommandLineStringBase(const E* pCommandLine)
		{
			if (!*pCommandLine)
			{
				E e[MAX_PATH];
				GetModuleFileNameT(e);
				init(e);
			}
			else
			{
				init(pCommandLine);
			}
		}
		CCommandLineStringBase(const myS& s) :
			CCommandLineStringBase(s.c_str()) {}

		CCommandLineStringBase(int argc, const E* const * const argv) {
			myS s;
			for (int i = 0; i < argc; ++i)
			{
				// args_.push_back(argv[i]);
				if (myContainSpace<std::basic_string<E>>(argv[i]))
					s += myEncloseDQ(argv[i]);
				else
					s += argv[i];

				if ((i + 1) != argc)
					s = myAddSpace(s);
			}
			init(s.c_str());
		}

		~CCommandLineStringBase()
		{
			delete[] (E*)p_;
		}

		bool operator==(const CommandLineStringType& that) const {
			if (this == &that)
				return true;

			return
				offsets_ == that.offsets_ &&
				args_ == that.args_ &&
				myTr::length(p_) == myTr::length(that.p_) &&
				myTr::compare(p_, that.p_, myTr::length(p_))==0;
				//myStrEqual(p_, that.p_);
		}
		bool operator!=(const CommandLineStringType& that) const {
			return !(*this == that);
		}
		bool SyntaxEqual(const CommandLineStringType& that) const {
			return args_ == that.args_;
		}
		int getIndex(const E* p) const
		{
			for (int i = 0; i < (int)args_.size(); ++i)
			{
				if (args_[i] == p)
					return i;
			}
			return -1;
		}
		size_t getCount() const {
			return args_.size();
		}
		const myS& getArg(size_t i) const {
			return args_[i];
		}
		const myS& operator[](size_t i) const {
			return args_[i];
		}
		void remove(size_t i, size_t size = 1) {
			assert(offsets_.size() == args_.size());
			while (size > 0)
			{
				args_.erase(args_.begin() + i);
				offsets_.erase(offsets_.begin() + i);
				--size;
			}
			dirty_ = true;
		}
		myS subString(size_t index) const
		{
			if(dirty_)
			{
				myS ret;
				for (size_t i =0; i<args_.size();++i)
				{
					ret += myAddDQIfNeccesaryWithConsideringEqual(args_[i]);
					if ((i + 1) != args_.size())
						ret = myAddSpace(ret);
				}
				return ret;
			}
			else
			{
				if (offsets_.size() <= index)
					return std::basic_string<E>();

				size_t ofs = offsets_[index];
				std::basic_string<E> ret(&p_[ofs]);
				const E* pTrimming = ret.c_str();
				ret = mySkipWS(pTrimming);
				return ret;
			}
		}
		myS toString() const {
			return subString(0);
		}

		static E** getCommandLineArg(const E* pCL, int* argc)
		{
			CCommandLineStringBase<E> me(pCL);

			*argc = 0;
			E** ppRetReturn = (E**)LocalAlloc(0, (me.args_.size() + 1) * sizeof(E*));
			E** ppRet = ppRetReturn;
			int i = 0;
			for (myVSIterator it = me.args_.begin();
				it != me.args_.end(); ++it)
			{
				//if (it->empty())
				//	continue;

				*ppRet = (E*)LocalAlloc(0, (it->size() + 1) * sizeof(E));
				memcpy(*ppRet, it->c_str(), (it->size()+1)*sizeof(E));
				++ppRet;
				++i;
			}
			*argc = i;
			*ppRet = NULL;
			return ppRetReturn;
		}
		static void freeCommandLineArg(E** argv)
		{
			for (E** pp = argv; *pp; ++pp)
			{
				LocalFree(*pp);
			}
			LocalFree(argv);
		}

		static myS getCommandLine(const std::vector<myS>& args, const E* pSeparator = nullptr)
		{
			myS ret;
			for (size_t i = 0; i < args.size();++i)
			{
				myS t = args[i];
				if (t.empty())
					continue;
				if (myContainSpace(t))
				{
					if (!myIsDQ(t[0]))
					{
						t = myEncloseDQ(t);
					}
				}
				ret += t;
				if ((i + 1) != args.size())
				{
					if (pSeparator)
						ret += pSeparator;
					else
						ret = myAddSpace(ret);
				}
			}
			return ret;
		}

		static void ExplodeExeAndArg(const E* pE, myS& exe, myS& arg)
		{
			myS all;
			if (pE)
				all = pE;
			else
				all = GetCommandLine();

			CCommandLineStringBase<E> tcl(all.c_str());
			exe = tcl.getArg(0);
			arg = tcl.subString(1);
		}
	};

#ifdef UNICODE
	typedef CCommandLineStringBase<wchar_t> CCommandLineString;
#else
	typedef CCommandLineStringBase<char> CCommandLineString;
#endif

	typedef CCommandLineStringBase<wchar_t> CCommandLineStringW;
	typedef CCommandLineStringBase<char> CCommandLineStringA;

}
