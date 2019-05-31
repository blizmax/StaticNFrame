
module("JingSaiUpdate", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaiupdate()
	local gcmsg = msg_jingsai_pb.gcjingsaiupdate()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "jingsai", "JinSaiEnter", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "jingsai", "JinSaiEnter", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""		
	end

	if nil == JingSaiModel.GetJingSaiInfo(cgmsg.jingsaiid, gcmsg.jsinfo) then
		gcmsg.result = ReturnCode["jingsai_is_end2"]
	else
		gcmsg.result = 0
	end

	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end