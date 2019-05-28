module("DouNiuStandup", package.seeall)   

--这里是站起操作（暂时用不上）
--只有坐在的玩家才能站起
--游戏开始了就不能站起

function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_pb.cgdouniustandup()
	local gcmsg = msg_douniu_pb.gcdouniustandup()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuAction", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuAction", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	
	if cgmsg.tableid == 0 then
		--如果tableID 是0，表示进入的是金币场，这边需要随机分配
		--暂时先返回
		
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogFile("error","DouNiuEnter table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	if tInfo.viptable ~= 0 and tInfo.usevipnum ~= 0 then
		--表示牌局已经开始了，不能够站起
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["DouNiu_stand_error"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	--这里是行动的开始
	--
	local ret = false
	for k,v in ipairs(tInfo.situser) do
		if gcmsg == v then
			ret = true
		end
	end 
	if ret == false then
		gcmsg.result = 0
		return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
	end
	
	tInfo.situser[cgmsg.chairid] = 0
	tInfo.playernum = tInfo.playernum - 1
	tInfo.standuser:append(cgmsg.userid)
	
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	
	gcmsg.chairid = cgmsg.chairid
	
	--在这里全部发送了
	local userIDList = DouNiuModel.GetUserList(tInfo, cgmsg.userid)
	gcmsg.result = 0
	SendMessage(userIDList, PacketCode[3016].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end