#include "StdAfx.h"
#include "YUCharactersetConvert.h"


CCharactersetConvert::CCharactersetConvert(void)
{
}


CCharactersetConvert::~CCharactersetConvert(void)
{
}

wchar_t* CCharactersetConvert::ANSIToUnicode( const char* str )   
{   
	int textlen ;   
	wchar_t * result;   
	textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 );   
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));   
	memset(result,0,(textlen+1)*sizeof(wchar_t));   
	MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen );   
	return result;   
}   

char* CCharactersetConvert::UnicodeToANSI( const wchar_t* str )   
{   
	char* result;   
	int textlen;   
	textlen = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL );   
	result =(char *)malloc((textlen+1)*sizeof(char));   
	memset( result, 0, sizeof(char) * ( textlen + 1 ) );   
	WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL );   
	return result;   
}   

wchar_t* CCharactersetConvert::UTF8ToUnicode( const char* str )   
{   
	int textlen ;   
	wchar_t * result;   
	textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 );   
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));   
	memset(result,0,(textlen+1)*sizeof(wchar_t));   
	MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen );   
	return result;   
}   

char* CCharactersetConvert::UnicodeToUTF8( const wchar_t* str )   
{   
	char* result;   
	int textlen;   
	textlen = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL );   
	result =(char *)malloc((textlen+1)*sizeof(char));   
	memset(result, 0, sizeof(char) * ( textlen + 1 ) );   
	WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );   
	return result;   
}   

/*���ַ�ת��Ϊ���ַ�Unicode - ANSI*/  
char* CCharactersetConvert::w2m(const wchar_t* wcs)   
{   
	int len;   
	char* buf;   
	len =wcstombs(NULL,wcs,0);   
	if (len == 0)   
		return NULL;   
	buf = (char *)malloc(sizeof(char)*(len+1));   
	memset(buf, 0, sizeof(char) *(len+1));   
	len =wcstombs(buf,wcs,len+1);   
	return buf;   
}   
/*���ַ�ת��Ϊ���ַ�ANSI - Unicode*/  
wchar_t* CCharactersetConvert::m2w(const char* mbs)   
{   
	int len;   
	wchar_t* buf;   
	len =mbstowcs(NULL,mbs,0);   
	if (len == 0)   
		return NULL;   
	buf = (wchar_t *)malloc(sizeof(wchar_t)*(len+1));   
	memset(buf, 0, sizeof(wchar_t) *(len+1));   
	len =mbstowcs(buf,mbs,len+1);   
	return buf;   
}   

char* CCharactersetConvert::ANSIToUTF8(const char* str)   
{   
	return UnicodeToUTF8(ANSIToUnicode(str));   
}   

char* CCharactersetConvert::UTF8ToANSI(const char* str)   
{   
	return UnicodeToANSI(UTF8ToUnicode(str));   
}   