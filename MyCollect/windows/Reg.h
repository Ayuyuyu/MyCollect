
#pragma once
#include "StdAfx.h"
#include <assert.h>  


#ifndef REGOP_H  
#define REGOP_H  

class CRegOperator
{
private:
	HKEY m_hKey;                                                    /* ���浱ǰ���������ľ��                      */

public:
	CRegOperator(HKEY hKey = HKEY_LOCAL_MACHINE);                 /* ���캯����Ĭ�ϲ���Ϊ��HKEY_LOCAL_MACHINE        */
	BOOL SetHKEY(TCHAR* strKey);                                 /* ���õ�ǰ�����ĸ���                            */
	BOOL OpenKey(TCHAR* lpSubKey);                               /* ��д����ʽ��ע���                           */
	void Close();                                                   /* �رռ����                                    */

	BOOL CreateKey(TCHAR* lpSubKey);                             /* ��������ʽ��ע���                           */
	BOOL DeleteKey(TCHAR* lphKey, LPCTSTR lpSubKey);             /* ɾ����Ӧ���Ӽ����Ӽ�Ϊ�գ�                    */
	BOOL DeleteValue(TCHAR* lpValueName);                        /* ɾ���Ӽ�������Ӧ�ļ�ֵ                      */
	BOOL SaveKey(TCHAR* lpFileName);                             /* �ѵ�ǰ��ֵ���浽ָ���ļ�                     */
	BOOL RestoreKey(TCHAR* lpFileName);                          /* ��ָ��ע����ļ��лָ�                      */

	BOOL Read(TCHAR* lpValueName, wstring* lpVal);               /* ����REG_SZ���͵�ֵ                         */
	BOOL Read(TCHAR* lpValueName, DWORD* pdwVal);                /* ����DWORD���͵�ֵ                          */
	BOOL Read(TCHAR* lpValueName, int* pnVal);                   /* ����INT���͵�ֵ                                */
	BOOL Write(TCHAR* lpSubKey, char* lpVal);                  /* д��REG_SZ����ֵ                              */
	BOOL Write(TCHAR* lpSubKey, DWORD dwVal);                    /* д��DWORD����ֵ                               */
	BOOL Write(TCHAR* lpSubKey, int nVal);                       /* д��INT����ֵ                             */

	virtual ~CRegOperator();
};

#endif  