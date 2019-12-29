/************************************************************************/
/* use rsa and aes from openssl
/* aes : cbc_mode
/* all output encoded by base64
/* 2017-03-06
/* by yuc
/************************************************************************/
#include "StdAfx.h"
#include "openssl_rsa_aes.h"
#include "openssl/rsa.h"
#include "openssl/aes.h"
#include "openssl/rand.h"
#include<openssl/pem.h>   
#include <openssl/bio.h> 
#pragma comment(lib, "libeay32.lib")  
#pragma comment(lib, "ssleay32.lib")  
#include "base64.h"

Copenssl_rsa_aes::Copenssl_rsa_aes(void)
{
}


Copenssl_rsa_aes::~Copenssl_rsa_aes(void)
{
}


/************************************************************************/
/*函数: pad 
/*功能: AES加密过程中,用于对输入数据的补齐。（按照aes 长度均为16的整数）
/*参数1: s 输入的需要加密的数据
/*参数2: s_len 需要加密数据长度
/*参数3: 补齐后输出的数据，需要外部删除分配的空间
/*参数4: 补齐后数据长度（真实加密的数据长度）
/*返回值: void
/************************************************************************/
void Copenssl_rsa_aes::pad(unsigned char* s,int s_len,unsigned char*& out,unsigned int& out_len)
{
	//int s_len = _mbslen(s);
	int plus_len = (BS-s_len%BS);
	char m = (char)(BS-s_len%BS);
	out_len = s_len+plus_len;
	out = new unsigned char[out_len];
	memset(out,0,out_len);
	memcpy(out,s,s_len);
	memset(out+s_len,m,plus_len);
}

/************************************************************************/
/*函数: unpad 
/*功能: AES解密过程中，用于去除对其的字符
/*参数1: s 输入的解密后的数据
/*参数2: s_len 解密数据长度
/*参数3: 去除补齐后输出的数据，需要外部删除分配的空间
/*参数4: 数据长度（真实解密后数据长度）
/*返回值: void
/************************************************************************/
void Copenssl_rsa_aes::unpad(unsigned char* s,unsigned int s_len,unsigned char*& out,unsigned int& out_len)
{
	int plus_len = s[s_len-1];
	out_len = s_len-plus_len;
	out = new unsigned char[out_len];
	memset(out,0,out_len);
	memcpy(out,s,out_len);
}


