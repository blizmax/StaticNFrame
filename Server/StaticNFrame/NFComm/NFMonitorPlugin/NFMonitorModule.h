// -------------------------------------------------------------------------
//    @FileName         :    NFMonitorModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIMonitorModule.h"
#include "NFComm/NFPluginModule/NFSystemInfo.h"
#include "NFComm/NFPluginModule/NFTimerMgr.h"
#include "NFComm/NFPluginModule/NFEventMgr.h"

/**
* @brief ���������ģ��
*/
class NFCMonitorModule : public NFIMonitorModule, public NFTimerObj, public NFEventObj
{
public:
	NFCMonitorModule(NFIPluginManager* p);

	virtual ~NFCMonitorModule();
public:
	virtual bool Init();

	virtual bool Execute();

	virtual bool Finalize();

	/**
	* @brief ����ʱ������
	*
	* @return void
	*/
	virtual void OnTimer(uint32_t nTimerID);

	/**
	* @brief �����¼�
	*
	* @return void
	*/
	virtual void OnExecute(uint16_t nEventID, uint64_t nSrcID, uint8_t bySrcType, const google::protobuf::Message& message);

	virtual const NFSystemInfo& GetSystemInfo() const;

	virtual uint32_t GetUserCount() const;
	virtual void SetUserCount(uint32_t count);
private:
	NFSystemInfo mSystemInfo;
};
