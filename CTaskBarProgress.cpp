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

#include "CTaskBarProgress.h"

namespace Ambiesoft {

	void CTaskBarProgress::Init(HWND h)
	{
		m_hWnd = h;
		CoCreateInstance(
			CLSID_TaskbarList, NULL, CLSCTX_ALL,
			IID_ITaskbarList3, (void**)&m_pTaskBarlist);
		if (m_pTaskBarlist)
			m_pTaskBarlist->SetProgressState(m_hWnd, TBPF_INDETERMINATE);
	}
	CTaskBarProgress::~CTaskBarProgress()
	{
		m_pTaskBarlist = nullptr;
	}

	void CTaskBarProgress::SetProgress(int percent)
	{
		if (m_pTaskBarlist)
		{
			m_pTaskBarlist->SetProgressValue(m_hWnd, percent, 100);
			m_pTaskBarlist->SetProgressState(m_hWnd, TBPF_NORMAL);
		}
	}
	void CTaskBarProgress::SetProgressPaused()
	{
		if (m_pTaskBarlist)
		{
			m_pTaskBarlist->SetProgressState(m_hWnd, TBPF_PAUSED);
		}
	}
	void CTaskBarProgress::SetProgressUnpaused()
	{
		if (m_pTaskBarlist)
		{
			m_pTaskBarlist->SetProgressState(m_hWnd, TBPF_NORMAL);
		}
	}
	void CTaskBarProgress::SetProgressEnded()
	{
		if (m_pTaskBarlist)
		{
			m_pTaskBarlist->SetProgressState(m_hWnd, TBPF_NOPROGRESS);
		}
	}
}