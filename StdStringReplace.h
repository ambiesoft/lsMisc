////////////////////////////////////////////////////////////////////
//http://www20.big.or.jp/~koma/dir_WEB/contents/program/012.html
//������u��
//�֐���	�F�@Replace()
//����		�F�@string&�^�@src�@�u���Ώۂ̕�����I�u�W�F�N�g
//		�F�@const char*�^�@pszKey �u���ΏےP��
//		�F�@const char*�^�@pszRepWord �u����P��
//�߂�l	�F�@�Ȃ�

#include "tstring.h"

void StdStringReplaceW(std::wstring& src, LPCWSTR pszKey, LPCWSTR pszRepWord);
void StdStringReplaceA(std::string& src, LPCSTR pszKey, LPCSTR pszRepWord);

#ifdef UNICODE
#else
#endif