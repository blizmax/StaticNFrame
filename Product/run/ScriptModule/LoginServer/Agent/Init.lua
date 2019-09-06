
--鉴于代理系统的复杂性。把代理系统单独出来作为一个模块来进行处理

require("Agent.Model.AgentModel")
require("common.st_agent_pb")

g_redisIndex[AgentModel.redis_index] = {index = g_redisInfo.redis_two, key = AgentModel.redis_index, link = 1}

