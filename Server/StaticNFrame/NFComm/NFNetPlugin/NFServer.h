// -------------------------------------------------------------------------
//    @FileName         :    NFServer.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/01
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFNetDefine.h"

struct evconnlistener;
struct event_base;
class NetObject;

class NFServer : public NFIModule
{
public:
	/**
	 * @brief 构造函数
	 */
	NFServer(NF_SERVER_TYPES serverType, uint32_t serverId, const NFServerFlag& flag);

	/**
	* @brief 析构函数
	*/
	virtual ~NFServer();

	/**
	* @brief	获得唯一ID
	*
	* @return
	*/
	uint32_t GetServerId() const;

	/**
	* @brief	获得服务器类型
	*
	* @return
	*/
	uint32_t GetServerType() const;

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
	* @brief	服务器每帧执行
	*
	* @return	是否成功
	*/
	virtual bool Execute() override;

	/**
	 * @brief 获得一个可用的ID
	 *
	 * @return uint32_t
	 */
	virtual uint32_t GetFreeUnLinkId();

	/**
	* @brief 获得当前网络对象个数
	*
	* @return uint32_t
	*/
	virtual uint32_t GetNetObjectCount() const;

	/**
	* @brief 获得当前最大链接个数
	*
	* @return uint32_t
	*/
	virtual uint32_t GetMaxConnectNum() const;

	/**
	 * @brief
	 *
	 * @return event_base*
	 */
	virtual event_base* GetEventBase() const;
private:
	/**
	* @brief libevent
	*/
	struct event_base* mBase;

	/**
	* @brief libevent监听器
	*/
	struct evconnlistener* mListener;

	/**
	* @brief 服务器配置数据
	*/
	NFServerFlag mFlag;

	/**
	* @brief 服务器类型
	*/
	NF_SERVER_TYPES mServerType;

	/**
	* @brief 服务器Id，一般一个应用程序一个服务器类型，一个服务器ID
	*/
	uint32_t mServerId;

	/**
	* @brief 链接对象数组
	*/
	std::vector<NetObject*> mNetObjectArray;

	/**
	* @brief 链接对象真实数目
	*/
	uint32_t mNetObjectCount;
};

