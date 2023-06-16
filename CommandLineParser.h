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
#ifdef UNICODE
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#endif // UNICODE
#endif // _WIN32

#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include "DebugNew.h"

#include "UrlEncode.h"
#include "UTF16toUTF8.h"
#include "CommandLineString.h"

#include "stdosd/stdosd.h"

namespace Ambiesoft {

	// Our root exception class
	class CCommandLineParserException : public std::exception
	{
	public:
		virtual std::wstring wwhat() const = 0;
	};

	template <class T, class I>
	class illegal_value_type_error;
	template <class I>
	class illegal_value_type_error<std::string, I> : public CCommandLineParserException
	{
		std::string s_;
		mutable std::string buff_;
	public:
		explicit illegal_value_type_error(const std::string& s)
			: s_(s){}

		virtual const char* what() const
#if defined(__MINGW32__) || defined(__GNUC__)
			_GLIBCXX_USE_NOEXCEPT
#endif
		{
			std::stringstream ss;
			ss << s_ << " is not " << typeid(I).name();
			buff_ = ss.str();
			return buff_.c_str();
		}
	};
	template <class I>
	class illegal_value_type_error<std::wstring,I> : public CCommandLineParserException
	{
		std::wstring s_;
		mutable std::string buff_;

		std::wstring getErrorString() const {
			std::wstringstream ss;
			ss << s_ << L" is not " << typeid(I).name();
			return ss.str();
		}
	public:
		explicit illegal_value_type_error(const std::wstring& s)
			: s_(s) {}


		virtual const char* what() const
#if defined(__MINGW32__) || defined(__GNUC__)
			_GLIBCXX_USE_NOEXCEPT
#endif
		{
			buff_ = toStdAcpString(getErrorString());
			return buff_.c_str();
		}
		virtual std::wstring wwhat() const {
			return getErrorString();
		}
	};









	template <class T>
	class no_value_error;
	template <>
	class no_value_error<std::string> : public CCommandLineParserException
	{
		std::string op_;
		mutable std::string buff_;
	public:
		explicit no_value_error(const std::string& op)
			: op_(op) {}

		virtual const char* what() const
#if defined(__MINGW32__) || defined(__GNUC__)
			_GLIBCXX_USE_NOEXCEPT
#endif
		{
			std::stringstream ss;
			ss << "option '" << op_ << "' must have value.";
			buff_ = ss.str();
			return buff_.c_str();
		}
	};
	template <>
	class no_value_error<std::wstring> : public CCommandLineParserException
	{
		std::wstring op_;
		mutable std::string buff_;

		std::wstring getErrorString() const {
			std::wstringstream ss;
			ss << "option '" << op_ << "' must have value.";
			return ss.str();
		}
	public:
		explicit no_value_error(const std::wstring& op)
			: op_(op) {}


		virtual const char* what() const
#if defined(__MINGW32__) || defined(__GNUC__)
			_GLIBCXX_USE_NOEXCEPT
#endif
		{
			buff_ = toStdAcpString(getErrorString());
			return buff_.c_str();
		}
		virtual std::wstring wwhat() const {
			return getErrorString();
		}
	};



















	inline static unsigned int AtoI(const std::string& s)
	{
		return atoi(s.c_str());
	}
	inline static unsigned int AtoI(const std::wstring& s)
	{
		return _wtoi(s.c_str());
	}
	template<class I>
	inline static std::string ItoA(const I& i, const std::string& s)
	{
		return std::to_string(i);
	}
	template<class I>
	inline static std::wstring ItoA(const I& i, const std::wstring& s)
	{
		return std::to_wstring(i);
	}

	inline static unsigned long long AtoI64(const std::string& s)
	{
		return _atoi64(s.c_str());
	}
	inline static unsigned long long AtoI64(const std::wstring& s)
	{
		return _wtoi64(s.c_str());
	}


	inline static bool StringCompare(
		const std::string& left,
		const std::string& right,
		bool ignoreCase = false)
	{
		if (ignoreCase)
			return _stricmp(left.c_str(), right.c_str()) == 0;
		else
			return _stricmp(left.c_str(), right.c_str()) == 0;
	}
	inline static bool StringCompare(
		const std::wstring& left,
		const std::wstring& right,
		bool ignoreCase = false)
	{
		if (ignoreCase)
			return _wcsicmp(left.c_str(), right.c_str()) == 0;
		else
			return wcscmp(left.c_str(), right.c_str()) == 0;
	}


	inline static void addkaigyo(std::string& s)
	{
		s += "\r\n";
	}
	inline static void addkaigyo(std::wstring& s)
	{
		s += L"\r\n";
	}
	inline static void addprevkaigyo(std::string& s)
	{
		s = "\r\n" + s;
	}
	inline static void addprevkaigyo(std::wstring& s)
	{
		s = L"\r\n" + s;
	}

