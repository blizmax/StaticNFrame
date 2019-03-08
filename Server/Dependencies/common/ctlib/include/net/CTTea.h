/**
* QQ对称加密第二代函数
*/

#ifndef __CTLIB_TEA_H__
#define __CTLIB_TEA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
    #include "CTSocketUtils.h"
#else
    #include <netinet/in.h>
    #include <sys/time.h>
    #include <unistd.h>
#endif

namespace CTLib
{

typedef struct tagTeaKey
{
	enum
	{
		TEAKEY_SIZE = 16,
	};

	char szKey[TEAKEY_SIZE];
}
TTeaKey;

/**
* QQ对称加密第二代函数
* TEA算法, CBC模式
* 密文格式: PadLen(1)+Padding(var,0-7)+Salt(2)+Body(var)+Zero(7)
* pKey为16字节
*/
class CTTea
{
private:
	enum
	{
		DELTA = 0x9e3779b9,
		ROUNDS = 16,
		LOG_ROUNDS = 4,
		SALT_LEN = 2,
		ZERO_LEN = 7,
	};

public:
	static int EncryptLen(int iInBufLen)
	{
		// 根据Body长度计算PadLen,最小必需长度必需为8byte的整数倍
		int iLen = iInBufLen + 1 + SALT_LEN + ZERO_LEN;
		int iPadLen = iLen % 8;
		if (iPadLen)
		{
			iLen += 8 - iPadLen;
		}

		return iLen;
	}

    /**
    * 加密算法
    * @param pInBuf 明文
    * @param iInBufLen pInBuf长度
    * @param pOutBuf 密文
    * @param riOutBufLen pOutBuf的长度, 是8的倍数, 至少应预留iInBufLen+17
    */
	static void Encrypt(const char* pInBuf, int iInBufLen, const TTeaKey& stKey,
						char* pOutBuf, int& riOutBufLen)
	{
		char src_buf[8], iv_plain[8], *iv_crypt;
		int src_i, i, j;

		// 根据Body长度计算PadLen,最小必需长度必需为8byte的整数倍
		int iPadSaltBodyZeroLen = iInBufLen + 1 + SALT_LEN + ZERO_LEN;
		int iPadLen = iPadSaltBodyZeroLen % 8;
		if (iPadLen)
		{
			iPadLen = 8 - iPadLen;
		}

		// 加密第一块数据(8byte), 取前面10byte
		src_buf[0] = ((char)rand()) & 0x0f8/*最低三位存PadLen,清零*/ | (char)iPadLen;
		src_i = 1; /*src_i指向src_buf下一个位置*/

		while (iPadLen--)
		{
			src_buf[src_i++] = (char)rand();
		}

		for (i = 0; i < 8; ++i)
		{
			iv_plain[i] = 0;
		}
		iv_crypt = iv_plain; // make zero iv
		riOutBufLen = 0; // init OutBufLen

		for (i = 1; i <= (int)SALT_LEN;)
		{
			if (src_i < 8)
			{
				src_buf[src_i++] = (char)rand();
				i++; // i inc in here
			}

			if (src_i == 8)
			{
				for (j = 0; j < 8; ++j) // 加密前异或前8个byte的密文(iv_crypt指向的)
				{
					src_buf[j]^=iv_crypt[j];
				}

				TeaEncryptECB(src_buf, stKey.szKey, pOutBuf);

				for (j = 0; j < 8; ++j) // 加密后异或前8个byte的明文(iv_plain指向的)
				{
					pOutBuf[j]^=iv_plain[j];
				}

				// 保存当前的iv_plain
				for (j = 0; j < 8; ++j)
				{
					iv_plain[j] = src_buf[j];
				}

				// 更新iv_crypt
				src_i = 0;
				iv_crypt = pOutBuf;
				riOutBufLen += 8;
				pOutBuf += 8;
			}
		}

		while (iInBufLen)
		{
			if (src_i < 8)
			{
				src_buf[src_i++] = *(pInBuf++);
				--iInBufLen;
			}

			if (src_i == 8)
			{
				for (j = 0; j < 8; ++j) // 加密前异或前8个byte的密文(iv_crypt指向的)
				{
					src_buf[j]^=iv_crypt[j];
				}

				TeaEncryptECB(src_buf, stKey.szKey, pOutBuf);

				for (j = 0; j < 8; ++j) // 加密后异或前8个byte的明文(iv_plain指向的)
				{
					pOutBuf[j]^=iv_plain[j];
				}

				// 保存当前的iv_plain
				for (j = 0; j < 8; ++j)
				{
					iv_plain[j] = src_buf[j];
				}

				src_i = 0;
				iv_crypt = pOutBuf;
				riOutBufLen += 8;
				pOutBuf += 8;
			}
		}
		
		for (i = 1; i <= (int)ZERO_LEN;)
		{
			if (src_i < 8)
			{
				src_buf[src_i++] = 0;
				++i;
			}
			
			if (src_i == 8)
			{
				for (j = 0; j < 8; ++j) // 加密前异或前8个byte的密文(iv_crypt指向的)
				{
					src_buf[j]^=iv_crypt[j];
				}

				TeaEncryptECB(src_buf, stKey.szKey, pOutBuf);

				for (j = 0; j < 8; ++j) // 加密后异或前8个byte的明文(iv_plain指向的)
				{
					pOutBuf[j]^=iv_plain[j];
				}

				// 保存当前的iv_plain
				for (j = 0; j < 8; ++j)
				{
					iv_plain[j] = src_buf[j];
				}

				src_i = 0;
				iv_crypt = pOutBuf;
				riOutBufLen += 8;
				pOutBuf += 8;
			}
		}
	}

