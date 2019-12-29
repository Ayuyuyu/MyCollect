#pragma once

#include "FilterFile.h"


CHunter::CHunter() :dwFilterCount(0), dwFileCount(0)
{
	headNode = (FileList *)malloc(sizeof(FileList));
	headNode->NextFile = NULL;
	currNode = headNode;
	for (int i = 0; i < CONST_MAX_FILTER; i++)
		ZeroMemory(szFilter[i], 5);
}

CHunter::~CHunter()
{
	PFileList next, tmp;
	tmp = headNode;

	while (tmp->NextFile != NULL)
	{
		next = tmp->NextFile;
		free(tmp);
		tmp = next;
	}
	free(tmp);
}

//添加过滤器，比如.txt，为了简省，没有考虑文件扩展名长度大于4的情况 
//请自行修改 
void CHunter::AddFilter(TCHAR *szInp)
{
	// 	if (wcslen(szInp) > 4)
	// 		return;
	wcsncpy_s(szFilter[dwFilterCount++], szInp, 100);
}

void CHunter::AddFile(TCHAR *szFile)
{
	currNode->NextFile = (FileList *)malloc(sizeof(FileList));
	currNode = currNode->NextFile;
	ZeroMemory(currNode->szFile, MAX_PATH);
	currNode->NextFile = NULL;
	wcsncpy_s(currNode->szFile, szFile, MAX_PATH);
	dwFileCount++;
}

//这是入口函数，调用它即可开始搜索，这个路径的尾部不应有反斜杠 
void CHunter::Hunt(TCHAR *szPath)
{
	HuntFile(szPath);
	currNode = headNode->NextFile;
}

DWORD CHunter::GetFileCount()
{
	return this->dwFileCount;
}

//这个函数依次遍历链表中的文件,并返回一个文件名 
TCHAR *CHunter::GetFile()
{
	TCHAR *szRet;
	szRet = currNode->szFile;
	currNode = currNode->NextFile;
	return szRet;
}

void CHunter::HuntFile(TCHAR * lpPath)
{
	TCHAR szFind[MAX_PATH];
	TCHAR szFile[MAX_PATH];
	WIN32_FIND_DATA FindFileData;

	ZeroMemory(szFind, MAX_PATH);
	ZeroMemory(szFile, MAX_PATH);
	wcscpy_s(szFind, lpPath);
	wcscat_s(szFind, TEXT("\\*.*"));

	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)        
		return;

	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				wcscpy_s(szFile, lpPath);
				wcscat_s(szFile, TEXT("\\"));
				wcscat_s(szFile, FindFileData.cFileName);
				HuntFile(szFile);
			}
		}
		else
		{
			wcscpy_s(szFile, lpPath);
			wcscat_s(szFile, TEXT("\\"));
			wcscat_s(szFile, FindFileData.cFileName);
			//cout << szFile << FindFileData.cFileName << endl; 
			for (int i = 0; i < dwFilterCount; i++)
			if (wcswcs(szFile, szFilter[i]) != 0)
				this->AddFile(szFile);
		}
		if (!FindNextFile(hFind, &FindFileData))        
			break;
	}
	FindClose(hFind);
}