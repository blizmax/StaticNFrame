module("UpdatePlayerInfo", package.seeall)

--获取用户的信息

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgupdateplayerinfo()
	local gcmsg = msg_human_pb.gcupdateplayerinfo()
	cgmsg:ParseFromString(buffer)

	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "pInfo", "UpdatePlayerInfo", 0, "修改玩家信息")
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "pInfo", "UpdatePlayerInfo", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	
	PlayerModel.ModifyPInfo(pInfo,cgmsg)
	
	--gcmsg.result = ReturnCode["success"]
	--gcmsg.result = 0
	PlayerModel.SetPlayerInfo(pInfo)
	cgmsg.userid = 0  --明白这里为什么可以这样写的
	return pInfo.userid, 0, cgmsg:ByteSize(), cgmsg:SerializeToString()
	
end