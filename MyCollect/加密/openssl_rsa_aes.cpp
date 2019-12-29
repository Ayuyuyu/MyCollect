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
/*����: pad 
/*����: AES���ܹ�����,���ڶ��������ݵĲ��롣������aes ���Ⱦ�Ϊ16��������
/*����1: s �������Ҫ���ܵ�����
/*����2: s_len ��Ҫ�������ݳ���
/*����3: �������������ݣ���Ҫ�ⲿɾ������Ŀռ�
/*����4: ��������ݳ��ȣ���ʵ���ܵ����ݳ��ȣ�
/*����ֵ: void
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
/*����: unpad 
/*����: AES���ܹ����У�����ȥ��������ַ�
/*����1: s ����Ľ��ܺ������
/*����2: s_len �������ݳ���
/*����3: ȥ���������������ݣ���Ҫ�ⲿɾ������Ŀռ�
/*����4: ���ݳ��ȣ���ʵ���ܺ����ݳ��ȣ�
/*����ֵ: void
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
/*����: AES_encrypt_base64
/*����: AES����unsigned char�����ݣ�����base64�ļ������ݡ�ע��CBCģʽ�ļ��ܣ��о������16λ�����iv����������ǰ��
/*����1: input_buf  ���������
/*����2: input_buf_length �������ݳ���
/*����3: aes_key AES��Կ
/*����ֵ: ����base64 �ļ�������                                                  
/************************************************************************/
std::string Copenssl_rsa_aes::AES_encrypt_base64(unsigned char*input_buf,unsigned int input_buf_length,const unsigned char *aes_key)
{
	if (input_buf == NULL || input_buf_length == 0|| aes_key == NULL)
	{
		printf("input param error");
		return "";
	}
	//��������� 16λ  iv���ڼ��ܺ�仯�����ﱣ��һ��
	unsigned char iv_enc[AES_BLOCK_SIZE] = {0};
	unsigned char iv_save[AES_BLOCK_SIZE] = {0};	
	RAND_bytes(iv_enc, AES_BLOCK_SIZE);
	memcpy(iv_save, iv_enc, AES_BLOCK_SIZE);
	//�������ɵ����ݼ��ܳ���
	const size_t enc_buf_length = ((input_buf_length + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
	//����������������ݴ�С
	unsigned char* enc_out_buf = new unsigned char[enc_buf_length];
	memset(enc_out_buf, 0, enc_buf_length);
	//�������������������,�����
	//�����aes-input����
	unsigned char* pad_aes_input = NULL;
	unsigned int pad_aes_input_len = 0;
	pad(input_buf,input_buf_length,pad_aes_input,pad_aes_input_len);
	//����openssl aes_cbc����
	AES_KEY enc_key;
	AES_set_encrypt_key(aes_key, 128, &enc_key);
	AES_cbc_encrypt(pad_aes_input, enc_out_buf, pad_aes_input_len, &enc_key, iv_enc, AES_ENCRYPT);		// 76 38 5f ea bf 4e 5a b1 ef a4 7d d1 18 a3 82 9c

	//Ҫ���vi+enc_out_buf�����ݣ�Ҫ�������16λvi�������
	unsigned char* out_buf_forbase64 = new unsigned char[enc_buf_length+BS];
	memset(out_buf_forbase64 ,0 ,enc_buf_length+BS);
	memcpy(out_buf_forbase64,iv_save,BS);
	memcpy(out_buf_forbase64+BS,enc_out_buf,enc_buf_length);
	//����ϵ����ݣ�����base64���룬����ָ���˳��ȣ������β�����������ݵ����⡣
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
/*����: AES_decrypt_base64
/*����: AES����base64�ļ������ݣ�����base64�Ľ�������
/*����1: base64_input_str  ����base64��ļ���string
/*����2: aes_key  aes��Կ
/*����ֵ: ����base64 �Ľ�������,��Ҫdecode��ȡ��ʼ��� ������һЩbyte�ַ������⣬stringҲ����ʹ�ã�
/************************************************************************/
std::string Copenssl_rsa_aes::AES_decrypt_base64(std::string base64_input_str,const unsigned char* aes_key)
{
	if (base64_input_str.length() == 0 || aes_key == NULL)
	{
		printf("input param error");
		return "";
	}
	//base64����
	std::string input_str = base64_decode(base64_input_str);
	//���vi�ļ������ݳ���
	int enc_buf_length = input_str.length();
	//���vi�ļ�������
	unsigned char* aes_input_buf = (unsigned char*)input_str.c_str();
	//��ȡ����iv  ����iv��BS���� һ��16
	unsigned char iv_dec[AES_BLOCK_SIZE] = {0};
	memcpy(iv_dec,aes_input_buf,BS); 
	//���ܵ�����  ��Ҫ��ȥǰ���VI��ָ����λ +BS
	unsigned char* enc_buf = new unsigned char[enc_buf_length-BS];
	memset(enc_buf,0,enc_buf_length-BS);
	memcpy(enc_buf,aes_input_buf+BS,enc_buf_length-BS);
	//������ܺ�����
	int dec_buf_length = enc_buf_length-BS;
	unsigned char* dec_out_buf = new unsigned char[dec_buf_length];
	memset(dec_out_buf,0,dec_buf_length);
	//������Կ,����
	AES_KEY  dec_key;
	AES_set_decrypt_key(aes_key, 128, &dec_key);
	AES_cbc_encrypt(enc_buf, dec_out_buf, enc_buf_length-BS, &dec_key, iv_dec, AES_DECRYPT);
	//��ȡ���غ󳤶�,Ҫ��ȥ���곤��
	//������ַ��������Ҫ+1
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
/*����: RSA_encrypt_base64
/*����:RSA ��Կ����
/*����1: input_buf ��Ҫ���ܵ�����
/*����2: input_buf_len  ��Ҫ���ܵ����ݳ���
/*����3: szPublicKeyPath ��Կ��·��
/*����ֵ: ����base64 �ļ�������                                                              
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
/*����: RSA_decrypt_base64
/*����:RSA ˽Կ����
/*����1: base64_input_str ����base64�ļ�������
/*����2: szPrivateKeyPath ˽Կ��·��
/*����ֵ: ��������,Ϊstring���͡���Ϊrsa�������ݳ������ƣ�һ����ܵ����ַ�������ֱ��ʹ��string
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
	//RSA���ܺ������
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