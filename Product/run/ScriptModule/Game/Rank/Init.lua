require("Rank.Controller.GetRankList")


require("common.msg_rank_pb")
require("common.define.RankDefine")
require("common.packet.packet_rank")

require("Rank.Model.RankModel")
require("Rank.Model.PopularModel")
require("Rank.Model.JettonModel")




g_redisIndex[PopularModel.popular_all] = {index = g_redisInfo.redis_three, key = PopularModel.popular_all, link = 0}
g_redisIndex[RankModel.redis_index] = {index = g_redisInfo.redis_three, des = "RankModel", link = 0}

g_redisIndex[JettonModel.redis_index] = {index = g_redisInfo.redis_three, des = "RankModel", link = 0}

g_redisIndex[PopularModel.redis_index] = {index = g_redisInfo.redis_three, des = "RankModel", link = 0}

