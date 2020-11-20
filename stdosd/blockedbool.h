//
//Copyright (C) 2018  Ambiesoft
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef BLOCKEDTRUE_H
#define BLOCKEDTRUE_H

#include <functional>

namespace Ambiesoft {

	template<class N>
	class BlockedValue
	{
		volatile N* target_;
		N exitValue_;
	public:
		BlockedValue(volatile N* target, N initialValue, N exitValue) :
			target_(target),
			exitValue_(exitValue)
		{
			*target_ = initialValue;
		}
		BlockedValue(volatile N* target) :
			BlockedValue(target,
				!N(),  // true
				N())   // false
		{}
		~BlockedValue()
		{
			*target_ = exitValue_;
		}
	};
	template<class N>
	class RestoreValue
	{
		using F = std::function<void(const N&)>;
		
		N restore_;
		F fRestore_;
	public:
		RestoreValue(const N& restore, const F& fRestore) :
			restore_(restore),
			fRestore_(fRestore)
		{
		}
		~RestoreValue()
		{
			fRestore_(restore_);
		}
	};
	
	typedef BlockedValue<bool> BlockedBool;



	template<class FUNC = std::function<void()> >
	class BlockedOperation
	{
		FUNC enter_;
		FUNC exit_;
	public:
		BlockedOperation(
			FUNC enter,
			FUNC exit) :
			exit_(exit) {
			if (enter)
				enter();
		}
		~BlockedOperation() {
			if (exit_)
				exit_();
		}
	};
} // namespace

#endif // BLOCKEDTRUE_H