	inline static void addspace(std::string& s)
	{
		s += " ";
	}
	inline static void addspace(std::wstring& s)
	{
		s += L" ";
	}

	inline static std::string getFileName(std::string program)
	{
		const size_t last_slash_idx = program.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
		{
			program.erase(0, last_slash_idx + 1);
		}

		return program;
	}
	inline static std::wstring getFileName(std::wstring program)
	{
		const size_t last_slash_idx = program.find_last_of(L"\\/");
		if (std::wstring::npos != last_slash_idx)
		{
			program.erase(0, last_slash_idx + 1);
		}

		return program;
	}

	inline static bool hasSpace(const std::string& s)
	{
		for (char c : s)
		{
			if (isspace(c))
				return true;
		}
		return false;
	}
	inline static bool hasSpace(const std::wstring& s)
	{
		for (wchar_t c : s)
		{
			if (iswspace(c))
				return true;
		}
		return false;
	}

	inline static std::string dq(const std::string& s)
	{
		if (!hasSpace(s))
			return s;

		return "\"" + s + "\"";
	}
	inline static std::wstring dq(const std::wstring& s)
	{
		if (!hasSpace(s))
			return s;

		return L"\"" + s + L"\"";
	}


	enum class ExactCount : short
	{
		Exact_1 = 1,
		Exact_2,
		Exact_3,
		Exact_4,
		Exact_5,
		Exact_6,
		Exact_7,
	};
		
	// 000000001(1) = only zero
	// 000000010(2) = only one
	// 000000011(3) = zero or one
	// 000000100(4) = only two
	// 000000101(5) = zero or two
	// 000000110(6) = one or two
	// 000000111(7) = zero, one or two
	// 000001000(8) = only tree
	enum class ArgCount : unsigned int
	{
		ArgCount_Uninitialized,
		ArgCount_Zero,
		ArgCount_One,
		ArgCount_ZeroOrOne,
		ArgCount_Two,
		ArgCount_ZeroOrTwo,
		ArgCount_OneOrTwo,
		ArgCount_ZeroOrOneOrTwo,
		ArgCount_Three,

		ArgCount_ZeroToInfinite,
		ArgCount_OneToInfinite,
		ArgCount_TwoToInfinite,
	};

	enum CaseFlags
	{
		CaseFlags_Default,
		CaseFlags_Sensitive,
		CaseFlags_Insensitive,
	};
	enum ArgEncodingFlags
	{
		ArgEncodingFlags_Default,
		ArgEncodingFlags_UTF8UrlEncode,
	};

	template <class MyS_, class myOptionType> 
	class BasicCommandLineParser;
	
	template<class myStringType> 
	class BasicOption
	{
        typedef BasicOption<myStringType> MyT_;
        typedef typename myStringType::traits_type::char_type Elem;
        typedef myStringType MyS_;

		class UserTargetBase
		{
		public:
			virtual void setTrue() = 0;
			virtual void setParsedValue(const MyS_& mys, const bool bStrict) = 0;
			virtual void setTarget(void*) = 0;
			virtual ~UserTargetBase() {}
		};
		template<class T>
		class UserTarget : public UserTargetBase
		{
		public:

		private:
			static void setValue(bool* pB, const MyS_& mys, const bool bSrict)
			{
				if (
					StringCompare(mys, stdosd::stdLiterals<Elem>::num0String()) ||
					StringCompare(mys, stdosd::stdLiterals<Elem>::noString(), true) ||
					StringCompare(mys, stdosd::stdLiterals<Elem>::offString(), true) ||
					StringCompare(mys, stdosd::stdLiterals<Elem>::falseString(), true)
					)
				{
					*pB = false;
					return;
				}
				if (
					StringCompare(mys, stdosd::stdLiterals<Elem>::num1String()) ||
					StringCompare(mys, stdosd::stdLiterals<Elem>::yesString(), true) ||
					StringCompare(mys, stdosd::stdLiterals<Elem>::onString(), true) ||
					StringCompare(mys, stdosd::stdLiterals<Elem>::trueString(), true)
					)
				{
					*pB = true;
					return;
				}
				if (bSrict)
				{
					throw illegal_value_type_error<MyS_, bool>(mys);
				}
				// assume true if not false
				*pB = true;
			}
			static void setValue(int* pI, const MyS_& mys, const bool bStrict)
			{
				int tmp = AtoI(mys);
				if (bStrict) {
					if (ItoA(tmp, mys) != mys) {
						throw illegal_value_type_error<MyS_, int>(mys);
					}
				}
				*pI = tmp;
			}
			static void setValue(long long* pLL_, const MyS_& mys, const bool bStrict)
			{
				long long tmp = AtoI64(mys);
				if (bStrict) {
					if (ItoA(tmp, mys) != mys) {
						throw illegal_value_type_error<MyS_, long long>(mys);
					}
				}
				*pLL_ = tmp;
			}
			static void setValue(MyS_* pMys_, const MyS_& mys, const bool bSrict)
			{
				*pMys_ = mys;
			}
			static void setValue(std::vector<MyS_>* pVmys_, const MyS_& mys, const bool bSrict)
			{
				pVmys_->push_back(mys);
			}
		
