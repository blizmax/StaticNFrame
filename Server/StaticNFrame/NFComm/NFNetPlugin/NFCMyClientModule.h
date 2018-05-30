// -------------------------------------------------------------------------
//    @FileName         :    NFCNetClientModule.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFServerDefine.h"
#include "NFComm/NFCore/NFBuffer.h"
#include "NFMyClient.h"

class NFCMyClientModule : public NFINetClientModule
{
public:
	explicit NFCMyClientModule(NFIPluginManager* p);
	virtual ~NFCMyClientModule();
	/////////////////////////////////////////////////////////////////////
	virtual bool Init() override;

	virtual bool AfterInit() override;

	virtual bool BeforeShut() override;

	virtual bool Shut() override;

	virtual bool Execute() override;

	virtual bool Finalize() override;

	///////////////////////////////////////////////////////////////////////

	virtual uint32_t AddServer(NF_SERVER_TYPES eServerType, const std::string& strIp, const int nPort, bool useThread = true) override;

	virtual void CloseServer(const uint32_t unLinkId) override;

	virtual void CloseServerByServerType(NF_SERVER_TYPES eServerType) override;

	virtual void CloseAllServer() override;

	////////////////////////////////////////////////////////////////////////////////

	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) override;

	virtual void SendByServerID(const uint32_t unLinkId, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	virtual void SendToServerByPB(const uint32_t unLinkId, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) override;

	virtual void SendToAllServer(const uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) override;

	virtual void SendToAllServer(const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	virtual void SendToAllServerByPB(const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) override;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, uint32_t nMsgID, const std::string& strData, const uint64_t nPlayerID) override;

	virtual void SendToAllServer(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID) override;

	virtual void SendToAllServerByPB(NF_SERVER_TYPES eServerType, const uint32_t nMsgID, const google::protobuf::Message& xData, const uint64_t nPlayerID) override;
protected:
	void ProcessExecute();

	virtual uint32_t GetFreeUnLinkId(NF_SERVER_TYPES eServerType);

	void SendMsg(NFMyClient* pClient, const uint32_t nMsgID, const char* msg, const uint32_t nLen, const uint64_t nPlayerID);

	void OnReceiveNetPack(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen);

	void OnSocketNetEvent(const eMsgType nEvent, const uint32_t unLinkId);
private:
	std::vector<std::vector<NFMyClient*>> mxServerMap;
	NFBuffer mxSendBuffer;
};

