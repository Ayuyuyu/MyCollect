#pragma once

#define  PRT_DEBUG 1;								//��ʾд���
#define  WRITE_FILE_ERRORLOG 1;			//�ļ�д���

#define ErrorPath	"ErrorLog.txt"

class CLogPrint
{
public:
	CLogPrint(void);
	~CLogPrint(void);
public:
	void PrintChbuf(const char *String,char *chbuf,int len);
	void WriteLog(const char *FilePath,const char *m_string);
	void PrintfLog(const char *format, ...);
	void TRACE_WIN32(const char *format, ...);
};

