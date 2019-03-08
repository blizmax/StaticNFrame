/**
* Base64编解码
*/

#ifndef __CTLIB_BASE64_H__
#define __CTLIB_BASE64_H__

namespace CTLib
{

class CTBase64
{
public:
	static int Encode(const char* pszBuf, int iBufLen, char* pszOut)
	{
		static const char EnBase64Tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		unsigned char c1, c2, c3;	// 输入缓冲区读出3个字节
		int iDstLen = 0;			 // 输出的字符计数
		int iDiv = iBufLen / 3;	  // 输入数据长度除以3得到的倍数
		int iMod = iBufLen % 3;	  // 输入数据长度除以3得到的余数
		
		unsigned char* pSrc = (unsigned char*) pszBuf;
		unsigned char* pDst = (unsigned char*) pszOut;
		
		// 每次取3个字节，编码成4个字符
		int i = 0;
		for (i = 0; i < iDiv; ++i)
		{
			// 取3个字节
			c1 = *pSrc++;
			c2 = *pSrc++;
			c3 = *pSrc++;
			
			// 编码成4个字符
			*pDst++ = EnBase64Tab[c1 >> 2];
			*pDst++ = EnBase64Tab[((c1 << 4) | (c2 >> 4)) & 0x3f];
			*pDst++ = EnBase64Tab[((c2 << 2) | (c3 >> 6)) & 0x3f];
			*pDst++ = EnBase64Tab[c3 & 0x3f];
			iDstLen += 4;
		}
		
		// 编码余下的字节
		if (iMod == 1)
		{
			c1 = *pSrc++;
			*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
			*pDst++ = EnBase64Tab[((c1 & 0x03) << 4)];
			*pDst++ = '=';
			*pDst++ = '=';
			iDstLen += 4;
		}
		else
		{
			c1 = *pSrc++;
			c2 = *pSrc++;
			*pDst++ = EnBase64Tab[(c1 & 0xfc) >> 2];
			*pDst++ = EnBase64Tab[((c1 & 0x03) << 4) | ((c2 & 0xf0) >> 4)];
			*pDst++ = EnBase64Tab[((c2 & 0x0f) << 2)];
			*pDst++ = '=';
			iDstLen += 4;
		}
		
		// 输出加个结束符
		*pDst = '\0';
		
		return iDstLen;
	}

	static int Decode(const char* pszBuf, int iBufLen, char* pszOut)
	{
		static const char DeBase64Tab[] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				62,		// '+'
				0, 0, 0,
				63,		// '/'
				52, 53, 54, 55, 56, 57, 58, 59, 60, 61,		// '0'-'9'
				0, 0, 0, 0, 0, 0, 0,
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
				13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,		// 'A'-'Z'
				0, 0, 0, 0, 0, 0,
				26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
				39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,		// 'a'-'z'
		};

		int iDstLen = 0;			// 输出的字符计数
		int i = 0, j = 18, k = 0;
		unsigned long nValue = 0;	// 解码用到的长整数
		
		unsigned char* pSrc = (unsigned char*) pszBuf;
		unsigned char* pDst = (unsigned char*) pszOut;
		
		// 取4个字符，解码到一个长整数，再经过移位得到3个字节
		while (i < iBufLen)
		{
			if (*pSrc == '=' )
			{
				++ pSrc;
				++ i;
				++ k;
				continue;
			}
			
			if (! (*pSrc == '+' || *pSrc == '/' || ( *pSrc >= 'a' && *pSrc <= 'z') || 
				(*pSrc >= '0' && *pSrc <= '9') || ( *pSrc >= 'A' && *pSrc <= 'Z')))
			{
				++ pSrc;
				++ i;
				continue;
			}
			
			nValue |= ((unsigned long) DeBase64Tab[*pSrc++]) << j;
			
			if (j == 0)
			{
				*pDst ++ = (unsigned char) ((nValue & 0x00ff0000) >> 16);
				*pDst ++ = (unsigned char) ((nValue & 0x0000ff00) >> 8);
				*pDst ++ = (unsigned char)  (nValue & 0x000000ff);
				j = 18;
				iDstLen += 3;
				nValue = 0;
			}
			else
			{
				j -= 6;
			}
			
			++ i;
		}
		
		if (k == 2)
		{
			*pDst ++ = (unsigned char) ((nValue & 0x00ff0000) >> 16);
			++ iDstLen;
		}
		else
		{
			*pDst ++ = (unsigned char) ((nValue & 0x00ff0000) >> 16);
			*pDst ++ = (unsigned char) ((nValue & 0x0000ff00) >> 8);
			iDstLen += 2;
		}
		
		// 输出加个结束符
		*pDst = '\0';
		
		return iDstLen;
	}
};

} // namespace CTLib

#endif //__CTLIB_BASE64_H__
