#pragma once

typedef struct _FILEMAP
{
	BYTE bFlag;
	char strPW[20];
}FILEMAP, *PFILEMAP;

DWORD FindProcess(TCHAR* strExeName);
HANDLE FindProcessHandle(TCHAR* strExeName);
bool EnableDebugPrivilege();
BOOL SendPassWord(FILEMAP stcMap);