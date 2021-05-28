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

// Must comment out to define NOMINMAX
// #include "stdafx.h"

#if !defined(NOMINMAX)
#define NOMINMAX
#endif

#include <Windows.h>

#include <vector>
#include <limits>
#include <cassert>

#include "GetChildWindowBy.h"

using namespace std;

namespace Ambiesoft{
	namespace {

		class ContextData {
		public:
			enum EnumBy {
				ENUM_BY_NOTHING,
				ENUM_BY_TEXT,
				ENUM_BY_CLASSNAME,
				ENUM_BY_ALL,
				ENUM_BY_HASFOCUS,
			};
		private:
			size_t maxcount_;
			EnumBy enumBy_;
			LPCWSTR pName_;
			vector<HWND> results_;
			bool allChildren_;
			ContextData() {}


		public:
			ContextData(const size_t& maxcount,
				EnumBy enumBy,
				LPCWSTR pName,
				bool allChildren = false) :
				maxcount_(maxcount), enumBy_(enumBy), pName_(pName), allChildren_(allChildren) {}

			LPCWSTR name() const {
				return pName_;
			}
			EnumBy enumBy() const {
				return enumBy_;
			}
			bool IsAllChildren() const {
				return allChildren_;
			}
			void addResult(HWND h) {
				results_.push_back(h);
			}
			size_t resultSize() const {
				return results_.size();
			}
			size_t maxcount() const {
				return maxcount_;
			}
			bool isResultEmpty() const {
				return results_.empty();
			}
			HWND getFirstResult() const {
				return results_[0];
			}
			vector<HWND> getResults() const {
				return results_;
			}
			bool getBoolResult() const {
				return !results_.empty();
			}


			static BOOL CALLBACK enumProc(HWND hwnd, LPARAM lParam)
			{
				ContextData* pThis = (ContextData*)lParam;

				if (pThis->enumBy() == ENUM_BY_ALL ||
					pThis->enumBy() == ENUM_BY_TEXT ||
					pThis->enumBy() == ENUM_BY_CLASSNAME)
				{
					if (pThis->name())
					{
						TCHAR szT[1024]; szT[0] = 0;
						if (pThis->enumBy() == ENUM_BY_TEXT)
							GetWindowText(hwnd, szT, _countof(szT));
						else if (pThis->enumBy() == ENUM_BY_CLASSNAME)
							GetClassName(hwnd, szT, _countof(szT));
						else
							assert(false);
						if (lstrcmp(szT, pThis->name()) == 0)
						{
							pThis->addResult(hwnd);
						}
					}
					else
					{
						pThis->addResult(hwnd);
					}
				}
				else if (pThis->enumBy() == ENUM_BY_HASFOCUS)
				{
					if (GetFocus() == hwnd)
						pThis->addResult(hwnd);
				}
				else
				{
					assert(FALSE);
				}

				if (pThis->resultSize() >= pThis->maxcount())
				{
					// no continue;
					return FALSE;
				}

				if (pThis->IsAllChildren())
				{
					EnumChildWindows(hwnd, enumProc, lParam);
				}
				// continue
				return TRUE;
			}
		};
	}
	HWND GetChildWindowByText(HWND hwndParent, LPCWSTR pName)
	{
		ContextData context(1, ContextData::ENUM_BY_TEXT, pName);

		EnumChildWindows(hwndParent, ContextData::enumProc, (LPARAM)&context);

		return context.isResultEmpty() ? NULL : context.getFirstResult();
	}
	HWND GetChildWindowByClassName(HWND hwndParent, LPCWSTR pName, DWORD* pdwLE)
	{
		ContextData context(1, ContextData::ENUM_BY_CLASSNAME, pName, true);

		EnumChildWindows(hwndParent, ContextData::enumProc, (LPARAM)&context);
		if (pdwLE)
		{
			*pdwLE = GetLastError();
		}
		return context.isResultEmpty() ? NULL : context.getFirstResult();
	}
	std::vector<HWND> GetChildWindowsByClassName(HWND hwndParent, LPCWSTR pName)
	{
		ContextData context(std::numeric_limits<size_t>::max(), ContextData::ENUM_BY_CLASSNAME, pName);

		EnumChildWindows(hwndParent, ContextData::enumProc, (LPARAM)&context);

		return context.getResults();
	}
	vector<HWND> GetChildWindows(HWND hwndParent)
	{
		ContextData context(std::numeric_limits<size_t>::max(), ContextData::ENUM_BY_ALL, nullptr);

		EnumChildWindows(hwndParent, ContextData::enumProc, (LPARAM)&context);

		return context.getResults();
	}
	bool IsChildHasFocus(HWND hwndParent)
	{
		ContextData context(1, ContextData::ENUM_BY_HASFOCUS, nullptr, true);

		EnumChildWindows(hwndParent, ContextData::enumProc, (LPARAM)&context);

		return context.getBoolResult();
	}

} // namespace