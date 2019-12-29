
#pragma once
#include "StdAfx.h"
#include <assert.h>  


#ifndef REGOP_H  
#define REGOP_H  

class CRegOperator
{
private:
	HKEY m_hKey;                                                    /* 保存当前操作根键的句柄                      */

public:
	CRegOperator(HKEY hKey = HKEY_LOCAL_MACHINE);                 /* 构造函数，默认参数为：HKEY_LOCAL_MACHINE        */
	BOOL SetHKEY(TCHAR* strKey);                                 /* 设置当前操作的根键                            */
	BOOL OpenKey(TCHAR* lpSubKey);                               /* 读写的形式打开注册表                           */
	void Close();                                                   /* 关闭键句柄                                    */

	BOOL CreateKey(TCHAR* lpSubKey);                             /* 创建的形式打开注册表                           */
	BOOL DeleteKey(TCHAR* lphKey, LPCTSTR lpSubKey);             /* 删除相应的子键（子键为空）                    */
	BOOL DeleteValue(TCHAR* lpValueName);                        /* 删除子键处的相应的键值                      */
	BOOL SaveKey(TCHAR* lpFileName);                             /* 把当前键值保存到指定文件                     */
	BOOL RestoreKey(TCHAR* lpFileName);                          /* 从指定注册表文件中恢复                      */

	BOOL Read(TCHAR* lpValueName, wstring* lpVal);               /* 读出REG_SZ类型的值                         */
	BOOL Read(TCHAR* lpValueName, DWORD* pdwVal);                /* 读出DWORD类型的值                          */
	BOOL Read(TCHAR* lpValueName, int* pnVal);                   /* 读出INT类型的值                                */
	BOOL Write(TCHAR* lpSubKey, char* lpVal);                  /* 写入REG_SZ类型值                              */
	BOOL Write(TCHAR* lpSubKey, DWORD dwVal);                    /* 写入DWORD类型值                               */
	BOOL Write(TCHAR* lpSubKey, int nVal);                       /* 写入INT类型值                             */

	virtual ~CRegOperator();
};

#endif  