/************************************************************************/
/*函数: AES_encrypt_base64
/*功能: AES加密unsigned char的数据，返回base64的加密数据。注：CBC模式的加密，有经过添加16位的随机iv到加密数据前。
/*参数1: input_buf  输入的数据
/*参数2: input_buf_length 输入数据长度
/*参数3: aes_key AES秘钥
/*返回值: 经过base64 的加密数据                                                  
/************************************************************************/
std::string Copenssl_rsa_aes::AES_encrypt_base64(unsigned char*input_buf,unsigned int input_buf_length,const unsigned char *aes_key)
{
	if (input_buf == NULL || input_buf_length == 0|| aes_key == NULL)
	{
		printf("input param error");
		return "";
	}
	//随机数生成 16位  iv会在加密后变化，这里保存一个
	unsigned char iv_enc[AES_BLOCK_SIZE] = {0};
	unsigned char iv_save[AES_BLOCK_SIZE] = {0};	
	RAND_bytes(iv_enc, AES_BLOCK_SIZE);
	memcpy(iv_save, iv_enc, AES_BLOCK_SIZE);
	//计算生成的数据加密长度
	const size_t enc_buf_length = ((input_buf_length + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
	//声明并分配加密数据大小
	unsigned char* enc_out_buf = new unsigned char[enc_buf_length];
	memset(enc_out_buf, 0, enc_buf_length);
	//声明并分配填充后的数据,并填充
	//填充后的aes-input长度
	unsigned char* pad_aes_input = NULL;
	unsigned int pad_aes_input_len = 0;
	pad(input_buf,input_buf_length,pad_aes_input,pad_aes_input_len);
	//进行openssl aes_cbc加密
	AES_KEY enc_key;
	AES_set_encrypt_key(aes_key, 128, &enc_key);
	AES_cbc_encrypt(pad_aes_input, enc_out_buf, pad_aes_input_len, &enc_key, iv_enc, AES_ENCRYPT);		// 76 38 5f ea bf 4e 5a b1 ef a4 7d d1 18 a3 82 9c

	//要组合vi+enc_out_buf的数据；要进行填充16位vi，再填充
	unsigned char* out_buf_forbase64 = new unsigned char[enc_buf_length+BS];
	memset(out_buf_forbase64 ,0 ,enc_buf_length+BS);
	memcpy(out_buf_forbase64,iv_save,BS);
	memcpy(out_buf_forbase64+BS,enc_out_buf,enc_buf_length);
	//将组合的数据，进行base64编码，这里指定了长度，避免结尾插入其他数据的问题。
	std::string strbase64 = base64_encode((const unsigned char*)out_buf_forbase64,enc_buf_length+BS);
	if (enc_out_buf != NULL)
	{
		delete[] enc_out_buf;
		enc_out_buf = NULL;
	}
	if (pad_aes_input  != NULL)
	{
		delete[] pad_aes_input;
		pad_aes_input = NULL;
	}
	if ( out_buf_forbase64 != NULL)
	{
		delete[] out_buf_forbase64;
		out_buf_forbase64 = NULL; 
	}
	return strbase64;
}



/************************************************************************/
/*函数: AES_decrypt_base64
/*功能: AES解密base64的加密数据，返回base64的解密数据
/*参数1: base64_input_str  经过base64后的加密string
/*参数2: aes_key  aes秘钥
/*返回值: 经过base64 的解密数据,需要decode获取初始结果 （避免一些byte字符的问题，string也方便使用）
/************************************************************************/
std::string Copenssl_rsa_aes::AES_decrypt_base64(std::string base64_input_str,const unsigned char* aes_key)
{
	if (base64_input_str.length() == 0 || aes_key == NULL)
	{
		printf("input param error");
		return "";
	}
	//base64解密
	std::string input_str = base64_decode(base64_input_str);
	//添加vi的加密数据长度
	int enc_buf_length = input_str.length();
	//添加vi的加密数据
	unsigned char* aes_input_buf = (unsigned char*)input_str.c_str();
	//获取解秘iv  拷贝iv，BS长度 一般16
	unsigned char iv_dec[AES_BLOCK_SIZE] = {0};
	memcpy(iv_dec,aes_input_buf,BS); 
	//加密的数据  需要减去前面的VI，指针移位 +BS
	unsigned char* enc_buf = new unsigned char[enc_buf_length-BS];
	memset(enc_buf,0,enc_buf_length-BS);
	memcpy(enc_buf,aes_input_buf+BS,enc_buf_length-BS);
	//定义解密后数据
	int dec_buf_length = enc_buf_length-BS;
	unsigned char* dec_out_buf = new unsigned char[dec_buf_length];
	memset(dec_out_buf,0,dec_buf_length);
	//生成秘钥,解密
	AES_KEY  dec_key;
	AES_set_decrypt_key(aes_key, 128, &dec_key);
	AES_cbc_encrypt(enc_buf, dec_out_buf, enc_buf_length-BS, &dec_key, iv_dec, AES_DECRYPT);
	//获取解秘后长度,要减去补完长度
	//如果是字符串结果需要+1
	unsigned char* real_dec = NULL;
	unsigned int real_len = 0;
	unpad(dec_out_buf,dec_buf_length,real_dec,real_len);
	std::string strdata = base64_encode(real_dec,real_len);
	if (real_dec != NULL)
	{
		delete [] real_dec;
		real_dec = NULL;
	}
	return strdata;
}

/************************************************************************/
/*函数: RSA_encrypt_base64
/*功能:RSA 公钥加密
/*参数1: input_buf 需要加密的数据
/*参数2: input_buf_len  需要加密的数据长度
/*参数3: szPublicKeyPath 公钥的路径
/*返回值: 经过base64 的加密数据                                                              
/************************************************************************/
std::string Copenssl_rsa_aes::RSA_encrypt_base64(unsigned char*input_buf,unsigned int input_buf_len,const char* szPublicKeyPath)
{  
	if (input_buf == NULL || input_buf_len  == 0 || szPublicKeyPath == NULL)
	{
		printf("intput error\n");
		return "";
	}
	OpenSSL_add_all_algorithms();  
	BIO* bp = BIO_new( BIO_s_file() );  
	BIO_read_filename( bp,szPublicKeyPath);  
	//RSA* rsaK = PEM_read_bio_RSAPublicKey( bp, NULL, NULL, NULL );  
	RSA* rsaK = PEM_read_bio_RSA_PUBKEY(bp,NULL,NULL,NULL);
	if (NULL == rsaK) 
	{  
		printf("read key file fail!\n");  
		return "";
	}
	int rsa_len = RSA_size(rsaK);  
	unsigned char *rsa_encode_buf = new unsigned char[rsa_len]; 
	memset(rsa_encode_buf,0,rsa_len);
	int ret = RSA_public_encrypt(input_buf_len,input_buf, rsa_encode_buf,rsaK,RSA_PKCS1_PADDING);  
	std::string rsa_encode_base64_buf = "";
	if (ret >= 0) {  
		rsa_encode_base64_buf = base64_encode(rsa_encode_buf,ret);
	}  
	delete[] rsa_encode_buf;  
	CRYPTO_cleanup_all_ex_data();  
	BIO_free_all( bp );  
	RSA_free(rsaK);  
	
	return rsa_encode_base64_buf;  
}  

/************************************************************************/
/*函数: RSA_decrypt_base64
/*功能:RSA 私钥解密
/*参数1: base64_input_str 经过base64的加密数据
/*参数2: szPrivateKeyPath 私钥的路径
/*返回值: 解密数据,为string类型。因为rsa加密数据长度限制，一般加密的是字符串，故直接使用string
/************************************************************************/
std::string Copenssl_rsa_aes::RSA_decrypt_base64(std::string base64_input_str,const char* szPrivateKeyPath)
{  
	if (base64_input_str.length() == 0|| szPrivateKeyPath == NULL)
	{
		printf("intput error \n");
		return "";
	}
	std::string input_str = base64_decode(base64_input_str);
	OpenSSL_add_all_algorithms();  
	BIO* bp = BIO_new( BIO_s_file() );  
	BIO_read_filename( bp,szPrivateKeyPath);  
	//RSA* rsaK = PEM_read_bio_RSAPrivateKey( bp, NULL, NULL, passwd ); 
	RSA* rsaK = PEM_read_bio_RSAPrivateKey( bp, NULL, NULL, NULL ); 
	if (NULL == rsaK) {  
		printf("read key file fail!\n");  
		return "";
	}
	int rsa_len = RSA_size(rsaK);  
	//RSA解密后的数据
	unsigned char *rsa_decode_buf = new unsigned char[rsa_len];
	memset(rsa_decode_buf,0,rsa_len);
	int ret = RSA_private_decrypt(input_str.length(),(const unsigned char*)input_str.c_str(),rsa_decode_buf,rsaK,RSA_PKCS1_PADDING);  
	std::string str_rsa_decode_buf = "";
	if (ret >= 0) {  
		str_rsa_decode_buf = std::string((char*)rsa_decode_buf,ret);
	}  
	delete[] rsa_decode_buf;  
	CRYPTO_cleanup_all_ex_data();  
	BIO_free_all( bp );  
	RSA_free(rsaK);  
	return str_rsa_decode_buf;  
}  