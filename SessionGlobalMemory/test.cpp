//BSD 3-Clause License
//
//Copyright (c) 2017, Ambiesoft
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//* Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//* Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//* Neither the name of the copyright holder nor the names of its
//  contributors may be used to endorse or promote products derived from
//  this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "stdafx.h"
#include <process.h>
#include <cassert>

#include "SessionGlobalMemory.h"
using namespace Ambiesoft;



#if _MSC_VER < 1500
#define _countof(a) (sizeof(a)/sizeof(a[0]))
#endif 

#if _MSC < 1600
#define static_assert(s,t) assert(s)
#endif

struct Data {
	int d[1024];
	int checksum_;

	Data() {
		memset(d, 0, sizeof(d));
		checksum_ = 0;
		ctorcount++;
	}

	Data(const Data& data) {
		memcpy(d, &data.d, sizeof(d));
		checksum_ = data.checksum_;
		ctorcount++;
	}



	void setrand()
	{
		checksum_ = 0;
		for (int i = 0; i < _countof(d); ++i)
		{
			d[i] = rand();
			checksum_ += d[i];
		}
	}

	bool isChesksumOK()
	{
		int cs = 0;
		for (int i = 0; i < _countof(d); ++i)
		{
			cs += d[i];
		}
		assert(cs == checksum_);
		return checksum_ == cs;
	}
	static int ctorcount;
	static void print() {
		printf("ctorcount = %d\n", ctorcount);
	}
};
int Data::ctorcount;


template<class T>
CSessionGlobalMemory<T> getSGMEM(T initialvalue, LPCTSTR pName)
{
	CSessionGlobalMemory<T> ret(pName);
	ret = initialvalue;
	return ret;
}

