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
	public:
		CHandle() : h_(nullptr){}
		CHandle(HANDLE h) :h_(h) {

		}
		CHandle(const CHandle& that) {
			h_ = that.h_;
		}
		CHandle(CHandle&& that) {
			h_ = that.h_;
			that.h_ = nullptr;
		}
		~CHandle() {
			if (h_ == nullptr || h_ == INVALID_HANDLE_VALUE)
				return;
			CloseHandle(h_);
		}
		const CHandle& operator=(CHandle& that) {
			this->h_ = that.h_;
			return *this;
		}
		const CHandle& operator=(HANDLE h) {
			this->h_ = h;
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
	};
	class CHWnd
	{
		HWND h_;
	public:
		CHWnd(HWND h) :h_(h) {

		}
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
	};
	class CHMenu
	{
		HMENU h_;
	public:
		CHMenu(HMENU h) :h_(h) {

		}
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
}