			template<class T>
			void setHadOption(T* pT) {}
			template<>
			void setHadOption<bool>(bool* pT) {
				*pT = true;
			}
		private:
			T* pUserRawTarget_ = nullptr;
		public:
			void setTarget(void* p) override
			{
				assert(!pUserRawTarget_);
				pUserRawTarget_ = static_cast<T*>(p);
			}
			void setTrue() override
			{
				setHadOption(pUserRawTarget_);
			}
			void setParsedValue(const MyS_& mys, const bool bStrict) override
			{
				setValue(pUserRawTarget_, mys, bStrict);
			}
		};

		std::vector<MyS_> options_;

		ArgCount argcountflag_;
		std::vector<MyS_> values_;
		bool hadOption_;
		bool parsed_;
		CaseFlags case_;
		ArgEncodingFlags encoding_;
		std::unique_ptr<UserTargetBase> userTarget_;
		MyS_ helpString_;

		template<class TargetType>
		void setTarget(TargetType* pT)
		{
			assert(!userTarget_);
			userTarget_.reset(new UserTarget<TargetType>());
			userTarget_->setTarget(pT);
		}

		void AddValue(const MyS_& value, const bool bStrict)
		{
			setHadOption();
			if(userTarget_)
				userTarget_->setParsedValue(encoding_ == ArgEncodingFlags_UTF8UrlEncode ?
					UrlDecodeStd<MyS_>(value.c_str()) : value, bStrict);

			values_.push_back(value);
		}
		void setHadOption()
		{
			if (argcountflag_ == ArgCount::ArgCount_Zero)
			{
				if(userTarget_)
					userTarget_->setTrue();
			}
			hadOption_ = true;
		}
		bool isMatchOption(const MyS_& option, bool ignoreCase) const
		{
			if (!ignoreCase)
			{
                typename std::vector< MyS_ >::const_iterator cIter = find(options_.begin(), options_.end(), option);
				return cIter != options_.end();
			}
			
            for (typename std::vector< MyS_ >::const_iterator cIter = options_.begin();
				cIter != options_.end();
				++cIter)
			{
				if (StringCompare(*cIter, option,true))
					return true;
			}
			return false;
		}
		bool isMatchOption(const MyS_& option) const
		{
			return isMatchOption(option, case_ == CaseFlags_Insensitive);
		}

		MyS_ getFirstOption() const
		{
			return options_[0];
		}

		void setArgFlag(int exactcount)
		{
			switch (exactcount)
			{
			case 0:
				argcountflag_ = ArgCount::ArgCount_Zero;
				break;
			case 1:
				argcountflag_ = ArgCount::ArgCount_One;
				break;
			case 2:
				argcountflag_ = ArgCount::ArgCount_Two;
				break;
			case 3:
				argcountflag_ = ArgCount::ArgCount_Three;
				break;
			default:
				argcountflag_ = ArgCount::ArgCount_ZeroToInfinite;
				break;
			}
		}

		void setParsed()
		{
			parsed_ = true;
		}

		void init()
		{
			hadOption_ = false;
			parsed_ = false;
			argcountflag_ = ArgCount::ArgCount_Uninitialized;
			case_ = CaseFlags_Default;
			// pTarget_ = NULL;
			encoding_ = ArgEncodingFlags_Default;
		}
		//void copy(const MyT_& that)
		//{
		//	options_ = that.options_;
		//	argcountflag_ = that.argcountflag_;
		//	values_ = that.values_;
		//	hadOption_ = that.hadOption_;
		//	parsed_ = that.parsed_;
		//	case_ = that.case_;
		//	encoding_ = that.encoding_;
		//	userTarget_.reset(
		//		new UserTarget<decltype(*((UserTarget*)(that.userTarget_.get()))->pUserRawTarget_)>());

		//	helpString_ = that.helpString_;
		//}
	public:
		//BasicOption(const MyT_& that)
		//{
		//	if (this == &that)
		//		return;

		//	copy(that);
		//}
		//MyT_& operator=(const MyT_& that)
		//{
		//	if (this == &that)
		//		return *this;

