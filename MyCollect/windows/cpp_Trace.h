#define _DEBUG_TRACE_C_ 3
#if 0 != _DEBUG_TRACE_C_  
#include <cstdio>  
#endif  
#if 1==_DEBUG_TRACE_C_				 //��ͨ��ӡ  
#define TRACE_C printf  
#elif 2==_DEBUG_TRACE_C_			//��ӡ�ļ������к�  
#define TRACE_C(fmt,...) printf("%s(%d): "##fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#elif 3==_DEBUG_TRACE_C_			//��ӡ�ļ������кš�������  
#define TRACE_C(fmt,...) printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  
#else  
#define TRACE_C  
#endif 