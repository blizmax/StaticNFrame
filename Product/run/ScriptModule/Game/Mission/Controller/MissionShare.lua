
module("MissionShare", package.seeall)


function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_mission_pb.cgmissionshare()
	local gcmsg = msg_mission_pb.gcmissionshare()
	gcmsg.result = 0
	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "hlyd", "GetRankList", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	if true == redisItem:hexists("hlyd_share_award", cgmsg.userid) then
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo ~= nil  and pInfo.popularity < 3000 and cgmsg.result == 1 then
		PlayerModel.AddPopular(pInfo, 100,"hlyd","分享获得")
		PlayerModel.SendPlayerInfo(pInfo,{"popularity"})
		PlayerModel.SetPlayerInfo(pInfo)
		redisItem:hset("hlyd_share_award",cgmsg.userid,pInfo.popularity)
	end
	
	if cgmsg.result == 0 then
		gcmsg.result = ReturnCode["hlyd_share_fail"]
	end
	
	return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end