		//	copy(that);
		//	return *this;
		//}
		BasicOption()
		{
			init();
			options_.push_back(MyS_());
			argcountflag_ = ArgCount::ArgCount_ZeroToInfinite;
		}
		//BasicOption(MyS_ option, ArgCount acf) 
		//{
		//	init();
		//	options_.push_back(option);
		//	argcountflag_ = acf;
		//}
		BasicOption(
			const MyS_& option,
			ExactCount exactcount,
			ArgEncodingFlags arf = ArgEncodingFlags_Default,
			const MyS_& helpstring = MyS_())
		{
			init();
			options_.push_back(option);
			encoding_ = arf;
			helpString_ = helpstring;
			setArgFlag((int)exactcount);
		}
		BasicOption(
			const MyS_& option,
			ArgCount acf,
			ArgEncodingFlags arf = ArgEncodingFlags_Default,
			const MyS_& helpstring = MyS_())
		{
			init();
			options_.push_back(option);
			encoding_ = arf;
			helpString_ = helpstring;
			argcountflag_ = acf;
		}

		//template<class InputIterator>
		//BasicOption(InputIterator first, InputIterator last, const int exactcount)
		//{
		//	init();
		//	while (first != last)
		//	{
		//		options_.push_back(*first);
		//		++first;
		//	}

		//	setArgFlag(exactcount);
		//}
		
		//BasicOption(MyS_ option1, MyS_ option2, ArgCount acf)
		//{
		//	init();
		//	options_.push_back(option1);
		//	options_.push_back(option2);
		//	argcountflag_ = acf;
		//}
                

		BasicOption(MyS_ option1, MyS_ option2, const int exactcount)
		{
			init();
			options_.push_back(option1);
			options_.push_back(option2);

			setArgFlag(exactcount);
		}
		BasicOption(const std::vector<MyS_>& optionStrings, const int exactcount) 
		{
			init();
			for(auto&& s : optionStrings)
			{
				options_.emplace_back(s);
			}

			setArgFlag(exactcount);
		}
		

		//BasicOption(const Elem* p1, const Elem* p2, const int exactcount)
		//{
		//	init();
		//	options_.push_back(p1);
		//	options_.push_back(p2);

		//	setArgFlag(exactcount);
		//}
		//BasicOption(std::initializer_list<const Elem*> optionStrings, const int exactcount) :
		//	BasicOption(optionStrings.begin(), optionStrings.end(),
		//		exactcount) {}

		BasicOption(MyS_ option)
		{
			init();
			options_.push_back(option);
			argcountflag_ = ArgCount::ArgCount_Zero;
		}
		//BasicOption(const MyS_& option1, const MyS_& option2)
		//{
		//	init();
		//	options_.push_back(option1);
		//	options_.push_back(option2);
		//	argcountflag_ = ArgCount_Zero;
		//}
		//BasicOption(const MyS_& option1, const MyS_& option2, const MyS_& option3)
		//{
		//	init();
		//	options_.push_back(option1);
		//	options_.push_back(option2);
		//	options_.push_back(option3);
		//	argcountflag_ = ArgCount_Zero;
		//}
		BasicOption(const std::vector<MyS_>& options)
		{
			init();
			options_ = options;
			argcountflag_ = ArgCount::ArgCount_Zero;
		}
		~BasicOption()
		{
			
		}
		friend class BasicCommandLineParser<MyS_, BasicOption<MyS_> >;


		// Command line had the option
		bool hadOption() const
		{
			assert(parsed_);
			return hadOption_;
		}

		// Command line had value of the option
		bool hadValue() const
		{
			assert(parsed_);
			return !values_.empty();
		}

		// Get all values of the option
		MyS_ getValueStrings() const
		{
			assert(parsed_);

			MyS_ ret;
			bool looped = false;
            for (typename std::vector<MyS_>::const_iterator it = values_.begin(); it != values_.end(); ++it)
			{
				if (looped)
				{
					ret += stdosd::stdLiterals<Elem>::spaceString();
				}
				looped = true;
				ret += dq(*it);
			}
			return ret;
		}

		// Get first value of the option
		MyS_ getFirstValue() const
		{
			assert(parsed_);

			MyS_ ret;
			if (values_.empty())
				return ret;
			return values_[0];
		}


		unsigned int getFirstValueAsUInt() const
		{
			assert(parsed_);
			return AtoI(getFirstValue());
		}
		unsigned long long getFirstValueAsUInt64() const
		{
			assert(parsed_);
			return AtoI64(getFirstValue());
		}
		void* getFirstValueAsPointer() const
		{
			assert(parsed_);
			if (sizeof(void*) == sizeof(unsigned int))
				return (void*)getFirstValueAsUInt();
			else
				return (void*)getFirstValueAsUInt64();
		}
		MyS_ getValue(size_t index)
		{
			assert(parsed_);
			return values_[index];
		}
		size_t getValueCount() const
		{
			assert(parsed_);
			return values_.size();
		}
	};


typedef BasicOption<std::wstring> COptionW;
typedef BasicOption<std::string> COptionA;

#ifdef UNICODE
	typedef COptionW COption;
#else
	typedef COptionA COption;
#endif


	


