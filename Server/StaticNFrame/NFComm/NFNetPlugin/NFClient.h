// -------------------------------------------------------------------------
//    @FileName         :    NFClient.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-24
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string>
#include "NFNetDefine.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NetObject.h"
#include "NFIClient.h"

/////////////////////////////////////////////////
/**
 *@file   NFClient.h
 *@brief  单线程libevent网络客户端封装类.
 *
 */
/////////////////////////////////////////////////

/**
*@brief  单线程libevent网络客户端封装类.
*/
class NFClient : public NFIClient
{
public:
	/**
	 *@brief  构造函数.
	 */
	NFClient(uint32_t nId, const NFClientFlag& flag);

	/**
	 *@brief  析构函数.
	 */
	virtual ~NFClient();

	/**
	 * @brief	初始化
	 *
	 * @return 是否成功
	 */
	virtual bool Init() override;

	/**
	 * @brief	关闭客户端释放数据
	 *
	 * @return  是否成功
	 */
	virtual bool Shut() override;

	/**
	 * @brief 释放数据
	 *
	 * @return bool
	 */
	virtual bool Finalize() override;

	/**
	 * @brief	服务器每帧执行
	 *
	 * @return	是否成功
	 */
	virtual bool Execute() override;

	/**
	 * @brief	删除关闭的连接
	 *
	 * @return	是否成功
	 */
	virtual void ExecuteClose();

	/**
	 * @brief	关闭连接
	 *
	 * @return	是否成功
	 */
	virtual void CloseServer();

	/**
	 * @brief	连接服务端
	 *
	 * @return  连接成功与否
	 */
	virtual bool Connect();

	/**
	 * @brief	关闭客户端连接, 与Conenct对应
	 * 程序运行时，只能在OnExectue里调用，
	 * 退出时，在析构函数调用
	 * @return
	 */
	virtual void Close();

	/**
	 * @brief	获得libevent主数据结构
	 * @return
	 */
	event_base* GetMainBase() const;

	/**
	 * @brief	发送数据
	 *
	 * @param pData		发送的数据, 这里的数据已经包含了数据头
	 * @param unSize	数据的大小
	 * @return
	 */
	virtual bool Send(const void* pData, uint32_t unSize);
protected:
	/**
	 * @brief	libevent的react数据结构
	 */
	event_base* m_pMainBase;;

	/**
	 * @brief	连接对象
	 */
	NetObject* m_pObject;
};

