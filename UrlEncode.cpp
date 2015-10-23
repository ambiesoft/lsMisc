
/*
----- UrlEncode �֐� for C ���� Version 1.02 Beta -----
(C)2002 �����
MAIL : kelbe@geocities.co.jp
HOME : http://www.geocities.co.jp/SilkRoad/4511/

�����Ƃ��ė^����ꂽ������� URL �G���R�[�h�����������Ԃ��܂��B
�Ȃ��A���o�[�W�����ł� ANSI/S-JIS �̂ݑΉ��ł��B

���� *csource
	URL �G���R�[�h������������ւ̃|�C���^���w�肵�܂��B

���� *cbuffer
	URL �G���R�[�h���ꂽ�����񂪊i�[�����o�b�t�@�ւ̃|�C���^���w�肵�܂��B
	�Ȃ��A���� *csource �Ɏw�肵��������T�C�Y�� 3 �{ + 1 �ȏ�̃T�C�Y��
	�K�v�ƂȂ�܂��B

���� lbuffersize
	cbuffer �̃o�b�t�@�T�C�Y���w�肵�܂��B

�߂�l
	�֐�����������ƁAcbuffer �� URL �G���R�[�h���ꂽ�����񂪊i�[����A
	cbuffer �o�b�t�@�ɏ������܂ꂽ������(�I�[�� NULL ����������)��
	�Ԃ�܂��B���s�����ꍇ�� 0 ���Ԃ�܂��B

���� URL �G���R�[�h�֐��ٍ͐�� VB �p UrlEncode �֐����A������ C ����p��
�������������̂ł��B���� C ����ɂ��Ă͑S���̏��S�҂ł��̂ŁA
�\�[�X������������ł��B�����������Ďg���ĉ�����(^^;
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "UrlEncode.h"

void UrlEncode(const unsigned char	*csource,
						size_t	nSize,
						char** pOut,
						int bUseMalloc)
{
	if ( csource==NULL || csource==0 || csource==NULL)
		return;

	if ( bUseMalloc )
		*pOut = (char*)malloc(nSize*3 + 2);
	else
		*pOut = new char[nSize*3 + 2];
	if( *pOut==NULL )
		return;

	const unsigned char* cbuffer = (const unsigned char*)*pOut;

	unsigned long lbuffersize = nSize*3 + 2;
	unsigned long	llength;										/* csource �̃T�C�Y���i�[ */
	unsigned long	lcount = 0;										/* csource �̓ǂݍ��݈ʒu�J�E���^ */
	unsigned char	cbyte;											/* �����o���ꂽ 1 �o�C�g���̃f�[�^���i�[ */
	unsigned char	ctemp[4];										/* �ϊ�����(1 ������)�ꎞ�i�[�o�b�t�@ */
	unsigned long	lresultcount = 0;								/* cbuffer �̏������݈ʒu�J�E���^ */

	llength = nSize;						/* csource �̕����T�C�Y�𓾂� */
	if(!llength) { return ; }							/* csource �� 0 �����̏ꍇ�A�֐��𔲂��� */
	if(lbuffersize < (llength * 3 + 1)) { return ; }	/* �o�b�t�@�T�C�Y������Ȃ��ꍇ�A�֐��𔲂��� */

	for(;;) {
		cbyte = *(csource + lcount);								/* 1 �o�C�g�𔲂��o�� */
		if( ((cbyte >= 0x81) && (cbyte <= 0x9F)) ||
			((cbyte >= 0xE0) && (cbyte <= 0xEF)) ) {				/* Shift-JIS 2 �o�C�g�����������ꍇ */
			sprintf((char*)ctemp, "%%%02X", cbyte);						/* URL �G���R�[�h(��ʃo�C�g) */
			strncpy((char*)cbuffer + lresultcount, (char*)ctemp, 4);				/* cbuffer �ɃR�s�[ */
			lcount++;												/* �ǂݍ��݃J�E���^���C���N�������g */
			lresultcount += 3;										/* �������݃J�E���^�� 3 ���₷ */
			if(lcount == llength) { break; }						/* ������̏I�[�ɒB�����ꍇ�A���[�v�𔲂��� */
			sprintf((char*)ctemp, "%%%02X", *(csource + lcount));			/* URL �G���R�[�h(���ʃo�C�g) */
			strncpy((char*)cbuffer + lresultcount, (char*)ctemp, 4);				/* cbuffer �ɃR�s�[ */
			lcount++;												/* �ǂݍ��݃J�E���^���C���N�������g */
			lresultcount += 3;										/* �������݃J�E���^�� 3 ���₷ */
		} else if(cbyte == 0x20) {									/* 1 �o�C�g���p�X�y�[�X(" ")�������ꍇ */
			strncpy((char*)cbuffer + lresultcount, "+", 2);				/* "+" �� cbuffer �ɃR�s�[ */
			lcount++;												/* �ǂݍ��݃J�E���^���C���N�������g */
			lresultcount++;											/* �������݃J�E���^���C���N�������g */
		} else if( ((cbyte >= 0x40) && (cbyte <= 0x5A)) ||			/* @A-Z */
				   ((cbyte >= 0x61) && (cbyte <= 0x7A)) ||			/* a-z */
				   ((cbyte >= 0x30) && (cbyte <= 0x39)) ||			/* 0-9 */
				   (cbyte == 0x2A) ||								/* "*" */
				   (cbyte == 0x2D) ||								/* "-" */
				   (cbyte == 0x2E) ||								/* "." */
				   (cbyte == 0x5F) ) {								/* "_" */ /* ���ϊ������������ꍇ */
			strncpy((char*)cbuffer + lresultcount, (char*)csource + lcount, 2);	/* ���̂܂� cbuffer �ɃR�s�[ */
			lcount++;												/* �ǂݍ��݃J�E���^���C���N�������g */
			lresultcount++;											/* �������݃J�E���^���C���N�������g */
		} else {													/* ���̑��̕����̏ꍇ */
			sprintf((char*)ctemp, "%%%02X", cbyte);						/* URL �G���R�[�h */
			strncpy((char*)cbuffer + lresultcount, (char*)ctemp, 4);				/* cbuffer �ɃR�s�[ */
			lcount++;												/* �ǂݍ��݃J�E���^���C���N�������g */
			lresultcount += 3;										/* �������݃J�E���^�� 3 ���₷ */
		}
		if(lcount == llength) { break; }							/* ������̏I�[�ɒB�����ꍇ�A���[�v�𔲂��� */
	}
	return ;
}















