require("Julebu.Controller.JulebuApply")
require("Julebu.Controller.JulebuApplyList")
require("Julebu.Controller.JulebuCreate")
require("Julebu.Controller.JulebuEnter")
require("Julebu.Controller.JulebuKict")
require("Julebu.Controller.JulebuList")
require("Julebu.Controller.JulebuMemberList")
require("Julebu.Controller.JulebuPass")
require("Julebu.Controller.JulebuTableList")
require("Julebu.Controller.JulebuChat")
require("Julebu.Controller.JulebuChatList")
require("Julebu.Controller.JulebuLeave")
require("Julebu.Controller.JulebuQuit")
require("Julebu.Controller.JulebuHistory")
require("Julebu.Controller.JulebuNowJiFen")
require("Julebu.Controller.JulebuSelfJiFen")
require("Julebu.Controller.JulebuSetLimitJiFen")
require("Julebu.Controller.JulebuSetNotice")
require("Julebu.Controller.JulebuDayHistory")
require("Julebu.Controller.JulebuSelfDay")
require("Julebu.Controller.JulebuSetAdmin")
require("Julebu.Controller.JulebuDianZan")
require("Julebu.Controller.JulebuInvite")
require("Julebu.Controller.JulebuPlayingMethod")
require("Julebu.Controller.JulebuSetBiSai")
require("Julebu.Controller.JulebuGetJimitJiFenHistory")
require("Julebu.Controller.JulebuGetPartner")
require("Julebu.Controller.JulebuAddNextPartner")
require("Julebu.Controller.JulebuAddPartner")
require("Julebu.Controller.JulebuDalPartnerData")
require("Julebu.Controller.JulebuCreaktable")


require("Julebu.Controller.JulebuGetAward")
require("Julebu.Controller.JulebuGetAwardInfo")
require("Julebu.Controller.JulebuAwardList")
require("Julebu.Controller.JulebuSetProportion")

require("common.st_julebu_pb")
require("common.msg_julebu_pb")
require("common.msg_julebu2_pb")

require("common.define.JulebuDefine")
require("common.packet.packet_julebu")

require("Julebu.Model.JulebuModel")
require("Julebu.Services.JulebuService")



g_redisIndex[JulebuModel.redis_index] = {index = g_redisInfo.redis_two, des="Julebu"}     --系统唯一的邮件

