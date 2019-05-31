require("JingSai.Controller.JingSaiEnter")
require("JingSai.Controller.JingSaiCreate")
require("JingSai.Controller.JingSaiCreateTable")
require("JingSai.Controller.JingSaiDel")
require("JingSai.Controller.JingSaiList")
require("JingSai.Controller.JingSaiMemberList")
require("JingSai.Controller.JingSaiSetDes")
require("JingSai.Controller.JingSaiTableList")
require("JingSai.Controller.JingSaiHistory")
require("JingSai.Controller.JingSaiUpdate")
require("JingSai.Controller.JingSaiPay")  
require("common.st_jingsai_pb")
require("common.msg_jingsai_pb")

require("common.define.JingSaiDefine")
require("common.packet.packet_jingsai")

require("JingSai.Model.JingSaiModel")
require("JingSai.Services.JingSaiService")


g_redisIndex[JingSaiModel.redis_index] = {index = g_redisInfo.redis_three, des="JingSai"}     --系统唯一的邮件

