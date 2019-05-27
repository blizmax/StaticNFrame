require("Human.Controller.AccountLogin")
require("Human.Controller.GetPlayerInfo")
require("Human.Controller.Heartbeat")
require("Human.Controller.ReConnect")
require("Human.Controller.GetInitInfo")
require("Human.Controller.GetMailList")
require("Human.Controller.ReadMail")
require("Human.Controller.GetMailGoods")
require("Human.Controller.PlayerStatus")
require("Human.Controller.UpdatePlayerInfo")
require("Human.Controller.AgencyOpt")
require("Human.Controller.EnterTable")
require("Human.Controller.InviteBind")
require("Human.Controller.GetAgencyInfo")
require("Human.Controller.AgencyGiveMoney")
require("Human.Controller.ApplyGroupMaster")
require("Human.Controller.AgencyGiveMoneyRecord")
require("Human.Controller.Registration")
require("Human.Controller.LuckTurnTable")
require("Human.Controller.UpdatePlayerMoney")
require("Human.Controller.AlterSomePlayerInfo")


require("common.st_shop_pb")
require("common.msg_shop_pb")
require("common.st_human_pb")
require("common.msg_human_pb")
require("common.define.HumanDefine")
require("common.packet.packet_human")
require("common.utils.MajiangHuUtils")

require("common.utils.MajiangUtils")

require("common.data.db_goods")

require("Human.Model.PlayerModel")
require("Human.Model.PlayerStatsModel")
require("Human.Model.GoodsModel")
require("Human.Model.OnlineModel")
require("Human.Model.NoticeModel")
require("Human.Model.BroadCastModel")
require("Human.Model.ChatModel")
require("Human.Model.MailModel")
require("Human.Model.AgencyModel")
require("Human.Model.SpecialModel")



require("Human.Services.TimeCheckService")
require("Human.Services.SysCountService")

g_redisIndex[NoticeModel.redis_index] = {index = g_redisInfo.redis_one, des="notice"}     --系统唯一的邮件

g_redisIndex[OnlineModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件

g_redisIndex[PlayerStatsModel.redis_key] = {index = g_redisInfo.redis_one, key = PlayerStatsModel.redis_key, link = 1}  --每个玩家自己的邮件

g_redisIndex[GoodsModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件

g_redisIndex[BroadCastModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件

g_redisIndex[ChatModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件

g_redisIndex[MailModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件

g_redisIndex[PlayerModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件

g_redisIndex[PlayerStatsModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件

g_redisIndex[AgencyModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