void testreader(int span)
{
	CSessionGlobalMemory<Data> sgData("sgData");
	for (;;)
	{
		Data data;
		sgData.get(data);
		printf("Data Read:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
		printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");

		Sleep(span);
	}
}

void testwriter(int span)
{
	CSessionGlobalMemory<Data> sgData("sgData");
	for (;;)
	{
		Data data;
		data.setrand();
		sgData = data;
		printf("Data assigned:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
		printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");

		Sleep(span);
	}
}

void test1();
void test2();
void test3();
void test4();
int main(int argc, char* argv[])
{
	if (argc > 1 )
	{

		if (strcmp(argv[1], "testall") == 0)
		{
			test1();
			test2();
			test3();
			test4();
			return 0;
		}
		if (strcmp(argv[1], "test1") == 0)
		{
			test2();
			return 0;
		}
		if (strcmp(argv[1], "test2") == 0)
		{
			test2();
			return 0;
		}
		if (strcmp(argv[1], "test3") == 0)
		{
			test3();
			return 0;
		}
		if (strcmp(argv[1], "test4") == 0)
		{
			test4();
			return 0;
		}
	}

	// test.exe -reader 100
	if (argc > 1)
	{
		
		int i= 1;
		// puts(argv[i]);
		if (_strcmpi(argv[i], "-reader") == 0)
		{
			SetConsoleTitleA("reader");			
			testreader(atoi(argv[i + 1]));
			return 0;
		}
		else if (_strcmpi(argv[i], "-writer") == 0)
		{
			SetConsoleTitleA("writer");
			testwriter(atoi(argv[i + 1]));
			return 0;
		}
		else
		{
			assert(false);
			return -1;
		}
	}

	srand((unsigned int)time(NULL));
	CSessionGlobalMemory<Data> sgData("sgData");
	CSessionGlobalMemoryNTS<Data> sgDataNTS("sgData");

	bool quit=false;
	for(; !quit;)
	{
		printf("\n"
			"'a' to assign fixed size data\n"
			"'r' to read fixed size data\n"
			"'0' to launch fixed size reader\n"
			"'1' to launch fixed size writer\n"
			"'q' to quit.\n"
			">"
			);

		switch(int ch=_getch())
		{
			case '0': case '1':
			{
				// http://www.cplusplus.com/forum/beginner/103383/
				char comspec[MAX_PATH]; comspec[0] = 0;
				size_t t;
				getenv_s(&t, comspec, "COMSPEC");
				_spawnl(_P_NOWAIT // flags
					, comspec   // cmd line
					, comspec   // arg[0] to main (of target)
					, "/c"      // arg[1]
					, "start"   // etc (see note above)
					, comspec
					, "/c"
					, argv[0]
					, ch == '0' ? "-reader" : "-writer"
					, "100"
					, NULL);
			}
			break;
			
			case 'a':
			{
				Data data;
				data.setrand();
				sgData = data;
				printf("Data assigned:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
				printf("Checksum is %s.\n", data.isChesksumOK() ? "OK":"NG");
			}
			break;
			case 'A':
			{
				Data data;
				data.d[0]=rand();
				data.d[1]=rand();
				data.d[2]=rand();
				memcpy(&sgDataNTS,&data,sizeof(data));
				printf("Data assigned:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
				printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");
			}
			break;

			case 'r':
			{
				Data data;
				sgData.get(data);
				
				printf("Data Read:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
				printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");
			}
			break;
			case 'R':
			{
				Data data;
				memcpy(&data, &sgDataNTS, sizeof(data));
				printf("Data Read:\t%d, %d, %d, ...\n", data.d[0], data.d[1], data.d[2]);
				printf("Checksum is %s.\n", data.isChesksumOK() ? "OK" : "NG");
			}
			break;


			case 'q':
			case 'Q':
			{
				quit = true;
				break;
			}
			break;

			default:
				break;
		}
	}


	return 0;
}



template<int LENGTH>
struct FixedLengthStruct{
	unsigned char data[LENGTH];
	int getLength() const {
		return LENGTH;
	}
};

struct VariableLengthStructBase {
protected:
	int length_;
	VariableLengthStructBase(){}
public:
	int getLength() const {
		return length_;
	}
};

template<int LENGTH>
struct VariableLengthStruct :VariableLengthStructBase {
public:
	unsigned char data[LENGTH];
	VariableLengthStruct() {
		length_ = LENGTH;
	}

	unsigned char* getData() {
		return data;
	}

};

void test1()
{

	//CSessionGlobalMemory<Data> tmp(sgData);
	CSessionGlobalMemory<int> tmp2(getSGMEM(3, "sgINT"));
	CSessionGlobalMemory<int> tmp3(getSGMEM(3, "sgINTtmp"));
	tmp3 = tmp2;
#if !defined(AMBIESOFT_NO_RVALUE_)
	tmp2 = std::move(tmp3);
	tmp2 = 9999;
#endif
	std::string sa = "aaa";
	std::string sb;
#if !defined(AMBIESOFT_NO_RVALUE_)
	sb = std::move(sa);
#endif
	sb = "bbb";
}

void test2()
{
	static_assert(1000 == sizeof(FixedLengthStruct<1000>), "not 1000");
	assert(1000 == sizeof(FixedLengthStruct<1000>));

	CSessionGlobalMemory<FixedLengthStruct<1000> > sgData("thousandbyte");

	FixedLengthStruct<1000> data;
	sgData.get(data);
	sgData.getName();
}

void test3()
{
	// setter
	CDynamicSessionGlobalMemory sgDyn("dyn1000", 1000);
	unsigned char* p = (unsigned char*)malloc(1000);
	p[0] = 11;
	p[1] = 12;
	p[2] = 13;

	p[997] = 97;
	p[998] = 98;
	p[999] = 99;
	sgDyn.set(p);


	{
		// getter
		CDynamicSessionGlobalMemory sgDynUser("dyn1000");
		unsigned char* p = (unsigned char*)malloc(sgDynUser.size());
		sgDynUser.get(p);
		assert(p[0] == 11);
		assert(p[999] == 99);
	}
}

void test4()
{
	CSessionGlobalMemory<bool> sgb("sgb", true);
	if (sgb)
		sgb = false;

	assert(!sgb);

	sgb = !sgb;

	assert(sgb);

}