// -------------------------------------------------------------------------
//    @FileName         :    NFServerCommon.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#include "NFServerCommon.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

NFServerConfig* NFServerCommon::GetServerConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType)
{
	NFServerConfig* pServerConfig = nullptr;
	if (pPluginManager->IsLoadAllServer())
	{
		std::vector<NFServerConfig*> vecServerConfig = NFConfigMgr::Instance()->GetServerConfigFromServerType(eServerType);
		if (!vecServerConfig.empty())
		{
			pServerConfig = vecServerConfig.front();
		}
	}
	else
	{
		NFServerConfig* pAppConfig = NFConfigMgr::Instance()->GetServerConfig(pPluginManager->GetAppID());
		if (pAppConfig == nullptr) return nullptr;

		std::vector<NFServerConfig*> vecServerConfig = NFConfigMgr::Instance()->GetServerConfigFromServerType(eServerType);
		if (!vecServerConfig.empty())
		{
			for (int i = 0; i < vecServerConfig.size(); i++)
			{
				NFServerConfig* pConfig = vecServerConfig[i];
				if (eServerType == NF_ST_MASTER)
				{			
					return pConfig;
				}
				else if (eServerType == NF_ST_LOGIN)
				{
					if (pConfig->mGameId == pAppConfig->mGameId)
					{
						return pConfig;
					}
				}
				else if (eServerType == NF_ST_WORLD)
				{
					if (pConfig->mGameId == pAppConfig->mGameId)
					{
						return pConfig;
					}
				}
				else if (eServerType == NF_ST_GAME)
				{
					if (pConfig->mGameId == pAppConfig->mGameId)
					{
						return pConfig;
					}
				}
				else if (eServerType == NF_ST_PROXY)
				{
					if (pConfig->mGameId == pAppConfig->mGameId)
					{
						return pConfig;
					}
				}
			}
		}
		
	}
	return pServerConfig;
}

NFServerConfig* NFServerCommon::GetAppConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType)
{
	NFServerConfig* pServerConfig = nullptr;
	if (pPluginManager->IsLoadAllServer())
	{
		std::vector<NFServerConfig*> vecServerConfig = NFConfigMgr::Instance()->GetServerConfigFromServerType(eServerType);
		if (!vecServerConfig.empty())
		{
			pServerConfig = vecServerConfig.front();
		}
	}
	else
	{
		pServerConfig = NFConfigMgr::Instance()->GetServerConfig(pPluginManager->GetAppID());
	}
	return pServerConfig;
}

bool NFServerCommon::MessageToJsonString(const google::protobuf::Message& msg, std::string& json)
{
	google::protobuf::util::JsonPrintOptions options;
	options.always_print_enums_as_ints = true;
	options.preserve_proto_field_names = true;

	google::protobuf::util::Status status = google::protobuf::util::MessageToJsonString(msg, &json, options);
	if (!status.ok())
	{
		NFLogError("MessageToJsonString Error | {}", status.error_message().ToString());
	}
	return status.ok();
}

bool NFServerCommon::JsonStringToMessage(const string& json, google::protobuf::Message& msg)
{
	google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;
	google::protobuf::util::Status status = google::protobuf::util::JsonStringToMessage(json, &msg, options);
	if (!status.ok())
	{
		NFLogError("JsonStringToMessage Error | {}", status.error_message().ToString());
	}
	return status.ok();
}
