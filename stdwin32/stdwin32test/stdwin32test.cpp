// stdwin32test.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "../stdwin32.h"

int main(int argc, char* argv[])
{
	std::string s=	stdwin32::string_format("%d-%s", 100, "aaa");
	return 0;
}
