
--广东麻将创建桌子

module("GdmjStandUp", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_gdmj_pb.cggdmjstandup()
	local gcmsg = msg_gdmj_pb.gcgdmjstandup()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjAction", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"gdmj","GdmjAction", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	
	
	if cgmsg.tableid == 0 then
		--如果tableID 是0，表示进入的是金币场，这边需要随机分配
		--暂时先返回
		
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	ThreadManager.GdmjLock(cgmsg.tableid)
	
	local tInfo = GdmjModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		LogFile("error","GdmjEnter table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	if tInfo.viptable ~= 0 and tInfo.usevipnum ~= 0 then
		--表示牌局已经开始了，不能够站起
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_stand_error"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	--这里是行动的开始
	--
	
	local userList = GdmjWork.GetUserList(tInfo)
	if cgmsg.chairid == 0 then
		for k,v in ipairs(tInfo.situser) do
			if v == cgmsg.userid then
				cgmsg.chairid = k
				break
			end
		end
	end
	
	if cgmsg.chairid == 0 then
		--如果到这里还是等于0，就表示出错了
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_opt_error"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	tInfo.situser[cgmsg.chairid] = 0
	tInfo.playernum = tInfo.playernum - 1
	tInfo.standuser:append(cgmsg.userid)
	
	GdmjModel.SetTableInfo(tInfo, 1)
	ThreadManager.GdmjUnLock(cgmsg.tableid)
	gcmsg.chairid = cgmsg.chairid
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	gcmsg.tableid = cgmsg.tableid
	--在这里全部发送了
	
	SendMessage(userList, PacketCode[2218].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
	
end