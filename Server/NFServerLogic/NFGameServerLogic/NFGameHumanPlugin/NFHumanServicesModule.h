// -------------------------------------------------------------------------
//    @FileName         :    NFHumanServicesModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/04/07
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameHumanPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFIHumanServicesModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFINoSqlModule.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"

class NFCHumanServicesModule : public NFIHumanServicesModule
{
public:
	NFCHumanServicesModule(NFIPluginManager* p);
	virtual ~NFCHumanServicesModule();

	virtual bool Init() override;

	/*
	** ���������Ҫ������������ָ��, ��̬���ص�ʱ������ָ����ܻ�ʧЧ
	*/
	virtual bool DynamicLoadPlugin() override;

	virtual void OnTimer(uint32_t nTimerID) override;

	virtual void SavePlayerDataToDB();
protected:
	NFIKernelModule* m_pKernelModule;
	NFINoSqlModule* m_pNosqlModule;
	NFIAsyMysqlModule* m_pAsyMysqlModule;
};