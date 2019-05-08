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

class NFCHumanServicesModule : public NFIHumanServicesModule
{
public:
	NFCHumanServicesModule(NFIPluginManager* p);
	virtual ~NFCHumanServicesModule();

	/*
	** ���������Ҫ������������ָ��, ��̬���ص�ʱ������ָ����ܻ�ʧЧ
	*/
	virtual bool DynamicLoadPlugin() override;
};