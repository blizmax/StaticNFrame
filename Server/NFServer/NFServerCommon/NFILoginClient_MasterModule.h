// -------------------------------------------------------------------------
//    @FileName         :    NFILoginClient_MasterModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/10/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIModule.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFServer/NFServerCommon/NFServerCommon.h"

class NFILoginClient_MasterModule : public NFIModule
{
public:
	virtual ~NFILoginClient_MasterModule()
	{

	}

	virtual NF_SHARE_PTR<NFServerData> GetSuitProxyServer(const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFServerData> GetSuitGameServer(const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFServerData> GetSuitWorldServer() = 0;

	virtual NF_SHARE_PTR<NFServerData> GetRandProxyServer() = 0;
	virtual NF_SHARE_PTR<NFServerData> GetRandGameServer() = 0;

	virtual NF_SHARE_PTR<NFServerData> GetProxyServerByServerId(uint32_t serverId) = 0;
	virtual NF_SHARE_PTR<NFServerData> GetGameServerByServerId(uint32_t serverId) = 0;

	virtual std::vector<NF_SHARE_PTR<NFServerData>> GetAllGameServer(uint32_t gameid = 0) = 0;
	virtual uint32_t GetMasterLinkId() const = 0;

};
