#pragma once
#include <string>

//AES加密 密钥长度
#define  BS 16

#define _DEBUG_TRACE_C_ 3
#if 0 != _DEBUG_TRACE_C_  
#include <cstdio>  
#endif  
#if 1==_DEBUG_TRACE_C_				 //普通打印  
#define TRACE_C printf  
#elif 2==_DEBUG_TRACE_C_			//打印文件名、行号  
#define TRACE_C(fmt,...) printf("%s(%d): \n"##fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#elif 3==_DEBUG_TRACE_C_			//打印文件名、行号、函数名  
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
	//AES 的密钥 一定需要16, 24, or 32 长，这里是使用16
	//对于输入需要是16的倍数，不过这里已经对长度的要求进行了进行了处理。
public:			
	void pad(unsigned char* s,int s_len,unsigned char*& out,unsigned int& out_len);
	void unpad(unsigned char* s,unsigned int s_len,unsigned char*& out,unsigned int& out_len);
	std::string AES_encrypt_base64(unsigned char*input_buf,unsigned int input_buf_length,const unsigned char *aes_key);
	std::string AES_decrypt_base64(std::string base64_input_str,const unsigned char* aes_key);
	//Use for RSA
	//RSA加密的明文长度不能超过RSA密钥的长度-11，比如1024位的，明文长度不能超过117。
	//这里一般是1024位的密钥
public:
	std::string RSA_encrypt_base64(unsigned char*input_buf,unsigned int input_buf_len,const char* szPublicKeyPath);
	std::string RSA_decrypt_base64(std::string base64_input_str,const char* szPrivateKeyPath);
};