	template <class myStringType, class myOptionType> 
	class BasicCommandLineParser
	{
        typedef myStringType MyS_;
		typedef typename myStringType::traits_type::char_type Elem;
		typedef typename myOptionType::MyS_ MyOS_;

		static MyS_ GetToken(const Elem* p)
		{
			MyS_ ret;
			if (!p || !p[0])
				return ret;

			bool dq = false;
			bool sq = false;

			for (; *p; ++p)
			{
				if (_istspace(*p))
				{
					if (!dq && !sq)
						break;
				}
				else if (*p == TEXT('"'))
				{
					if (!sq)
					{
						dq = true;
						continue;
					}
				}
				else if (*p == TEXT('\''))
				{
					if (!dq)
					{
						sq = true;
						continue;
					}
				}

				ret += *p;
			}
			return ret;
		}



		typedef std::vector<BasicOption<MyS_>*> POPTIONARRAY;
		// typedef std::vector<BasicOption<MyS_> > OPTIONARRAY;

        typedef BasicOption<MyS_> MyO_;
		POPTIONARRAY useroptions_;
		POPTIONARRAY inneroptions_;
		POPTIONARRAY unknowns_;
		bool empty_;
		bool parsed_;
		CaseFlags case_;
		MyS_ description_;
		MyS_ appname_;
		MyS_ program_;

		// throws error if true
		bool strict_ = false;

		MyO_* FindOption(const MyS_& option)
		{
            for (typename POPTIONARRAY::iterator it = useroptions_.begin(); it != useroptions_.end(); ++it)
			{
				if ((*it)->isMatchOption(option))
				{
					return *it;
				}
			}


            for (typename POPTIONARRAY::iterator it = inneroptions_.begin();
				it != inneroptions_.end();
				++it)
			{
				if ((*it)->isMatchOption(option))
				{
					// operator * returns a reference
					return &(*(*it));
				}
			}
			return NULL;
		}

		void init()
		{
			empty_=true;
			parsed_=false;
			case_ = CaseFlags_Default;
		}

