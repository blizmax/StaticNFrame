// -------------------------------------------------------------------------
//    @FileName         :    NFServerDefine.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/05/25
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#include "NFServerDefine.h"

std::string GetServerName(NF_SERVER_TYPES serverId)
{
	if (serverId >= 0 && serverId < NF_ST_MAX)
	{
		return gArrayServer[serverId];
	}
	return std::string();
}

