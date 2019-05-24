// -------------------------------------------------------------------------
//    @FileName         :    NFHumanModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIPlayerModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFPlayerDefine.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFCore/NFMapEx.hpp"

class NFCPlayerModule : public NFIPlayerModule
{
public:
	NFCPlayerModule(NFIPluginManager* p);
	virtual ~NFCPlayerModule();

	virtual bool Init() override;

	virtual NFIObject* GetPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode);

	virtual  NFIObject*  GetPlayerInfo(uint64_t playerId, uint32_t& retCode);

	virtual void CreatePlayer(const NFMsg::cgaccountlogin& cgMsg);

	virtual NFIObject* GetPlayerObject(uint64_t playerId);
private:
	virtual std::string GetInitFaceID();

	virtual NFIObject* LoadPlayerInfoByCID(const std::string& account, const std::string& password, uint32_t& retCode);

	virtual NFIObject* LoadPlayerInfo(uint64_t playerId, uint32_t& retCode);
private:
	virtual NFIObject* CreatePlayerObject(NFMsg::db_playerinfo* pInfo);
};
