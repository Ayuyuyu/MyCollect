
#include "Reg.h"  

/*============================================================
* 函 数 名：CRegOperator
* 参　　数：HKEY [IN] : 默认是HKEY_LOCAL_MACHINE
* 功能描述：构造函数，初始化根键
* 返 回 值：无
*============================================================*/

CRegOperator::CRegOperator(HKEY hKey)
{
	m_hKey = HKEY_LOCAL_MACHINE;
}

/*============================================================
* 函 数 名：~CRegOperator
* 参　　数：NULL [IN]
* 功能描述：析构函数，关闭打开的注册表句柄
*============================================================*/

CRegOperator::~CRegOperator()
{
	Close();
}

/*============================================================
* 函 数 名：Close
* 参　　数：NULL [IN]
* 功能描述：关闭打开键的句柄
*============================================================*/

void CRegOperator::Close()
{
	if (m_hKey)
	{
		/* 句柄非空进行关闭 */
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
}

/*============================================================
* 函 数 名：SetHKEY
* 参　　数：LPCTSTR [IN] : 根键值
* 功能描述：由传入的根键字符串设置当前操作的根键的值
*============================================================*/

BOOL CRegOperator::SetHKEY(TCHAR* strKey)
{
	assert(m_hKey);
	assert(strKey);

	/* 逐个进行比较 */
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
	/* 对操作系统版本进行测试 */
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
* 函 数 名：OpenKey
* 参　　数：LPCTSTR [IN] : 子键字符串
* 功能描述：通过传入子键的字符串打开注册表相应的位置
*============================================================*/

BOOL CRegOperator::OpenKey(TCHAR* lpSubKey)
{
	assert(m_hKey);
	assert(lpSubKey);

	HKEY hKey;
	long lReturn = RegOpenKeyEx(m_hKey, lpSubKey, 0L, KEY_READ | KEY_WRITE | KEY_EXECUTE, &hKey);

	if (ERROR_SUCCESS == lReturn)
	{
		/* 成功打开则将打开的句柄保存 */
		m_hKey = hKey;

		return TRUE;
	}
	/* 打开失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：CreateKey
* 参　　数：LPCTSTR [IN] : 子键字符串
* 功能描述：通过传入子键的字符串打开（存在子键）或者创建（不存在子键）相应的子键
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
		/* 成功打开或者创建则将句柄保存 */
		m_hKey = hKey;

		return TRUE;
	}
	/* 打开或者创建失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：DeleteKey
* 参　　数：LPCTSTR LPCTSTR [IN] : 根键值 子键值
* 功能描述：通过传入的根键和子键，将子键删除删除，不能包含子键
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
		/* 删除成功 */
		return TRUE;
	}
	/* 删除失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：DeleteValue
* 参　　数：LPCTSTR [IN] : 要删除键值的名称
* 功能描述：通过传入键值名称，删除对应的键值
*============================================================*/

BOOL CRegOperator::DeleteValue(TCHAR* lpValueName)
{
	assert(m_hKey);
	assert(lpValueName);

	long lReturn = RegDeleteValue(m_hKey, lpValueName);

	if (ERROR_SUCCESS == lReturn)
	{
		/* 删除成功 */
		return TRUE;
	}
	/* 删除失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：SaveKey
* 参　　数：LPCTSTR [IN] : 待保存的文件名
* 功能描述：通过保存的文件名称，保存对应的子键
*============================================================*/

BOOL CRegOperator::SaveKey(TCHAR* lpFileName)
{
	assert(m_hKey);
	assert(lpFileName);

	long lReturn = RegSaveKey(m_hKey, lpFileName, NULL);

	if (ERROR_SUCCESS == lReturn)
	{
		/* 保存成功 */
		return TRUE;
	}
	/* 保存失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：RestoreKey
* 参　　数：LPCTSTR [IN] : 待恢复的文件名
* 功能描述：通过文件名称，从其中导入注册表中
*============================================================*/

BOOL CRegOperator::RestoreKey(TCHAR* lpFileName)
{
	assert(m_hKey);
	assert(lpFileName);

	long lReturn = RegRestoreKey(m_hKey, lpFileName, 0);

	if (ERROR_SUCCESS == lReturn)
	{
		/* 导入成功 */
		return TRUE;
	}
	/* 导入失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：Read
* 参　　数：LPCTSTR CString [IN] : 键值 读取值的类型
* 功能描述：将指定位置的CString类型的值读出
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
		/* 查询成功 */
		*lpVal = szString;

		return TRUE;
	}
	/* 查询失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：Read
* 参　　数：LPCTSTR DWORD [IN] : 键值 读取值的类型
* 功能描述：将指定位置DWORD类型的值读出
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
		/* 查询成功 */
		*pdwVal = dwDest;

		return TRUE;
	}
	/* 查询失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：Read
* 参　　数：LPCTSTR int [IN] : 键值 读取值的类型
* 功能描述：将指定位置int类型的值读出
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
		/* 查询成功 */
		*pnVal = (int)dwDest;

		return TRUE;
	}

	return FALSE;
}

/*============================================================
* 函 数 名：Write
* 参　　数：LPCTSTR LPCTSTR [IN] : 键值 写入值的类型
* 功能描述：将LPCTSTR类型的值写入指定位置
*============================================================*/

BOOL CRegOperator::Write(TCHAR* lpValueName, char* lpValue)
{
	assert(m_hKey);
	assert(lpValueName);
	assert(lpValue);

	long lReturn = RegSetValueEx(m_hKey, lpValueName, 0L, REG_SZ, (const BYTE *)lpValue, strlen(lpValue) + 1);

	if (ERROR_SUCCESS == lReturn)
	{
		/* 成功写入 */
		return TRUE;
	}
	/* 写入失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：Write
* 参　　数：LPCTSTR DWORD [IN] : 键值 写入值的类型
* 功能描述：将DWORD类型的值写入指定位置
*============================================================*/

BOOL CRegOperator::Write(TCHAR* lpSubKey, DWORD dwVal)
{
	assert(m_hKey);
	assert(lpSubKey);

	long lReturn = RegSetValueEx(m_hKey, lpSubKey, 0L, REG_DWORD, (const BYTE *)&dwVal, sizeof(DWORD));

	if (ERROR_SUCCESS == lReturn)
	{
		/* 成功写入 */
		return TRUE;
	}
	/* 写入失败 */
	return FALSE;
}

/*============================================================
* 函 数 名：Write
* 参　　数：LPCTSTR int [IN] : 键值 写入值的类型
* 功能描述：将整型数写入指定位置
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
		/* 成功写入 */
		return TRUE;
	}
	/* 写入失败 */
	return FALSE;
}
