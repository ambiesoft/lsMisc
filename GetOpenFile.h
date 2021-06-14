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

#include <vector>
#include <tchar.h>
#include "tstring.h"

namespace Ambiesoft {


	class CGetOpenFileFilter{
		std::vector<tstring> descs_;
		std::vector<tstring> exts_;
		std::vector<bool> showextsindesc_;
		tstring theFilter_;

	public:
		CGetOpenFileFilter() {
		}
		void AddFilter(const tstring& desc, const tstring& ext, bool bShowExtInDesc = true)
		{
			descs_.push_back(desc);
			exts_.push_back(ext);
			showextsindesc_.push_back(bShowExtInDesc);
		}


		operator LPCTSTR() {
			size_t size = exts_.size();
			if (size == 0)
				return NULL;

			theFilter_ = _T("");
			for (size_t i = 0; i < size; ++i)
			{
				theFilter_ += descs_[i];
				if (showextsindesc_[i])
				{
					theFilter_ += _T(" (");
					theFilter_ += exts_[i];
					theFilter_ += _T(")");
				}
				theFilter_ += _T('|');

				theFilter_ += exts_[i];
				theFilter_ += _T('|');
			}
			theFilter_ += _T('|');
			return theFilter_.c_str();
		}
	};

	enum class GETFILEFILTER
	{
		NONE,
		APP,
	};
	BOOL GetOpenFile(
		HINSTANCE hInst,
		HWND hWnd,
		LPCTSTR pFilter,
		LPCTSTR pInitialDir,
		LPCTSTR pTitle,
		std::wstring* fullpath, std::wstring* path = NULL);
	BOOL GetOpenFile(
		HINSTANCE hInst,
		HWND hWnd,
		GETFILEFILTER filter,
		LPCTSTR pInitialDir,
		LPCTSTR pTitle,
		std::wstring* fullpath, std::wstring* path = NULL);

	BOOL GetSaveFile(
		HWND hWnd,
		LPCTSTR pFilter,
		LPCTSTR pInitialDir,
		LPCTSTR pInitialFile,
		LPCTSTR pDefExt,
		LPCTSTR pTitle,
		std::wstring* fullpath, std::wstring* path = NULL);

}