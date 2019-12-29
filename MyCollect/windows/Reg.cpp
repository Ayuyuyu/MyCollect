
#include "Reg.h"  

/*============================================================
* �� �� ����CRegOperator
* �Ρ�������HKEY [IN] : Ĭ����HKEY_LOCAL_MACHINE
* �������������캯������ʼ������
* �� �� ֵ����
*============================================================*/

CRegOperator::CRegOperator(HKEY hKey)
{
	m_hKey = HKEY_LOCAL_MACHINE;
}

/*============================================================
* �� �� ����~CRegOperator
* �Ρ�������NULL [IN]
* ���������������������رմ򿪵�ע�����
*============================================================*/

CRegOperator::~CRegOperator()
{
	Close();
}

/*============================================================
* �� �� ����Close
* �Ρ�������NULL [IN]
* �����������رմ򿪼��ľ��
*============================================================*/

void CRegOperator::Close()
{
	if (m_hKey)
	{
		/* ����ǿս��йر� */
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
}

/*============================================================
* �� �� ����SetHKEY
* �Ρ�������LPCTSTR [IN] : ����ֵ
* �����������ɴ���ĸ����ַ������õ�ǰ�����ĸ�����ֵ
*============================================================*/

BOOL CRegOperator::SetHKEY(TCHAR* strKey)
{
	assert(m_hKey);
	assert(strKey);

	/* ������бȽ� */
	if (0 == wcscmp(strKey, L"HKEY_CLASSES_ROOT"))
	{
		m_hKey = HKEY_CLASSES_ROOT;
		return TRUE;
	}
	if (0 == wcscmp(strKey, L"HKEY_CURRENT_USER"))
	{
		m_hKey = HKEY_CURRENT_USER;
		return TRUE;
	}
	if (0 == wcscmp(strKey, L"HKEY_LOCAL_MACHINE"))
	{
		m_hKey = HKEY_LOCAL_MACHINE;
		return TRUE;
	}
	if (0 == wcscmp(strKey, L"HKEY_USERS"))
	{
		m_hKey = HKEY_USERS;
		return TRUE;
	}
	if (0 == wcscmp(strKey, L"HKEY_PERFORMANCE_DATA"))
	{
		m_hKey = HKEY_PERFORMANCE_DATA;
		return TRUE;
	}
	if (0 == wcscmp(strKey, L"HKEY_PERFORMANCE_TEXT"))
	{
		m_hKey = HKEY_PERFORMANCE_TEXT;
		return TRUE;
	}
	if (0 == wcscmp(strKey, L"HKEY_PERFORMANCE_NLSTEXT"))
	{
		m_hKey = HKEY_PERFORMANCE_NLSTEXT;
		return TRUE;
	}
	/* �Բ���ϵͳ�汾���в��� */
#if(WINVER >= 0x0400)  
	if (0 == wcscmp(strKey, L"HKEY_CURRENT_CONFIG"))
	{
		m_hKey = HKEY_CURRENT_CONFIG;
		return TRUE;
	}
	if (0 == wcscmp(strKey, L"HKEY_DYN_DATA"))
	{
		m_hKey = HKEY_DYN_DATA;
		return TRUE;
	}
#endif  

	return FALSE;
}

/*============================================================
* �� �� ����OpenKey
* �Ρ�������LPCTSTR [IN] : �Ӽ��ַ���
* ����������ͨ�������Ӽ����ַ�����ע�����Ӧ��λ��
*============================================================*/

BOOL CRegOperator::OpenKey(TCHAR* lpSubKey)
{
	assert(m_hKey);
	assert(lpSubKey);

	HKEY hKey;
	long lReturn = RegOpenKeyEx(m_hKey, lpSubKey, 0L, KEY_READ | KEY_WRITE | KEY_EXECUTE, &hKey);

	if (ERROR_SUCCESS == lReturn)
	{
		/* �ɹ����򽫴򿪵ľ������ */
		m_hKey = hKey;

		return TRUE;
	}
	/* ��ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����CreateKey
* �Ρ�������LPCTSTR [IN] : �Ӽ��ַ���
* ����������ͨ�������Ӽ����ַ����򿪣������Ӽ������ߴ������������Ӽ�����Ӧ���Ӽ�
*============================================================*/

BOOL CRegOperator::CreateKey(TCHAR* lpSubKey)
{
	assert(m_hKey);
	assert(lpSubKey);

	HKEY hKey;
	DWORD dw;
	long lReturn = RegCreateKeyEx(m_hKey, lpSubKey, 0L, NULL, REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw);

	if (ERROR_SUCCESS == lReturn)
	{
		/* �ɹ��򿪻��ߴ����򽫾������ */
		m_hKey = hKey;

		return TRUE;
	}
	/* �򿪻��ߴ���ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����DeleteKey
* �Ρ�������LPCTSTR LPCTSTR [IN] : ����ֵ �Ӽ�ֵ
* ����������ͨ������ĸ������Ӽ������Ӽ�ɾ��ɾ�������ܰ����Ӽ�
*============================================================*/

//BOOL CRegOperator::DeleteKey( HKEY hKey, LPCTSTR lpSubKey )  
BOOL CRegOperator::DeleteKey(TCHAR* lphKey, LPCTSTR lpSubKey)
{
	assert(lphKey);
	assert(lpSubKey);
	assert(m_hKey);

	SetHKEY(lphKey);
	long lReturn = RegDeleteValue(m_hKey, lpSubKey);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ɾ���ɹ� */
		return TRUE;
	}
	/* ɾ��ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����DeleteValue
* �Ρ�������LPCTSTR [IN] : Ҫɾ����ֵ������
* ����������ͨ�������ֵ���ƣ�ɾ����Ӧ�ļ�ֵ
*============================================================*/

BOOL CRegOperator::DeleteValue(TCHAR* lpValueName)
{
	assert(m_hKey);
	assert(lpValueName);

	long lReturn = RegDeleteValue(m_hKey, lpValueName);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ɾ���ɹ� */
		return TRUE;
	}
	/* ɾ��ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����SaveKey
* �Ρ�������LPCTSTR [IN] : ��������ļ���
* ����������ͨ��������ļ����ƣ������Ӧ���Ӽ�
*============================================================*/

BOOL CRegOperator::SaveKey(TCHAR* lpFileName)
{
	assert(m_hKey);
	assert(lpFileName);

	long lReturn = RegSaveKey(m_hKey, lpFileName, NULL);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ����ɹ� */
		return TRUE;
	}
	/* ����ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����RestoreKey
* �Ρ�������LPCTSTR [IN] : ���ָ����ļ���
* ����������ͨ���ļ����ƣ������е���ע�����
*============================================================*/

BOOL CRegOperator::RestoreKey(TCHAR* lpFileName)
{
	assert(m_hKey);
	assert(lpFileName);

	long lReturn = RegRestoreKey(m_hKey, lpFileName, 0);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ����ɹ� */
		return TRUE;
	}
	/* ����ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����Read
* �Ρ�������LPCTSTR CString [IN] : ��ֵ ��ȡֵ������
* ������������ָ��λ�õ�CString���͵�ֵ����
*============================================================*/

BOOL CRegOperator::Read(TCHAR* lpValueName, wstring* lpVal)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(lpVal);

	DWORD dwType;
	DWORD dwSize = 200;
	TCHAR szString[2048];
	memset(szString, 0, 2048 * sizeof(TCHAR));

	long lReturn = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE *)szString, &dwSize);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ��ѯ�ɹ� */
		*lpVal = szString;

		return TRUE;
	}
	/* ��ѯʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����Read
* �Ρ�������LPCTSTR DWORD [IN] : ��ֵ ��ȡֵ������
* ������������ָ��λ��DWORD���͵�ֵ����
*============================================================*/

