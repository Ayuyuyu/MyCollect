#pragma once
#include <shlwapi.h> 
#pragma comment(lib,"shlwapi.lib") 

using namespace std;

//������������������ 
#define CONST_MAX_FILTER 16
//��������ݽṹ 
typedef struct tagList{
	TCHAR szFile[MAX_PATH];
	struct tagList *NextFile;
}FileList, *PFileList;
//������ 
class CHunter{
public:
	CHunter();
	~CHunter();

	void AddFilter(TCHAR *szFilter);//��ӹ����� 
	void CHunter::Hunt(TCHAR *szPath);
	TCHAR *GetFile();//ȡ�������е��ļ� 
	DWORD    GetFileCount();//ȡ���ļ������� 

private:
	PFileList headNode;//����ͷ 
	PFileList currNode;

	void AddFile(TCHAR *szFile);
	void HuntFile(TCHAR *lpPath);

	TCHAR    szFilter[CONST_MAX_FILTER][100];
	DWORD    dwFilterCount;
	DWORD    dwFileCount;
};
