#ifndef MINIDUMPER_H
#define MINIDUMPER_H

#include <windows.h>

class CMiniDumper
{
public:

    CMiniDumper(bool bPromptUserForMiniDump);
    ~CMiniDumper(void);

private:

    static LONG WINAPI unhandledExceptionHandler(struct _EXCEPTION_POINTERS *pExceptionInfo);
    void setMiniDumpFileName(void);
    bool getImpersonationToken(HANDLE* phToken);
    BOOL enablePrivilege(LPCTSTR pszPriv, HANDLE hToken, TOKEN_PRIVILEGES* ptpOld);
    BOOL restorePrivilege(HANDLE hToken, TOKEN_PRIVILEGES* ptpOld);
    LONG writeMiniDump(_EXCEPTION_POINTERS *pExceptionInfo );
	void DisableSetUnhandledExceptionFilter();

    _EXCEPTION_POINTERS *m_pExceptionInfo;
    TCHAR m_szMiniDumpPath[MAX_PATH];
    TCHAR m_szAppPath[MAX_PATH];
    TCHAR m_szAppBaseName[MAX_PATH];

	CRITICAL_SECTION m_cs;
};

#endif // MINIDUMPER_H
