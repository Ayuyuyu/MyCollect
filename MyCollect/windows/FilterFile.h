#pragma once
#include <shlwapi.h> 
#pragma comment(lib,"shlwapi.lib") 

using namespace std;

//定义过滤器的最大数量 
#define CONST_MAX_FILTER 16
//链表的数据结构 
typedef struct tagList{
	TCHAR szFile[MAX_PATH];
	struct tagList *NextFile;
}FileList, *PFileList;
//主体类 
class CHunter{
public:
	CHunter();
	~CHunter();

	void AddFilter(TCHAR *szFilter);//添加过滤器 
	void CHunter::Hunt(TCHAR *szPath);
	TCHAR *GetFile();//取得链表中的文件 
	DWORD    GetFileCount();//取得文件的数量 

private:
	PFileList headNode;//链表头 
	PFileList currNode;

	void AddFile(TCHAR *szFile);
	void HuntFile(TCHAR *lpPath);

	TCHAR    szFilter[CONST_MAX_FILTER][100];
	DWORD    dwFilterCount;
	DWORD    dwFileCount;
};
