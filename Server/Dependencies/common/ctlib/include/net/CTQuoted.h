/**
* Quoted编解码
*/

#ifndef __CTLIB_QUOTED_H__
#define __CTLIB_QUOTED_H__

#include <stdio.h>

namespace CTLib
{

class CTQuoted
{
public:
	static int Encode(const char* pszBuf, int iBufLen, char* pszOut)
	{
		int iDstLen = 0;		// 输出的字符计数
		int i = 0;
		
		unsigned char* pSrc = (unsigned char*) pszBuf;
		unsigned char* pDst = (unsigned char*) pszOut;
		
		for (i = 0; i < iBufLen; ++i, ++pSrc)
		{
			// ASCII 33-60, 62-126原样输出，其余的需编码
			if ((*pSrc >= '!') && (*pSrc <= '~') && (*pSrc != '='))
			{
				*pDst++ = *pSrc;
				++iDstLen;
			}
			else
			{
				sprintf((char*)pDst, "=%02X", *pSrc);
				pDst += 3;
				iDstLen += 3;
			}
		}
		
		// 输出加个结束符
		*pDst = '\0';
		
		return iDstLen;
	}

	static int Decode(const char* pszBuf, int iBufLen, char* pszOut)
	{
		int iDstLen = 0;		// 输出的字符计数
		int i = 0, j = 0;
		
		unsigned char* pSrc = (unsigned char*) pszBuf;
		unsigned char* pDst = (unsigned char*) pszOut;
		
		char nNum[2];
		
		while (i < iBufLen)
		{
			if (*pSrc == '=')		// 是编码字节
			{
				if (i + 2 >= iBufLen)
				{
					return -1;
				}
				
				for (j = 0; j < 2; ++j)
				{
					++ pSrc;
					if (*pSrc >= '0' && *pSrc <= '9')
					{
						nNum[j] = *pSrc - '0';
					}
					else if (*pSrc >= 'A' && *pSrc <= 'F')
					{
						nNum[j] = *pSrc - 'A' + 10;
					}
					else if (*pSrc >= 'a' && *pSrc >= 'f')
					{
						nNum[j] = *pSrc - 'a' + 10;
					}
					else
					{
						return -2;
					}
				}
				
				*pDst = (unsigned char)(16 * nNum[0] + nNum[1]);
				++pDst;
				++pSrc;
				i += 3;
			}
			else		// 非编码字节
			{
				*pDst++ = *pSrc++;
				++i;
			}
			
			++iDstLen;
		}
		
		// 输出加个结束符
		*pDst = '\0';
		
		return iDstLen;
	}
};

} // namespace

#endif //__CTLIB_QUOTED_H__
