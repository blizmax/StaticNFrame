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
#include <functional>
#include <unordered_map>
#include "NFComm/NFPluginModule/NFTimerMgr.h"



class NFCCallBackModule final : public NFICallBackModule, public NFTimerObj
{
	struct NFCallBack
	{
		NFCallBack()
		{
			m_ptrFunc = nullptr;
			m_ptrModule = nullptr;
			m_timeOut = 0;
			m_guid = 0;
		}

		void* m_ptrModule; //模块指针
		void* m_ptrFunc;	//函数指针
		int64_t m_timeOut; //超时时间ms
		uint64_t m_guid; //拥有者
		std::string m_desc; //描述信息
	};

	struct NFAsyMysqlCallBack
	{
		std::unordered_map<uint32_t, NFCallBack> m_updateCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_updateMessageCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_queryOneCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_queryMoreCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_queryMoreWithCondCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_queryMoreWithLimitCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_queryMoreByLikeCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_queryOneMessageCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_queryMoreMessageCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_executeMoreCallBack;
		std::unordered_map<uint32_t, NFCallBack> m_executeOneCallBack;
	};
public:
	NFCCallBackModule(NFIPluginManager* p);
	virtual ~NFCCallBackModule();

	virtual bool Init() override;

	virtual bool Execute() override;

	virtual bool Finalize() override;

	virtual void OnTimer(uint32_t nTimerID) override;
private:
	void HandleTimeout();

	bool ProcessTimeout(const NFCallBack& back);
private:
	NFAsyMysqlCallBack m_asyncMysqlCallBack;
};
