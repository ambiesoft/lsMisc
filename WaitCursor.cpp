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

#include <cassert>
#include <Windows.h>

#include "WaitCursor.h"

namespace Ambiesoft {
	int CWaitCursor::m_nCount = 0;
	LPCRITICAL_SECTION CWaitCursor::ms_pcsCursor = NULL;

	void CWaitCursor::DoWait(BOOL bWait)
	{
		if (NULL == ms_pcsCursor)
		{
			ms_pcsCursor = new CRITICAL_SECTION;
			InitializeCriticalSection(ms_pcsCursor);

			atexit(&CWaitCursor::DeleteCursorCS);
		}

		EnterCriticalSection(ms_pcsCursor);
		if (bWait)
		{
			if (m_nCount > 0)
			{
				m_nCount++;
				LeaveCriticalSection(ms_pcsCursor);
				return;
			}
			m_nCount = 1;
			m_hCurOld = SetCursor(LoadCursor(NULL, IDC_WAIT));
		}
		else
		{
			if (m_nCount > 1) {
				m_nCount--;
				LeaveCriticalSection(ms_pcsCursor);
				return;
			}
			m_nCount = 0;

			if (m_hCurOld == NULL) {
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
			SetCursor(m_hCurOld);
			m_hCurOld = NULL;
		}
		LeaveCriticalSection(ms_pcsCursor);
	}

	void CWaitCursor::DeleteCursorCS()
	{
		assert(ms_pcsCursor);
		DeleteCriticalSection(ms_pcsCursor);
		delete ms_pcsCursor;
		ms_pcsCursor = NULL;
	}
}