
module("JingSaiTableList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaitablelist()
	local gcmsg = msg_jingsai_pb.gcjingsaitablelist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "jingsai", "JingSaiTableList", 0, "获取俱乐部列表，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "jingsai", "JingSaiTableList", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	local getList = JingSaiModel.GetGameList(cgmsg.jingsaiid, cgmsg.pagenum)
	
	for k,v in ipairs(getList) do    --
		local strGet = JingSaiModel.GetGameInfo(cgmsg.jingsaiid, v)
		if strGet ~= nil then
			local addItem = gcmsg.jslist:add()
			addItem:ParseFromString(strGet)
		else
			JingSaiModel.RemoveGame(cgmsg.jingsaiid, v) --如果没有取到，就删除
		end
	end
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end