    /**
    * 解密算法
    * @param pInBuf 密文
    * @param iInBufLen pInBuf的长度, 是8的倍数
    * @param stKey 密钥
    * @param pOutBuf 明文
    * @param riOutBufLen pOutBuf的长度, 至少应预留iInBufLen-10
    */
	static bool Decrypt(const char* pInBuf, int iInBufLen, const TTeaKey& stKey,
						char* pOutBuf, int& riOutBufLen)
	{
		char dest_buf[8], zero_buf[8];
		const char *iv_pre_crypt, *iv_cur_crypt;
		int dest_i, i, j;
		const char *pInBufBoundary;
		int iBufPos = 0;
		
		if ((iInBufLen % 8) || (iInBufLen < 16))
		{
			return false;
		}

		TeaDecryptECB(pInBuf, stKey.szKey, dest_buf);
		int iPadLen = dest_buf[0] & 0x7;
		i = iInBufLen - 1 - iPadLen - SALT_LEN - ZERO_LEN; // 明文长度
		if ((riOutBufLen < i) || (i < 0))
		{
			return false;
		}

		riOutBufLen = i;
		
		pInBufBoundary = pInBuf + iInBufLen; // 输入缓冲区的边界, 下面不能pInBuf>=pInBufBoundary

		for (i = 0; i < 8; ++i)
		{
			zero_buf[i] = 0;
		}

		iv_pre_crypt = zero_buf;
		iv_cur_crypt = pInBuf;

		pInBuf += 8;
		iBufPos += 8;
		
		dest_i = 1; // dest_i指向dest_buf下一个位置

		// 把Padding滤掉
		dest_i += iPadLen;

		// 把Salt滤掉
		for (i = 1; i <= (int)SALT_LEN;)
		{
			if (dest_i < 8)
			{
				++dest_i;
				++i;
			}
			else if (dest_i == 8)
			{
				iv_pre_crypt = iv_cur_crypt;
				iv_cur_crypt = pInBuf; 
				
				// 异或前一块明文(在dest_buf[]中)
				for (j = 0; j < 8; ++j)
				{
					if ((iBufPos + j) >= iInBufLen)
					{
						return false;
					}
					dest_buf[j] ^= pInBuf[j];
				}

				TeaDecryptECB(dest_buf, stKey.szKey, dest_buf);
				
				pInBuf += 8;
				iBufPos += 8;
				
				dest_i = 0; // dest_i指向dest_buf下一个位置
			}
		}
		
		// 还原明文
		int iPlainLen = riOutBufLen;
		while (iPlainLen)
		{
			if (dest_i < 8)
			{
				*(pOutBuf++) = dest_buf[dest_i]^iv_pre_crypt[dest_i];
				++dest_i;
				--iPlainLen;
			}
			else if (dest_i == 8)
			{
				iv_pre_crypt = iv_cur_crypt;
				iv_cur_crypt = pInBuf; 
				
				for (j = 0; j < 8; ++j)
				{
					if ((iBufPos + j) >= iInBufLen)
					{
						return false;
					}
					dest_buf[j] ^= pInBuf[j];
				}

				TeaDecryptECB(dest_buf, stKey.szKey, dest_buf);

				pInBuf += 8;
				iBufPos += 8;
				
				dest_i = 0; // dest_i指向dest_buf下一个位置
			}
		}
		
		for (i = 1; i <= (int)ZERO_LEN;)
		{
			if (dest_i < 8)
			{
				if (dest_buf[dest_i]^iv_pre_crypt[dest_i])
				{
					return false;
				}
				++dest_i;
				++i;
			}
			else if (dest_i == 8)
			{
				iv_pre_crypt = iv_cur_crypt;
				iv_cur_crypt = pInBuf; 

				for (j = 0; j < 8; ++j)
				{
					if ((iBufPos + j) >= iInBufLen)
					{
						return false;
					}
					dest_buf[j] ^= pInBuf[j];
				}

				TeaDecryptECB(dest_buf, stKey.szKey, dest_buf);

				pInBuf += 8;
				iBufPos += 8;
				dest_i = 0;
			}
		}
		
		return true;
	}

protected:
	static void TeaEncryptECB(const char *pInBuf, const char *pKey, char *pOutBuf)
	{
		unsigned int y, z;
		unsigned int sum;
		unsigned int k[4];
		int i;
		
		y = ntohl(*((unsigned int*)pInBuf));
		z = ntohl(*((unsigned int*)(pInBuf+4)));
		
		for (i = 0; i < 4; ++i)
		{
			// now key is TCP/IP-endian
			k[i] = ntohl(*((unsigned int*)(pKey+i*4)));
		}
		
		sum = 0;
		for (i = 0; i < (int)ROUNDS; ++i)
		{   
			sum += DELTA;
			y += (z << 4) + k[0] ^ z + sum ^ (z >> 5) + k[1];
			z += (y << 4) + k[2] ^ y + sum ^ (y >> 5) + k[3];
		}
		
		*((unsigned int*)pOutBuf) = htonl(y);
		*((unsigned int*)(pOutBuf+4)) = htonl(z);
	}
	
	static void TeaDecryptECB(const char *pInBuf, const char *pKey, char *pOutBuf)
	{
		unsigned int y, z, sum;
		unsigned int k[4];
		int i;
		
		y = ntohl(*((unsigned int*)pInBuf));
		z = ntohl(*((unsigned int*)(pInBuf+4)));
		
		for (i = 0; i < 4; ++i)
		{
			// key is TCP/IP-endian
			k[i] = ntohl(*((unsigned int*)(pKey+i*4)));
		}
		
		sum = DELTA << LOG_ROUNDS;
		for (i = 0; i < (int)ROUNDS; ++i)
		{
			z -= (y << 4) + k[2] ^ y + sum ^ (y >> 5) + k[3]; 
			y -= (z << 4) + k[0] ^ z + sum ^ (z >> 5) + k[1];
			sum -= DELTA;
		}
		
		*((unsigned int*)pOutBuf) = htonl(y);
		*((unsigned int*)(pOutBuf+4)) = htonl(z);
	}
};

} // namespace

#endif // __CTLIB_TEA_H__
