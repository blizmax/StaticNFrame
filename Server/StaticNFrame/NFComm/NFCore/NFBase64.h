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

class _NFExport NFBase64
{
public:
	/**
	* @brief 构造函数.
	*
	*/
	NFBase64() {}

	/**
	* @brief 析构函数.
	*/
	virtual ~NFBase64() {}

	/**
	* @brief Base64位编码
	*
	* @param src  要编码的数据，不能为空
	* @param dst  编码后的数据，不能是空指针
	* @return     编码是否成功
	*/
	static bool Encode(const std::string& src, std::string* dst);

	/**
	* @brief Base64位解码
	*
	* @param data  要解码的数据，不能为空
	* @param size  解码后的数据，不能是空指针
	* @return      解码是否成功
	*/
	static bool Decode(const std::string& src, std::string* dst);

private:
	/**
	* @brief 根据在Base64编码表中的序号求得某个字符
	*
	* @param n  在Base64编码表中的序号
	* @return   在Base64编码表中的序号对应的某个字符
	*/
	static inline char Base2Chr(unsigned char n);

	/**
	* @brief 求得某个字符在Base64编码表中的序号
	*
	* @param c  输入字符
	* @return   字符在Base64编码表中的序号
	*/
	static inline unsigned char Chr2Base(char c);

	/**
	* @brief 求得编码后的数据长度
	*
	* @param n  编码前数据的长度
	* @return   编码后数据的长度
	*/
	inline static int Base64EncodeLen(int n);

	/**
	* @brief 求得解码后的数据长度
	*
	* @param n  解码前数据的长度
	* @return   解码后数据的长度
	*/
	inline static int Base64DecodeLen(int n);
};
