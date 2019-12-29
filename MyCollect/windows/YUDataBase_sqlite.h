#pragma once
#include "stdafx.h"
#include <sqlite3/sqlite3.h>
#pragma comment(lib,"sqlite3.lib")



typedef int (*PSelectCallback)(LPVOID pCall,int RowCout,LPTSTR *lpValues,LPTSTR *lpFields);

class CDataBase_sqlite
{
public:
	CDataBase_sqlite();
	~CDataBase_sqlite();
public:
	long    Open(CString strDBPath);
	long    IsTableExist(CString cat);
	long    sql_run(CString sql);
	long    Close();
	int     Select(CString sql,PSelectCallback lpCallBack,LPVOID lpThis);
	CString GetHistoryDBPath();
private:
	sqlite3 *m_SQL;
};

