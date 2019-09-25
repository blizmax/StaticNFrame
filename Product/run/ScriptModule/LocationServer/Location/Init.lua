
require("LocationServer.Location.Model.LocationModel")

require("Human.Model.PlayerModel")

require("LocationServer.Location.Services.LocationService")

require("LocationServer.Location.Worker.CheckIPLocation")

require("common.st_human_pb")

g_redisIndex[LocationModel.redis_index] = {index = g_redisInfo.redis_one, des="location"}  --每个玩家自己的邮件
g_redisIndex[PlayerModel.redis_index] = {index = g_redisInfo.redis_one, key = PlayerModel.redis_index, link = 1}