// http://www.emoticode.net/c/urlencode-in-plain-c.html
static char hex[] = "0123456789abcdef";

char i2a(char code) {
	return hex[code & 15];
}

char *urlencodenew(char *pstr)
{
	char
		*buf,
		*pbuf;

	pbuf = buf = (char *)malloc(strlen(pstr) * 3 + 1);

	while (*pstr) {
		if ((*pstr >0 && isalnum(*pstr)) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
			*pbuf++ = *pstr;
		}
		else if (*pstr == ' ') {
			*pbuf++ = '+';
		}
		else {
			*pbuf++ = '%';
			*pbuf++ = i2a(*pstr >> 4);
			*pbuf++ = i2a(*pstr & 15);
		}
		pstr++;
	}
	*pbuf = '\0';

	return buf;
}

char *urlencodenew2(char *pstr, size_t size)
{
	char
		*buf,
		*pbuf;

	pbuf = buf = (char *)malloc(size * 3 + 1);

	while (size--) {
		if ((*pstr >0 && isalnum(*pstr)) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') {
			*pbuf++ = *pstr;
		}
		else if (*pstr == ' ') {
			*pbuf++ = '+';
		}
		else {
			*pbuf++ = '%';
			*pbuf++ = i2a(*pstr >> 4);
			*pbuf++ = i2a(*pstr & 15);
		}
		pstr++;
	}
	*pbuf = '\0';

	return buf;
}


static char a2ibyte(char c)
{
	switch (c) {
	case '0': return 0x0;
	case '1': return 0x1;
	case '2': return 0x2;
	case '3': return 0x3;
	case '4': return 0x4;
	case '5': return 0x5;
	case '6': return 0x6;
	case '7': return 0x7;
	case '8': return 0x8;
	case '9': return 0x9;
	case 'a':case 'A': return 0xa;
	case 'b':case 'B': return 0xb;
	case 'c':case 'C': return 0xc;
	case 'd':case 'D': return 0xd;
	case 'e':case 'E': return 0xe;
	case 'f':case 'F': return 0xf;
	}
	return 0;
}
static char a2i(char c1, char c2)
{
	return (a2ibyte(c1) << 4) | (a2ibyte(c2));
}
unsigned char* urldecode(const char* penc, unsigned int* pSize)
{
	unsigned int size = 0;
	unsigned char* pOrig = (unsigned char*)malloc(strlen(penc) + 1);
	unsigned char* p = pOrig;
	for (; *penc; ++penc, ++p, ++size)
	{
		char c = *penc;
		if (c == '%')
		{
			penc++;
			if (!*penc)
				break;
			char c1, c2;
			c1 = *penc;
			penc++;
			if (!*penc)
				break;

			c2 = *penc;
			c = a2i(c1, c2);
		}

		*p = c;
	}
	*p = 0;
	*pSize = size;
	return pOrig;
}
