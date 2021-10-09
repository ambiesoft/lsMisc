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
#include <Windows.h>

namespace Ambiesoft {
	class CHandle
	{
		HANDLE h_;
		void Close() {
			if (*this)
				CloseHandle(h_);
			h_ = nullptr;
		}
	public:
		CHandle() : h_(nullptr){}
		explicit CHandle(HANDLE h) :h_(h) {}
		CHandle(const CHandle& that) = delete;
		CHandle(CHandle&& that) noexcept {
			h_ = that.h_;
			that.h_ = nullptr;
		}

		~CHandle() {
			Close();
		}

		const CHandle& operator=(HANDLE h) {
			Close();
			this->h_ = h;
			return *this;
		}
		const CHandle& operator=(CHandle& that) = delete;
		CHandle& operator=(CHandle&& that) {
			if (&*this != &that) {
				Close();
				this->h_ = that.h_;
				that.h_ = nullptr;
			}
			return *this;
		}
	
		operator bool() const {
			return h_ != nullptr && h_ != INVALID_HANDLE_VALUE;
		}
		operator HANDLE() const {
			return h_;
		}
		HANDLE* operator &() {
			return &h_;
		}
		HANDLE Release() {
			HANDLE h = h_;
			h_ = nullptr;
			return h;
		}
	};
	class CHWnd
	{
		HWND h_;
	public:
		CHWnd():h_(nullptr) {}
		explicit CHWnd(HWND h) :h_(h) {}
		~CHWnd() {
			if (h_ == nullptr)
				return;
			DestroyWindow(h_);
		}
		operator bool() const {
			return !!IsWindow(h_);
		}
		operator HWND() const {
			return h_;
		}
		const CHWnd& operator=(CHWnd& that) {
			this->h_ = that.h_;
			return *this;
		}
		const CHWnd& operator=(HWND h) {
			this->h_ = h;
			return *this;
		}
	};
	class CHIcon
	{
		HICON h_;
	public:
		explicit CHIcon(HICON h) : h_(h) {}
		~CHIcon() {
			if (h_ == nullptr)
				return;
			DestroyIcon(h_);
		}
		operator bool() const {
			return !!h_;
		}
		operator HICON() const {
			return h_;
		}
	};
	class CHMenu
	{
		HMENU h_;
	public:
		explicit CHMenu(HMENU h) :h_(h) {}
		~CHMenu() {
			if (h_ == nullptr)
				return;
			DestroyMenu(h_);
		}
		operator bool() const {
			return !!IsMenu(h_);
		}
		operator HMENU() const {
			return h_;
		}
	};

	class CHModule
	{
		HMODULE h_ = nullptr;
		bool autoclose_ = false;
		void Close() {
			if (h_) {
				FreeLibrary(h_);
				h_ = nullptr;
			}
		}
	public:
		explicit CHModule(HMODULE h, bool autoclose = true) :
			h_(h),
			autoclose_(autoclose) {};
		~CHModule() {
			if(autoclose_)
				Close();
		}
		CHModule(const CHModule& that) = delete;
		CHModule(CHModule&& that) noexcept {
			h_ = that.h_;
			that.h_ = nullptr;
		}
		const CHModule& operator=(HMODULE h) {
			Close();
			this->h_ = h;
			return *this;
		}
		operator bool() const {
			return !!h_;
		}
		operator HMODULE() const {
			return h_;
		}
	};

	template<class FUNC>
	class CGetProcAddress
	{
		HMODULE h_ = nullptr;
		FUNC func_ = nullptr;
	public:
		CGetProcAddress(){}
		CGetProcAddress(const CGetProcAddress&) = delete;
		CGetProcAddress(CGetProcAddress&& that) {
			h_ = that.h_;
			that.h_ = nullptr;
			func_ = that.func_;
			that.func_ = nullptr;
		}
		CGetProcAddress & operator=(const CGetProcAddress&) = delete;

		CGetProcAddress(LPCWSTR pLibrary, LPCSTR pFunc) {
			h_ = LoadLibrary(pLibrary);
			if (!h_)
				return;
			func_ = (FUNC)GetProcAddress(h_, pFunc);
		}
		~CGetProcAddress() {
			if (h_)
				FreeLibrary(h_);
		}
		FUNC GetProc() const {
			return func_;
		}
		operator bool() const {
			return !!func_;
		}

	};
}
