// -------------------------------------------------------------------------
//    @FileName         :    NFCCallBackModule.h
//    @Author           :    GaoYi
//    @Date             :    2018-10-31
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFICallBackModule.h"
#include <map>
#include <functional>
#include "NFComm/NFPluginModule/NFTimerMgr.h"


class NFCCallBackModule : public NFICallBackModule, public NFTimerObj
{
	struct NFCallBack
	{
		NFCallBack()
		{
			m_ptrFunc = nullptr;
			m_timeOut = 0;
		}

		void* m_ptrFunc;		//函数指针
		int64_t m_timeOut;		//超时时间ms
		uint64_t m_guid;		//拥有者
		std::string m_desc;		//描述信息
	};
public:
	NFCCallBackModule(NFIPluginManager* p);
	virtual ~NFCCallBackModule();

	virtual bool Init();

	virtual bool Execute();

	virtual bool Finalize();

	virtual void OnTimer(uint32_t nTimerID);
private:
	void HandleTimeout();

	bool ProcessTimeout(const NFCCallBackModule::NFCallBack& back);
private:
	std::map<uint32_t, NFCallBack> m_callbackMap;
};
