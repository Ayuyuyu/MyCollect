#pragma once
#include <windows.h>
#include <tlhelp32.h>   
#include "Process.h"



DWORD FindProcess(TCHAR* strExeName)
{
	DWORD dwPID = 0;
	int i = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		i += 0;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		//printf(" 进程名称：%s \n", pe32.szExeFile);  
		if (_wcsicmp(strExeName, pe32.szExeFile) == 0)
		{
			//printf("进程运行中");  
			dwPID = pe32.th32ProcessID;
			i += 1;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	if (i > 0)      
		return dwPID;
	return 0;
}



HANDLE FindProcessHandle(TCHAR* strExeName)
{
	DWORD dwPID = 0;
	int i = 0;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		i += 0;
	}
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		//printf(" 进程名称：%s \n", pe32.szExeFile);  
		if (_wcsicmp(strExeName, pe32.szExeFile) == 0)
		{

			return ::OpenProcess(PROCESS_ALL_ACCESS, false, pe32.th32ProcessID);
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	return 0;
}

bool EnableDebugPrivilege()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return   FALSE;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}
	return true;
}

BOOL SendPassWord(FILEMAP stcMap)
{
	HANDLE hMapHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT("MYWindowsFileMap"));
	if (NULL == hMapHandle)
	{
		hMapHandle = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, TEXT("MYWindowsFileMap"));
		if (NULL == hMapHandle)
		{
			OutputDebugString(TEXT("CreateFileMapping file Process."));
			return FALSE;
		}
	}
	LPTSTR lpMapAddr = (LPTSTR)MapViewOfFile(hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (NULL == lpMapAddr)
	{
		OutputDebugString(TEXT("MapViewOfFile file Process."));
		return FALSE;
	}
	memcpy(lpMapAddr, &stcMap, sizeof(FILEMAP));
	return TRUE;
}