		bool isEmptyOptionStrings(const std::vector<MyS_>& options) const
		{
			for (auto&& s : options)
			{
				if (!s.empty())
					return false;
			}
			return true;
		}
		MyS_ toArgSymbol(const MyS_& s) const {
			return stdosd::stdStringUpper(stdosd::stdTrimStart(s, stdosd::stdLiterals<Elem>::OptionPrefix()));
		}
		void processOptionStringHelper(
			const ArgCount& argcount,
			const std::vector<MyS_>& options,
			const MyS_& helpstring,
			MyS_& explain,
			MyS_& usage) const
		{
			// usage
			if (isEmptyOptionStrings(options))
			{
				switch (argcount)
				{
					case ArgCount::ArgCount_One:
						usage += stdosd::stdLiterals<Elem>::commandlineonearg();
						break;
					case ArgCount::ArgCount_ZeroOrOne:
						usage += stdosd::stdLiterals<Elem>::commandlinezerooronearg();
						break;
					case ArgCount::ArgCount_Two:
						usage += stdosd::stdLiterals<Elem>::commandlinetwoargs();
						break;
					case ArgCount::ArgCount_ZeroOrTwo:
						usage += stdosd::stdLiterals<Elem>::commandlinezeroortwoargs();
						break;
					case ArgCount::ArgCount_OneOrTwo:
						usage += stdosd::stdLiterals<Elem>::commandlineoneortwoargs();
						break;
					case ArgCount::ArgCount_ZeroOrOneOrTwo:
						usage += stdosd::stdLiterals<Elem>::commandlinezerooroneortwoargs();
						break;
					case ArgCount::ArgCount_ZeroToInfinite:
						usage += stdosd::stdLiterals<Elem>::commandlinezerotoinfiniteargs();
						break;
					case ArgCount::ArgCount_OneToInfinite:
						usage += stdosd::stdLiterals<Elem>::commandlineonetoinfiniteargs();
						break;
					case ArgCount::ArgCount_TwoToInfinite:
						usage += stdosd::stdLiterals<Elem>::commandlinetwotoinfiniteargs();
						break;
					default:
						assert(false);
						break;
				}
			}
			else
			{
				if (argcount == ArgCount::ArgCount_Zero)
				{
					usage += stdosd::stdLiterals<Elem>::NSquareBlacketBegin;
					stdosd::IsBetweenLoop isBetween;
					for (auto&& s : options)
					{
						if (isBetween)
							usage += stdosd::stdLiterals<Elem>::NVerticalBar;
						usage += s;
					}
					usage += stdosd::stdLiterals<Elem>::NSquareBlacketEnd;
					usage += stdosd::stdLiterals<Elem>::NSpace;
				}
				else if (argcount == ArgCount::ArgCount_One)
				{
					usage += stdosd::stdLiterals<Elem>::NSquareBlacketBegin;
					MyS_ argSymbol;
					stdosd::IsBetweenLoop isBetween;
					for (auto&& s : options)
					{
						if (isBetween)
							usage += stdosd::stdLiterals<Elem>::NVerticalBar;
						if (toArgSymbol(s).size() > argSymbol.size())
							argSymbol = toArgSymbol(s);
						usage += s;
					}
					usage += stdosd::stdLiterals<Elem>::NSpace;
					usage += argSymbol;
					usage += stdosd::stdLiterals<Elem>::NSquareBlacketEnd;
					usage += stdosd::stdLiterals<Elem>::NSpace;
				}
			}
			
			

			// explain
			if (isEmptyOptionStrings(options))
				explain += stdosd::stdLiterals<Elem>::Arg();
			else
			{
				explain += toOptionString(options);
			}
			addkaigyo(explain);

			if (!helpstring.empty())
			{
				// addkaigyo(explain);
				addspace(explain);
				addspace(explain);
				explain += helpstring;
				addkaigyo(explain);
			}
			addkaigyo(explain);
		}
		static MyS_ toOptionString(const std::vector<MyS_>& vs) {
			MyS_ rets;
			for (size_t i = 0; i < vs.size(); ++i) {
				rets += vs[i];
				if ((i + 1) < vs.size())
					rets += stdosd::stdLiterals<Elem>::commandlineseparator();
			}
			return rets;
		}
	public:
		BasicCommandLineParser(CaseFlags kase, const MyS_& description = MyS_(), const MyS_& appname = MyS_())
		{
			init();
			case_ = kase;
			description_ = description;
			appname_ = appname;
		}
		BasicCommandLineParser(const MyS_& description, const MyS_& appname = MyS_())
		{
			init();
			description_ = description;
			appname_ = appname;
		}
		BasicCommandLineParser()
		{
			init();
		}
		~BasicCommandLineParser()
		{
			for (auto&& op : inneroptions_)
				delete op;
			for (auto&& op : unknowns_)
				delete op;
		}
		
		// Call after the instanciation
		// if 'strict_' is true, Pasre() will throw CCommandLineParserException
		void setStrict() {
			strict_ = true;
		}

