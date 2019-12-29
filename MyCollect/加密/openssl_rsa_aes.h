#pragma once
#include <string>

//AES���� ��Կ����
#define  BS 16

#define _DEBUG_TRACE_C_ 3
#if 0 != _DEBUG_TRACE_C_  
#include <cstdio>  
#endif  
#if 1==_DEBUG_TRACE_C_				 //��ͨ��ӡ  
#define TRACE_C printf  
#elif 2==_DEBUG_TRACE_C_			//��ӡ�ļ������к�  
#define TRACE_C(fmt,...) printf("%s(%d): \n"##fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#elif 3==_DEBUG_TRACE_C_			//��ӡ�ļ������кš�������  
#define TRACE_C(fmt,...) printf("%s(%d)-<%s>: \n"##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  
#else  
#define TRACE_C  
#endif 

class Copenssl_rsa_aes
{
public:
	Copenssl_rsa_aes(void);
	~Copenssl_rsa_aes(void);

	//Use for AES_CBC
	//AES ����Կ һ����Ҫ16, 24, or 32 ����������ʹ��16
	//����������Ҫ��16�ı��������������Ѿ��Գ��ȵ�Ҫ������˽����˴���
public:			
	void pad(unsigned char* s,int s_len,unsigned char*& out,unsigned int& out_len);
	void unpad(unsigned char* s,unsigned int s_len,unsigned char*& out,unsigned int& out_len);
	std::string AES_encrypt_base64(unsigned char*input_buf,unsigned int input_buf_length,const unsigned char *aes_key);
	std::string AES_decrypt_base64(std::string base64_input_str,const unsigned char* aes_key);
	//Use for RSA
	//RSA���ܵ����ĳ��Ȳ��ܳ���RSA��Կ�ĳ���-11������1024λ�ģ����ĳ��Ȳ��ܳ���117��
	//����һ����1024λ����Կ
public:
	std::string RSA_encrypt_base64(unsigned char*input_buf,unsigned int input_buf_len,const char* szPublicKeyPath);
	std::string RSA_decrypt_base64(std::string base64_input_str,const char* szPrivateKeyPath);
};

