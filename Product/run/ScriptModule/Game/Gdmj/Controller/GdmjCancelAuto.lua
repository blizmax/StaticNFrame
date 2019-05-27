--离开麻将的牌桌
module("GdmjCancelAuto", package.seeall)

function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_gdmj_pb.cggdmjcancelauto()
	local gcmsg = msg_gdmj_pb.gcgdmjcancelauto()
	
	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "Majiang", "MajiangCancelAuto", 0, "缓存已经存在")
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		--如果在缓存中找不到对应的玩家的playerInfo, 则让玩家退出从新登陆拉去玩家信息
		LogBehavior.Error(cgmsg.userid, "Majiang", "MajiangCancelAuto", 0, "玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	if cgmsg.tableid == 0 then
    	gcmsg.result = ReturnCode["gdmj_not_exist"]
		LogBehavior.Warning(pInfo,"Majiang", "MajiangCancelAuto", 0, "牌桌不存,tableID="..cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if cgmsg.tableid ~= tonumber(GdmjModel.GetUserTableID(cgmsg.userid)) then
		--如果不是在牌桌中，就下发离开桌面的协议
    	gcmsg.result = ReturnCode["gdmj_not_exist"]
		LogBehavior.Warning(pInfo,"Majiang", "MajiangCancelAuto", 0, "牌桌不存222,tableID="..cgmsg.tableid)

		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	ThreadManager.GdmjLock(cgmsg.tableid)
	local tInfo = GdmjModel.GetTableInfo(cgmsg.tableid)
	gcmsg.chairid = 0
	for k,v in ipairs(tInfo.situser) do
		if v == cgmsg.userid then
			gcmsg.chairid = k
			break
		end
	end
	
	if gcmsg.chairid == 0 then
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_player_not"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local mjUser = GdmjModel.GetChairUser(cgmsg.tableid, cgmsg.userid)
	mjUser.autoplayer = 0
	GdmjModel.SetChairUser(cgmsg.tableid,mjUser)
	
	gcmsg.autoplayer = 0
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	GdmjModel.SetTableInfo(tInfo, 1)
	local userIDList = GdmjWork.GetUserList(tInfo, cgmsg.userid)
	ThreadManager.GdmjUnLock(cgmsg.tableid)
	SendMessage(userIDList, PacketCode[2233].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end