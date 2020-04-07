# SessionGlobalMemory
A C++ class to access win32 Shared Memory 

# How to use
Include `SessionGlobalMemory.h` in your project, and refer to the namespace `Ambiesoft`.

## Fixed size data
Define a variable as follows.
```
CSessionGlobalMemory<Data> sgData("sgData");
```
This creates a shared memory named `sgData` and Mutex for exclusive access named `sgData_Mutex`. Other processes can create same name object that can access this *Data*. *Data* must not include a pointer since it is invalid between processes. 

## Dynamic size data
### Creating and setting data

```
	CDynamicSessionGlobalMemory sgDyn("dyn1000", 1000);
	unsigned char* p = (unsigned char*)malloc(1000);
	p[0] = 11;
	p[1] = 12;
	p[2] = 13;

	p[997] = 97;
	p[998] = 98;
	p[999] = 99;
	sgDyn.set(p);
  ```
1. Create an instance of `CDynamicSessionGlobalMemory`, with arguments of name and size.
2. Copy local data by calling `set()`.

### User or consumer (may be another process)
```
	CDynamicSessionGlobalMemory sgDynUser("dyn1000");
	unsigned char* p = (unsigned char*)malloc(sgDynUser.size());
	sgDynUser.get(p);
	assert(p[0] == 11);
	assert(p[999] == 99);
```
1. Create an instance of `CDynamicSessionGlobalMemory` with name.
2. Allocate local memory with size of `size()`.
3. Get the data by calling get().



# Licence
```
// Copyright (c) 2017 Ambiesoft. 
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
