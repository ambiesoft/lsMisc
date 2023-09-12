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


#ifdef __cplusplus_cli

namespace Ambiesoft {

	ref class COpenCommandGetResult
	{
	internal:
		static System::Text::StringBuilder^ sbOut;
		static System::Text::StringBuilder^ sbErr;
		static void OnOutputDataReceived(System::Object ^sender, System::Diagnostics::DataReceivedEventArgs ^e)
		{
			sbOut->Append(e->Data);
		}
		static void OnErrorDataReceived(System::Object ^sender, System::Diagnostics::DataReceivedEventArgs ^e)
		{
			sbErr->Append(e->Data);
		}

		static void OpenCommnadGetResultImpl(System::String^ fileName, System::String^ arguments,
			System::Text::Encoding^ encoding,
			System::String^% out,
			System::String^% err)
		{
			System::Diagnostics::Process process;

			System::Diagnostics::ProcessStartInfo startInfo;
			startInfo.FileName = fileName;
			startInfo.Arguments = arguments;

			startInfo.CreateNoWindow = true;
			startInfo.UseShellExecute = false;

			// startInfo.WorkingDirectory = System::Environment::CurrentDirectory;

			startInfo.StandardOutputEncoding = encoding;
			startInfo.RedirectStandardOutput = true;

			startInfo.StandardErrorEncoding = encoding;
			startInfo.RedirectStandardError = true;

			process.OutputDataReceived += gcnew System::Diagnostics::DataReceivedEventHandler(&COpenCommandGetResult::OnOutputDataReceived);
			process.ErrorDataReceived += gcnew System::Diagnostics::DataReceivedEventHandler(&COpenCommandGetResult::OnErrorDataReceived);

			process.StartInfo = %startInfo;
			sbOut = gcnew System::Text::StringBuilder();
			sbErr = gcnew System::Text::StringBuilder();

			process.Start();

			process.BeginOutputReadLine();
			process.BeginErrorReadLine();

			process.WaitForExit();

			out = sbOut->ToString();
			err = sbErr->ToString();

		}
	};
	static void OpenCommnadGetResult(System::String^ fileName, System::String^ arguments,
		System::Text::Encoding^ encoding,
		System::String^% out,
		System::String^% err)
	{
		COpenCommandGetResult::OpenCommnadGetResultImpl(
			fileName, arguments,
			encoding,
			out, err);
	}



	System::Windows::Forms::DialogResult Alert(System::String^ text);
	System::Windows::Forms::DialogResult Alert(System::Exception^ ex);


	ref struct TemporalCurrentDir
	{
		System::String^ save_;
		TemporalCurrentDir(System::String^ dir)
		{
			save_ = System::Environment::CurrentDirectory;
			System::Environment::CurrentDirectory = dir;
		}
		~TemporalCurrentDir()
		{
			System::Environment::CurrentDirectory = save_;
		}
	};

	System::String^ getShortPah(System::String^ file);
} // namespace


#endif