BOOL CRegOperator::Read(TCHAR* lpValueName, DWORD* pdwVal)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(pdwVal);

	DWORD dwType;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwDest;
	long lReturn = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE *)&dwDest, &dwSize);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ��ѯ�ɹ� */
		*pdwVal = dwDest;

		return TRUE;
	}
	/* ��ѯʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����Read
* �Ρ�������LPCTSTR int [IN] : ��ֵ ��ȡֵ������
* ������������ָ��λ��int���͵�ֵ����
*============================================================*/

BOOL CRegOperator::Read(TCHAR* lpValueName, int* pnVal)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(pnVal);

	DWORD dwType;
	DWORD dwSize = sizeof(DWORD);
	DWORD dwDest;
	long lReturn = RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType, (BYTE *)&dwDest, &dwSize);

	if (ERROR_SUCCESS == lReturn)
	{
		/* ��ѯ�ɹ� */
		*pnVal = (int)dwDest;

		return TRUE;
	}

	return FALSE;
}

/*============================================================
* �� �� ����Write
* �Ρ�������LPCTSTR LPCTSTR [IN] : ��ֵ д��ֵ������
* ������������LPCTSTR���͵�ֵд��ָ��λ��
*============================================================*/

BOOL CRegOperator::Write(TCHAR* lpValueName, char* lpValue)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(lpValue);

	long lReturn = RegSetValueEx(m_hKey, lpValueName, 0L, REG_SZ, (const BYTE *)lpValue, strlen(lpValue) + 1);

	if (ERROR_SUCCESS == lReturn)
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����Write
* �Ρ�������LPCTSTR DWORD [IN] : ��ֵ д��ֵ������
* ������������DWORD���͵�ֵд��ָ��λ��
*============================================================*/

BOOL CRegOperator::Write(TCHAR* lpSubKey, DWORD dwVal)
{
	assert(m_hKey);
	assert(lpSubKey);

	long lReturn = RegSetValueEx(m_hKey, lpSubKey, 0L, REG_DWORD, (const BYTE *)&dwVal, sizeof(DWORD));

	if (ERROR_SUCCESS == lReturn)
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;
}

/*============================================================
* �� �� ����Write
* �Ρ�������LPCTSTR int [IN] : ��ֵ д��ֵ������
* ������������������д��ָ��λ��
*============================================================*/

BOOL CRegOperator::Write(TCHAR* lpSubKey, int nVal)
{
	assert(m_hKey);
	assert(lpSubKey);

	DWORD dwValue;
	dwValue = (DWORD)nVal;

	long lReturn = RegSetValueEx(m_hKey, lpSubKey, 0L, REG_DWORD, (const BYTE *)&dwValue, sizeof(DWORD));

	if (ERROR_SUCCESS == lReturn)
	{
		/* �ɹ�д�� */
		return TRUE;
	}
	/* д��ʧ�� */
	return FALSE;
}
