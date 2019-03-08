/**
* Binhex编解码
*/

#ifndef __CTLIB_BINHEX_H__
#define __CTLIB_BINHEX_H__

namespace CTLib
{

class CTBinhex
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
			sprintf((char*)pDst, "%02X", *pSrc);
			pDst += 2;
			iDstLen += 2;
		}
		
		// 输出加个结束符
		*pDst = '\0';
		
		return iDstLen;
	}

	static int Decode(const char* pszBuf, int iBufLen, char* pszOut)
	{
		unsigned char* pSrc = (unsigned char*) pszBuf;
		unsigned char* pDst = (unsigned char*) pszOut;
		
		if (iBufLen % 2 != 0)
		{
			return -1;
		}
		
		int iDstLen = (iBufLen / 2);
		char nNum[2];
		
		int i = 0, j = 0;
		for (i = 0; i < iDstLen; ++i)
		{
			for (j = 0; j < 2; ++j, ++pSrc)
			{
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
		}
		
		return iDstLen;
	}
};
	
} // namespace CTLib

#endif //__CTLIB_BINHEX_H__
