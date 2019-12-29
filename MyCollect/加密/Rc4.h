#include "stdafx.h"
#include <stdio.h>
#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

class CRC4 
{
private:
	unsigned char sbox[256];      /* Encryption array             */
	unsigned char key[256],k;     /* Numeric key values           */
	int  m, n, i, j, ilen;        /* Ambiguously named counters   */
public:

	CRC4 () 
	{
		memset(sbox,0,256);
		memset(key,0,256);
	}
	virtual ~CRC4 ()
	{							
		memset(sbox,0,256);  /* remove Key traces in memory  */
		memset(key,0,256);   
	}

	char *Encrypt(char *pszText,const char *pszKey) 
	{
		i=0, j=0,n = 0;
		ilen = (int)strlen(pszKey);

		for (m = 0;  m < 256; m++)  /* Initialize the key sequence */
		{
			*(key + m)= *(pszKey + (m % ilen));
			*(sbox + m) = m;
		}

		for (m=0; m < 256; m++)
		{
			n = (n + *(sbox+m) + *(key + m)) &0xff;
			SWAP(*(sbox + m),*(sbox + n));
		}

		ilen = (int)strlen(pszText);
		for (m = 0; m < ilen; m++)
		{
			i = (i + 1) &0xff;
			j = (j + *(sbox + i)) &0xff;
			SWAP(*(sbox+i),*(sbox + j));  /* randomly Initialize the key sequence */
			k = *(sbox + ((*(sbox + i) + *(sbox + j)) &0xff ));
			if(k == *(pszText + m))       /* avoid '\0' beween the decoded text; */
				k = 0;
			*(pszText + m) ^=  k;
		}
		return pszText;

	}

	char *Decrypt(char *pszText,const char *pszKey)
	{
		return Encrypt(pszText,pszKey) ;  /* using the same function as encoding */
	}
};