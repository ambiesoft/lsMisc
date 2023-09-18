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
#include <cassert>
//#include <xstddef>  // for 'addressof'
#include <Windows.h>

#ifndef NDEBUG
#define HANDLEVERIFY(f) assert(f)
#else
#define HANDLEVERIFY(f) (f)
#endif

namespace Ambiesoft {

	// Process, thread, mutex, etc
	// invalid is nullptr
	struct KernelHandleTraits
	{
		using HandleType = HANDLE;
		static constexpr bool IsInvalid(HandleType h) {
			return h == nullptr;
		}
		static void SetInvalid(HandleType* h) {
			*h = nullptr;
		}
		static bool Close(HandleType h) {
			return !!CloseHandle(h);
		}
	};
	struct FileHandleTraits
	{
		using HandleType = HANDLE;
		static constexpr bool IsInvalid(HandleType h) {
			return h == nullptr || h == INVALID_HANDLE_VALUE;
		}
		static void SetInvalid(HandleType* h) {
			*h = nullptr;
		}
		static bool Close(HandleType h) {
			return !!CloseHandle(h);
		}
	};
	struct FindVolumeHandleTraits
	{
		using HandleType = HANDLE;
		static constexpr bool IsInvalid(HandleType h) {
			return h == nullptr || h == INVALID_HANDLE_VALUE;
		}
		static void SetInvalid(HandleType* h) {
			*h = INVALID_HANDLE_VALUE;
		}
		static bool Close(HandleType h) {
			return !!FindVolumeClose(h);
		}
	};
	struct HwndTraits
	{
		using HandleType = HWND;
		static constexpr bool IsInvalid(HandleType h) {
			return h == nullptr || !IsWindow(h);
		}
		static void SetInvalid(HandleType* h) {
			*h = nullptr;
		}
		static bool Close(HandleType h) {
			return !!DestroyWindow(h);
		}
	};
	struct HiconTraits
	{
		using HandleType = HICON;
		static constexpr bool IsInvalid(HandleType h) {
			return h == nullptr;
		}
		static void SetInvalid(HandleType* h) {
			*h = nullptr;
		}
		static bool Close(HandleType h) {
			return !!DestroyIcon(h);
		}
	};
	struct HmenuTraits
	{
		using HandleType = HMENU;
		static constexpr bool IsInvalid(HandleType h) {
			return h == nullptr;
		}
		static void SetInvalid(HandleType* h) {
			*h = nullptr;
		}
		static bool Close(HandleType h) {
			return !!DestroyMenu(h);
		}
	};
	struct HDCTraits
	{
		using HandleType = HDC;
		static constexpr bool IsInvalid(HandleType h) {
			return h == nullptr;
		}
		static void SetInvalid(HandleType* h) {
			*h = nullptr;
		}
		static bool Close(HandleType h) {
			return !!ReleaseDC(WindowFromDC(h), h);
		}
	};
	struct HFontTraits
	{
		using HandleType = HFONT;
		static constexpr bool IsInvalid(HandleType h) {
			return h == nullptr;
		}
		static void SetInvalid(HandleType* h) {
			*h = nullptr;
		}
		static bool Close(HandleType h) {
			return !!DeleteObject(h);
		}
	};

	template<class Trait>
	class CHandleBase
	{
		using T = typename Trait::HandleType;
		T h_;
	public:
		CHandleBase() {
			Trait::SetInvalid(&h_);
		}
		explicit CHandleBase(T h) :h_(h) {}
		CHandleBase(const CHandleBase& that) = delete;
		CHandleBase(CHandleBase&& that) noexcept {
			h_ = that.h_;
			Trait::SetInvalid(&that.h_);
		}

		~CHandleBase() {
			Close();
		}

		const CHandleBase& operator=(T h) {
			Close();
			this->h_ = h;
			return *this;
		}
		const CHandleBase& operator=(CHandleBase& that) = delete;
		CHandleBase& operator=(CHandleBase&& that) {
			if (addressof(*this) != addressof(that)) {
				Close();
				this->h_ = that.h_;
				Trait::SetInvalid(&that.h_);
			}
			return *this;
		}

		operator bool() const {
			return !Trait::IsInvalid(h_);
		}
		operator T() const {
			return h_;
		}
		T* operator &() {
			Close();
			return &h_;
		}
		T Release() {
			T h = h_;
			Trait::SetInvalid(&h_);
			return h;
		}
		bool Close() {
			bool ret = false;
			if (!Trait::IsInvalid(h_))
			{
				ret = Trait::Close(h_);
			}
			Trait::SetInvalid(&h_);
			return ret;
		}
	};

	template<class Trait>
	bool operator==(const typename Trait::HandleType& left, const CHandleBase<Trait>& right)
	{
		return right == left;
	}
	using CKernelHandle = CHandleBase<KernelHandleTraits>;
	using CFileHandle = CHandleBase<FileHandleTraits>;
	using CFindVolumeHandle = CHandleBase<FindVolumeHandleTraits>;
	using CHWnd = CHandleBase<HwndTraits>;
	using CHIcon = CHandleBase<HiconTraits>;
	using CHMenu = CHandleBase<HmenuTraits>;
	using CHDC = CHandleBase<HDCTraits>;
	using CHFont = CHandleBase<HFontTraits>;
	
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

	class CcoInitializer
	{
	public:
		CcoInitializer() {
			CoInitialize(nullptr);
		}
		~CcoInitializer() {
			CoUninitialize();
		}
	};
}
#undef HANDLEVERIFY
