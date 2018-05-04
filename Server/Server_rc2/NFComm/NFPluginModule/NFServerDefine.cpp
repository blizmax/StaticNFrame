#include "NFServerDefine.h"

std::string GetServerName(NF_SERVER_TYPES serverId)
{
	if (serverId >= 0 && serverId < NF_ST_MAX)
	{
		return gArrayServer[serverId];
	}
	return std::string();
}