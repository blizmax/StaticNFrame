// -------------------------------------------------------------------------
//    @FileName         :    NFLhdControllerModule.h
//    @Author           :    GaoYi
//    @Date             :    2019/05/20
//    @Email			:    445267987@qq.com
//    @Module           :    NFGameLhdPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFServerLogic/NFServerLogicCommon/NFILhdControllerModule.h"
#include "NFServerLogic/NFServerLogicCommon/NFIPlayerModule.h"

class NFCLhdControllerModule : public NFILhdControllerModule
{
public:
	NFCLhdControllerModule(NFIPluginManager* p);

	virtual ~NFCLhdControllerModule();

	virtual bool Init();

	virtual bool Shut();

	/*
	** 这个函数主要用来保存引擎指针, 动态加载的时候引擎指针可能会失效
	*/
	virtual bool DynamicLoadPlugin();
public:
	/**
	* @brief 处理创建桌子
	*
	* @return void
	*/
	void OnHandleCreateTable(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);
private:
	NFIPlayerModule* m_pPlayerModule;
};

