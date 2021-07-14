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








#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/logging.h"

#include "CommandLineString.h"

#include "TraceGlog.h"

namespace Ambiesoft {

	bool TraceFunctionGlog::enabled_ = false;
	int TraceFunctionGlog::depth_ = 0;
	// LOG(INFO) << "log info";

	static std::string gLogName;

	// static
	void TraceFunctionGlog::PrepareGlog()
	{
		CCommandLineStringA cmd;
		for (size_t i = 1; i < cmd.getCount(); ++i)
		{
			if (cmd.getArg(i) == "--enable-log")
			{
				enabled_ = true;
				break;
			}
		}
		if (!enabled_)
			return;

		CreateDirectoryA("./glogout", NULL);
		FLAGS_log_dir = "./glogout";

		gLogName = cmd.getArg(0);
		google::InitGoogleLogging(gLogName.c_str());
	}

	TraceFunctionGlog::TraceFunctionGlog(const char* file, const char* func, int line)
	{
		if (!enabled_)
			return;
		file_ = file;
		func_ = func;
		line_ = line;

		++depth_;
		int depth = depth_;
		while (--depth)
			depthstr_ += " ";

		LOG(INFO) << depthstr_ << "ENTER-> " << file_ << ":" << func_ << ":" << line_;
	}
	TraceFunctionGlog::~TraceFunctionGlog()
	{
		if (!enabled_)
			return;

		LOG(INFO) << depthstr_ << "<-LEAVE " << file_ << ":" << func_ << ":" << line_;
	}

}