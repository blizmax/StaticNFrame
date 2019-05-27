
--require("Mission.Model.MissionModel")


require("common.msg_mission_pb")
require("common.define.MissionDefine")
require("common.packet.packet_mission")

require("Mission.Model.MissionModel")
require("Mission.Controller.MissionShare")



g_redisIndex[MissionModel.redis_index] = {index = g_redisInfo.redis_two, des="MissionModel"}  --每个玩家自己的邮件