		MyS_ getHelpMessage() const {
			return getHelpMessage(std::vector<const Elem*>());
		}
		MyS_ getHelpMessage(const Elem* exceptOption) const {
			std::vector<const Elem*> v = { exceptOption };
			return getHelpMessage(v);
		}
		MyS_ getHelpMessage(const std::vector<const Elem*>& exceptOptions) const {
			MyS_ appname;
			MyS_ description;
			MyS_ explain;
			MyS_ usage;

			if (!appname_.empty())
			{
				appname += appname_;
				addkaigyo(appname);
			}
			if (!description_.empty())
			{
				description += description_;
				addkaigyo(description);
				addkaigyo(description);
				if (!appname.empty())
				{
					MyS_ t;
					addspace(t);
					t += description;
					description = std::move(t);
				}
			}

			MyS_ program;
			if (program_.empty())
				program = stdosd::stdLiterals<Elem>::program();
			else
			{
				program = getFileName(program_);
			}
			usage += program;
			addspace(usage);
			
			for (size_t i = 0; i < inneroptions_.size(); ++i)
			{
				ArgCount argcount = ArgCount::ArgCount_Uninitialized;
				std::vector<MyS_> options;
				MyS_ helpString;
				bool skip = false;
				for (size_t j = 0; j < inneroptions_[i]->options_.size(); ++j)
				{
					for (auto&& exceptOption : exceptOptions)
					{
						if (exceptOption && exceptOption == inneroptions_[i]->options_[j]) {
							skip = true;
							break;
						}
					}
					assert(argcount == ArgCount::ArgCount_Uninitialized || argcount == inneroptions_[i]->argcountflag_);
					argcount = inneroptions_[i]->argcountflag_;

					assert(helpString.empty() || helpString == inneroptions_[i]->helpString_);
					helpString = inneroptions_[i]->helpString_;

					options.emplace_back(inneroptions_[i]->options_[j]);
				}
				if (!skip && inneroptions_[i]->options_.size() != 0)
				{
					processOptionStringHelper(
						argcount,
						options,
						helpString,
						explain,
						usage);
				}
			}
			for (size_t i = 0; i < useroptions_.size(); ++i)
			{
				bool skip = false;
				for (size_t j = 0; j < useroptions_[i]->options_.size(); ++j) {
					for (auto&& exceptOption : exceptOptions)
					{
						if (exceptOption && exceptOption == useroptions_[i]->options_[j]) {
							skip = true;
							break;
						}
					}
				}
				if (skip)
					continue;
				processOptionStringHelper(
					useroptions_[i]->argcountflag_,
					useroptions_[i]->options_,
					useroptions_[i]->helpString_,
					explain,
					usage);
			}

			addkaigyo(usage);
			addkaigyo(usage);
			return appname + description + usage + explain;
		}
		bool isEmpty() const
		{
			return empty_;
		}
		MyS_ getUnknowOptionStrings() const
		{
			assert(parsed_);

			MyS_ ret;
			for (typename POPTIONARRAY::const_iterator it = unknowns_.begin(); it != unknowns_.end(); ++it)
			{
				if (!ret.empty())
					addspace(ret);
				ret += (*it)->getFirstOption();
				if ((*it)->hadValue())
				{
					addspace(ret);
					ret += (*it)->getValueStrings();
				}
			}
			return ret;
		}
		MyS_ getFirstUnknowOptionString() const
		{
			assert(parsed_);
			if (unknowns_.empty())
				return MyS_();
			return unknowns_[0]->getFirstOption();
		}
		bool hadUnknownOption() const
		{
			assert(parsed_);
			return !unknowns_.empty();
		}
		void AddOption(MyO_* cli)
		{
			if(cli->case_==CaseFlags_Default)
				cli->case_=case_;

			assert(!parsed_);

			check(cli);

			useroptions_.push_back(cli);
		}

#ifdef _DEBUG
		void check(MyO_* cli)
		{
			for (typename POPTIONARRAY::const_iterator it = useroptions_.begin();
				it != useroptions_.end();
				++it)
			{
				// check whether same option is added.
				assert((*it) != cli);

				// check same option string is added
				for(typename std::vector< MyS_ >::const_iterator cIter=cli->options_.begin();
					cIter != cli->options_.end();
					++cIter)
				{
					assert(!( (*it)->isMatchOption(cIter->c_str(), 
						(*it)->case_ == CaseFlags_Insensitive || cli->case_ == CaseFlags_Insensitive)));
				}
			}

			for(typename POPTIONARRAY::const_iterator it = inneroptions_.begin();
				it != inneroptions_.end();
				++it)
			{
				for(typename std::vector< MyS_ >::const_iterator cIter=cli->options_.begin();
					cIter != cli->options_.end();
					++cIter)
				{
					// ensure same option does not exist
					assert(!((*it)->isMatchOption(cIter->c_str(),
						(*it)->case_ == CaseFlags_Insensitive || cli->case_ == CaseFlags_Insensitive)));
				}
			}
		}
#else
        void check(MyO_*){}
#endif
		
		// Iterator option strings
		template<class TARGET>
		void AddOptionRange(
			const std::vector<MyS_>& optionStrings,
			int exactCount,
			TARGET* pTarget,
			ArgEncodingFlags arf = ArgEncodingFlags_Default,
			const MyS_& helpstring = MyS_())
		{
			MyO_* option = new MyO_(optionStrings, exactCount);
			option->case_ = case_;
			check(option);
			option->setTarget(pTarget);
			option->encoding_ = arf;
			option->helpString_ = helpstring;
			inneroptions_.push_back(option);
		}
		template<class TARGET>
		void AddOptionRange(
			const std::vector<MyS_>& optionStrings,
			ArgCount argCount,
			TARGET* pTarget,
			ArgEncodingFlags arf = ArgEncodingFlags_Default,
			const MyS_& helpstring = MyS_())
		{
			MyO_* option = new MyO_(optionStrings);
			option->argcountflag_ = argCount;
			option->case_ = case_;
			check(option);
			option->setTarget(pTarget);
			option->encoding_ = arf;
			option->helpString_ = helpstring;
			inneroptions_.push_back(option);
		}

		// single option string
		template<class TARGET>
		void AddOption(
			MyS_ optionString1,
			int exactCount,
			TARGET* pTarget,
			ArgEncodingFlags arf = ArgEncodingFlags_Default,
			const MyS_& helpstring = MyS_())
		{
			AddOptionRange({ optionString1 }, exactCount, pTarget, arf, helpstring);
		}
		template<class TARGET>
		void AddOption(
			MyS_ optionString1,
			ArgCount argCount,
			TARGET* pTarget,
			ArgEncodingFlags arf = ArgEncodingFlags_Default,
			const MyS_& helpstring = MyS_())
		{
			AddOptionRange({ optionString1 }, argCount, pTarget, arf, helpstring);
		}

