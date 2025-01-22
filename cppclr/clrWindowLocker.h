#pragma once
#ifdef __cplusplus_cli

#include <Windows.h>
#include <cassert>

namespace Ambiesoft {
	namespace CLRHelper {

		struct WindowLocker
		{
			HWND h_;
			bool enable_;
			WindowLocker(HWND h, bool enable = true) {
				enable_ = enable;
				if (!enable)
					return;

				h_ = h;
				assert(::IsWindow(h));
				//			DVERIFY(::LockWindowUpdate(h));
				SendMessageA(h, WM_SETREDRAW, FALSE, 0);
			}
			~WindowLocker() {
				if (!enable_)
					return;
				//			::LockWindowUpdate(NULL);
				SendMessageA(h_, WM_SETREDRAW, TRUE, 0);
			}
		};

		ref struct FormLocker
		{
			System::Windows::Forms::Form^ f_;
			HWND h_;
		public:
			FormLocker(System::Windows::Forms::Form^ f) : f_(f) 
			{
				h_ = (HWND)f->Handle.ToPointer();
				assert(::IsWindow(h_));
				SendMessageA(h_, WM_SETREDRAW, FALSE, 0);
			}
			~FormLocker() 
			{
				SendMessageA(h_, WM_SETREDRAW, TRUE, 0);
				f_->Invalidate(true);
				f_->Update();
			}
		};

		ref struct ListUpdateLocker
		{
			System::Windows::Forms::ListView^ c_;
			ListUpdateLocker(System::Windows::Forms::ListView^ c) :c_(c)
			{
				assert(c && c->IsHandleCreated && !c->IsDisposed);
				c_->BeginUpdate();
			}
			~ListUpdateLocker()
			{
				c_->EndUpdate();
			}
		};

		ref class LayoutSuspender 
		{
			initonly System::Windows::Forms::Control^ f_;
		public:
			LayoutSuspender(System::Windows::Forms::Control^ f)
			{
				f->SuspendLayout();
				f_ = f;
			}
			~LayoutSuspender()
			{
				f_->ResumeLayout();
			}
		};
	}
}

#endif // __cplusplus_cli