#include "StdAfx.h"
#include "YUDataBase_sqlite.h"


CDataBase_sqlite::CDataBase_sqlite(void)
{
	m_SQL = NULL;
}


CDataBase_sqlite::~CDataBase_sqlite(void)
{
}


long CDataBase_sqlite::Open(CString strDBPath)
{
	long ret = -1;
	if(m_SQL == NULL)
	{
		if (strDBPath.IsEmpty())
			strDBPath = GetHistoryDBPath();
		ret = sqlite3_open16(strDBPath, &m_SQL);
		sql_run(L"PRAGMA page_size=8192;");
		sql_run(L"PRAGMA synchronous=OFF;");
		sql_run(L"PRAGMA encoding = \"UTF-16\";");
	}
	return ret;
}

long CDataBase_sqlite::IsTableExist(CString cat)
{
	long ret = 0;
	CString sql = L"";
	if (cat != L"")
		sql=L"select count(name) from sqlite_master where name=\""+cat+L"\"";
	const void *zLeftover; 
	sqlite3_stmt* st=NULL;
	ret=sqlite3_prepare16_v2(m_SQL,sql.GetBuffer(),-1,&st,&zLeftover);
	ret=sqlite3_step(st);
	sql=(wchar_t*)sqlite3_column_text16(st,0);
	ret=sqlite3_finalize(st);

	return _wtoi(sql);
}

long CDataBase_sqlite::sql_run(CString sql)
{
	long ret = 0;
	const void *zLeftover; 
	sqlite3_stmt *st=NULL;
	ret = sqlite3_prepare16_v2(m_SQL,sql.GetBuffer(),-1,&st,&zLeftover);
	sqlite3_step(st);
	sqlite3_finalize(st);

	return ret;
}

int CDataBase_sqlite::Select(CString sql,PSelectCallback lpCallBack,LPVOID lpThis)
{
	CString key;
	const void *zLeftover; 
	sqlite3_stmt* st=NULL;
	int re = 0;
	int ret=sqlite3_prepare16_v2(m_SQL,sql.GetBuffer(),-1,&st,&zLeftover);
	ret=sqlite3_bind_text16(st,1,key.GetBuffer(),key.GetLength()*sizeof TCHAR,SQLITE_STATIC);
	CString strFileds;
	CString strValues;
	for(;;)
	{	

		re=sqlite3_step(st);
		if(re!=100||sqlite3_column_count(st)==0)
			break;

		int ct=sqlite3_column_count(st);
		TCHAR **szFileds= new TCHAR*[ct];
		TCHAR **szValues = new TCHAR*[ct];
		for (int i=0;i<ct;i++)
			//for (int i=ct-1;i>=0;i--)
		{
			strFileds=(wchar_t*)sqlite3_column_name16(st,i);
			strValues=(wchar_t*)sqlite3_column_text16(st,i);
			szFileds[i]=new TCHAR[strFileds.GetLength()+1];
			szValues[i]=new TCHAR[strValues.GetLength()+1];
			wcscpy_s(szFileds[i],strFileds.GetLength()+1,strFileds.GetBuffer());
			wcscpy_s(szValues[i],strValues.GetLength()+1,strValues.GetBuffer());

		}
		if(lpCallBack!=NULL)
		{
			lpCallBack(lpThis,ct,szValues,szFileds);
		}
		delete[]szFileds;
		delete[]szValues;
		if(re==1)
			break;
	}
	sqlite3_finalize(st);
	return ret;
}

long CDataBase_sqlite::Close()
{
	long ret = sqlite3_close(m_SQL);
	m_SQL = NULL;

	return ret;
}

CString CDataBase_sqlite::GetHistoryDBPath()
{
	CString szBuff;
	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szPath, MAX_PATH);
	szBuff.Format(_T("%s\\data\\history.db"), szPath);

	return szBuff;
}