		// two option strings
		//template<class TARGET>
		//void AddOption(
		//	MyS_ optionString1,
		//	MyS_ optionString2,
		//	int exactCount,
		//	TARGET* pTarget,
		//	ArgEncodingFlags arf = ArgEncodingFlags_Default,
		//	const MyS_& helpstring = MyS_())
		//{
		//	const MyS_ ops[] = { optionString1, optionString2 };
		//	AddOptionRange(ops, ops + _countof(ops), exactCount, pTarget, arf, helpstring);
		//}
		//template<class TARGET>
		//void AddOption(
		//	std::initializer_list<MyS_> optionStrings,
		//	int exactCount,
		//	TARGET* pTarget,
		//	ArgEncodingFlags arf = ArgEncodingFlags_Default,
		//	const MyS_& helpstring = MyS_())
		//{
		//	AddOptionRange(optionStrings.begin(), optionStrings.end(), exactCount, pTarget, arf, helpstring);
		//}
		//template<class TARGET>
		//void AddOption(
		//	std::initializer_list<const Elem*> optionStrings,
		//	int exactCount,
		//	TARGET* pTarget,
		//	ArgEncodingFlags arf = ArgEncodingFlags_Default,
		//	const MyS_& helpstring = MyS_())
		//{
		//	AddOptionRange(optionStrings.begin(), optionStrings.end(), exactCount, pTarget, arf, helpstring);
		//}


#ifdef _WIN32
// #ifdef UNICODE
		void Parse()
		{
			Parse(GetCommandLineW());
		}
		void Parse(LPCSTR pCommandLine)
		{
			int argc;
			LPSTR* pArg = CCommandLineStringBase<Elem>::getCommandLineArg(pCommandLine, &argc);
			Parse(argc, pArg);
			CCommandLineStringBase<Elem>::freeCommandLineArg(pArg);
		}
		void Parse(LPCWSTR pCommandLine)
		{
			LPWSTR *szArglist;
			int nArgs;

			szArglist = CommandLineToArgvW(pCommandLine, &nArgs);
			if (NULL == szArglist)
			{
				return;
			}
			Parse(nArgs, szArglist);
			LocalFree(szArglist);
		}
// #endif
#endif

	private:
		size_t ParseParam(size_t i, size_t argc, const Elem* const targv[], MyO_* pMyO)
		{
			if (pMyO->argcountflag_ == ArgCount::ArgCount_Zero)
				return i;

			if (pMyO->argcountflag_ == ArgCount::ArgCount_One)
			{
				++i;
				if (i >= argc)
				{
					if (strict_)
					{
						throw no_value_error<MyS_>(targv[i-1]);
					}
					return i;
				}

				const Elem* pArgv = targv[i];
				pMyO->AddValue(pArgv, strict_);
				return i;
			}

			// not yet
			assert(false);
			return 1044410;
		}
	public:
		void Parse(size_t argc, const Elem* const targv[])
		{
			assert(!parsed_);  // already parsed
			parsed_ = true;

			if (argc > 0)
			{
				program_ = targv[0];
			}

			for (size_t i = 1; i < argc; ++i)
			{
				const Elem* pArgv = targv[i];
				if (!pArgv || !pArgv[0])
					continue;

				empty_ = false;

				if (pArgv[0] == L'-' || pArgv[0] == L'/')
				{ // option argument
					MyO_* pA = FindOption(pArgv);
					if (!pA)
					{
						unknowns_.push_back(new MyO_(pArgv));
						continue;
					}

					pA->setHadOption();

					i = ParseParam(i, argc, targv, pA);
				}
				else 
				{ // value argument
					myOptionType* pA = FindOption(stdosd::stdLiterals<Elem>::nulString());
					if (!pA)
					{
						unknowns_.push_back(new MyO_((pArgv)));
						continue;
					}
					else
					{
						pA->AddValue(pArgv, strict_);
						continue;
					}
				}
			}


			// mark all options as parsed.
			// when user call some functions which is not added to parser or before parse(),
			// assert() fails.
			for (typename POPTIONARRAY::const_iterator it = useroptions_.begin();
				it != useroptions_.end();
				++it)
			{
				(*it)->setParsed();
			}

			for (typename POPTIONARRAY::iterator it = unknowns_.begin();
				it != unknowns_.end();
				++it)
			{
				(*it)->setParsed();
			}
		}
	};

			

typedef BasicCommandLineParser<std::wstring, COptionW> CCommandLineParserW;
typedef BasicCommandLineParser<std::string, COptionA> CCommandLineParserA;

#ifdef UNICODE
typedef CCommandLineParserW CCommandLineParser;
#else
typedef CCommandLineParserA CCommandLineParser;
#endif

	
} // namespace
