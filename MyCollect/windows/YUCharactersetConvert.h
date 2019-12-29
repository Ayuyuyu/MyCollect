#pragma once
//#include <stdio.h>   
//#include <windows.h>   
#include <locale.h>   
#define BUFF_SIZE 1024   
//ʹ�����Ƽ�ʹ��windows API

class CCharactersetConvert
{
public:
	CCharactersetConvert(void);
	~CCharactersetConvert(void);
public:
	static wchar_t * ANSIToUnicode( const char* str );   
	static char * UnicodeToANSI( const wchar_t* str );   
	static wchar_t * UTF8ToUnicode( const char* str );   
	static char * UnicodeToUTF8( const wchar_t* str ); 	 
	static char* w2m(const wchar_t* wcs);			/*���ַ�ת��Ϊ���ַ�Unicode - ANSI*/ 
	static wchar_t* m2w(const char* mbs);   		/*���ַ�ת��Ϊ���ַ�ANSI - Unicode*/  
	static char* ANSIToUTF8(const char* str);
	static char* UTF8ToANSI(const char* str);   
};