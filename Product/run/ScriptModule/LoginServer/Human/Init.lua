require("Human.Controller.AccountLogin")

require("common.st_human_pb")
require("common.st_shop_pb")
require("common.msg_human_pb")
require("common.define.HumanDefine")
require("common.packet.packet_human")

require("Human.Model.PlayerModel")
require("Human.Model.MailModel")
require("Human.Model.OnlineModel")
require("Human.Model.AgencyModel")

g_redisIndex[OnlineModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[PlayerModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[MailModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[AgencyModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
