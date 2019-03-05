// -------------------------------------------------------------------------
//    @FileName         :    NFBase64.h
//    @Author           :    GaoYi
//    @Date             :    2017-08-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

#include <string>

#include "NFPlatform.h"

/////////////////////////////////////////////////
/**
* @file tc_base64.h
* @brief  base64�������.
*/

/////////////////////////////////////////////////

/**
* @brief �����ṩ��׼��Base64�ı������
*/
class _NFExport NFBase64
{
public:
	/**
	* @brief  ���ַ�������base64����.
	*
	* @param data         ��Ҫ���������
	* @param bChangeLine  �Ƿ���Ҫ�����ձ������ݼ��뻻�з� ��
	*                     (RFC�н���ÿ76���ַ������س����У�Ĭ��Ϊ����ӻ���
	* @return string      ����������
	*/
	static std::string Encode(const std::string &data, bool bChangeLine = false);

	/**
	* @brief  ���ַ�������base64����.
	*
	* @param data     ��Ҫ���������
	* @return string  ����������
	*/
	static std::string Decode(const std::string &data);

	/**
	* @brief  ���ַ�������base64���� .
	*
	* @param pSrc        ��Ҫ���������
	* @param nSrcLen     ��Ҫ��������ݳ���
	* @param pDst        ����������
	* @param bChangeLine �Ƿ���Ҫ�����ձ������ݼ��뻻�з���
	*                    RFC�н���ÿ76���ַ������س����У�Ĭ��Ϊ����ӻ���
	* @return            �������ַ����ĳ���
	*/
	static int Encode(const unsigned char* pSrc, int nSrcLen, char* pDst, bool bChangeLine = false);

	/**
	* @brief  ���ַ�������base64����.
	*
	* @param pSrc    ��Ҫ���������
	* @param nSrcLe  ��Ҫ��������ݳ���
	* @param pDst   ����������
	* @return       �������ַ����ĳ���
	*/
	static int Decode(const char* pSrc, int nSrcLen, unsigned char* pDst);

protected:

	/**
	* base64�����
	*/
	static const char EnBase64Tab[];
	/**
	* base64�����
	*/
	static const char DeBase64Tab[];
};

