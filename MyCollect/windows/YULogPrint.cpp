#include "StdAfx.h"
#include "YULogPrint.h"


CLogPrint::CLogPrint(void)
{
}


CLogPrint::~CLogPrint(void)
{
}

/************************以16进制打印**************************
*/
void CLogPrint::PrintChbuf(const char *String,char *chbuf,int len)
{
	char temp[20];
	std::string stu(String);


	for (int i=0;i<len;i++)
	{
		memset(temp,0,20);
		sprintf(temp,"%02X ",(UCHAR)chbuf[i]);
		stu+=temp;
	}

	stu+="\n";
	PrintfLog(stu.c_str());

}

/************************写txt Log**************************
*/
void CLogPrint::WriteLog(const char *FilePath,const char *m_string)
{
	FILE *pfile=fopen(FilePath,"a+");
	if (pfile==NULL)
		return ;

	char temp[100];
	memset(temp,0,sizeof(temp));

	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(temp,"%04d-%02d-%02d %02d:%02d:%02d -> ",st.wYear,st.wMonth,st.wDay,\
		st.wHour,st.wMinute,st.wSecond);

	fwrite(temp,1,strlen(temp),pfile);

	fwrite(m_string,1,strlen(m_string),pfile);

	fclose(pfile);
}

/************************打印信息并且写txt Log**************************
*/
void CLogPrint::PrintfLog(const char *format, ...)
{

	va_list argptr;
	char buffer[1024];
	int re=0;

	memset(buffer,0,sizeof(buffer));

	va_start(argptr, format);
	re=vsprintf(buffer,format,argptr); 
	va_end(argptr);

	if(re>=0)
	{
		OutputDebugStringA(buffer);
#if PRT_DEBUG
printf(buffer);
#endif		
#if WRITE_FILE_ERRORLOG	
		WriteLog(ErrorPath,buffer);
#endif
	}
}
/************************格式化打印调试信息**************************
*/
void CLogPrint::TRACE_WIN32(const char *format, ...)
{
	va_list argptr;
	char buffer[1024];
	int re=0;

	memset(buffer,0,sizeof(buffer));

	va_start(argptr, format);
	re=vsprintf(buffer,format,argptr); 
	va_end(argptr);

	if(re>=0)
	{
		OutputDebugStringA(buffer);
		printf(buffer);
	}
}