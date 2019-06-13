
module("GdmjLeave", package.seeall)

--该操作是离开房子，在牌局没开始的时候，全部玩家都能够离开房间
--如果是房主离开房间，房间依然会存在。如果是空的牌桌，每个牌桌存活的时间是6个小时

function execute(packetID, operateID, buffer)
	local cgmsg = msg_gdmj_pb.cggdmjleave()
	local gcmsg = msg_gdmj_pb.gcgdmjleave()
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjLeave", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"gdmj","GdmjLeave", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if cgmsg.tableid == 0 then
		--如果tableID 是0，表示进入的是金币场，这边需要随机分配
		--暂时先返回
		--SendMessage(cgmsg.userid, PacketCode[2218].client, gcStandUp:ByteSize(), gcStandUp:SerializeToString())
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
		
	ThreadManager.GdmjLock(cgmsg.tableid)
	
	local tInfo = GdmjModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		--gcmsg.result = ReturnCode["gdmj_not_exist"]  --这里不管牌桌在不在，都让他退出
		
		
		gcmsg.userid = cgmsg.userid
		--LogFile("error","GdmjEnter table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	--local mjUser = GdmjModel.GetChairUser(cgmsg.tableid, cgmsg.userid)

	--if mjUser == nil then
		--牌桌不存在
	--	ThreadManager.GdmjUnLock(cgmsg.tableid)
	--	gcmsg.result = ReturnCode["gdmj_not_exist"]
	--	LogFile("error","GdmjEnter mjUser not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
	--	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	--end	
	--这里是行动的开始
	
	
	
	
	if tInfo.viptable == 0 then
		--非VIP的房间一定要遵守这个规则
		if tInfo.status == g_gdmjStatus.status_waiting or tInfo.status == g_gdmjStatus.status_playing or tInfo.status == g_gdmjStatus.status_counting then
			
			gcmsg.result = ReturnCode["gdmj_leave_playing"]
			
			ThreadManager.GdmjUnLock(cgmsg.tableid)
			
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
	end


	local standLen = #tInfo.standuser
	local isStand = false
	for i = standLen,1,-1 do
		if cgmsg.userid == tInfo.standuser[i] then
			--确保把全部都要删除
			
			isStand = true
			tInfo.standuser:remove(i)
		end
	end
	if isStand == true then
		--一定要确保全部能够
		--如果是在站立的列表中
		GdmjModel.SetTableInfo(tInfo, 1)  --这里需要设置的
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.userid = cgmsg.userid
		GdmjModel.DelUserTableID(cgmsg.userid)   --这里还需要把
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	


	local isExist = false
	for k,v  in ipairs(tInfo.situser) do
		if v == cgmsg.userid then
			isExist = true
			break
		end
	end

	if isExist == false then
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.userid = cgmsg.userid
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	if tInfo.usevipnum ~= 0 then 
		--牌局已经开始了
		--tInfo.usevipnum
		--if tInfo.julebuid ~= 0 or tInfo.state ~= g_gdmjStatus.status_ready then
			gcmsg.result = ReturnCode["gdmj_leave_error"]
			ThreadManager.GdmjUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		--end
	end
	
	--如果牌局还没开始，是可以退出的，退出的时候，检查是否在坐下的列表还是在站起的列表中
	
	if tInfo.usevipnum ~= 0 and tInfo.julebuid ~= 0 then
		local userInfo = GdmjModel.GetChairUser(tInfo.tableid, cgmsg.userid)
		--GdmjEvent.AddJiFen(tInfo.julebuid, cgmsg.userid, userInfo.carryjetton,tInfo.tableid, tInfo.julebutype)   --同步积分
	end
	
	local chairID = 0
	for k,v in ipairs(tInfo.situser) do
		
		if v == cgmsg.userid then
			--表示是坐下的列表中，需要给其他人发送站起的协议
			local gcStandUp = msg_gdmj_pb.gcgdmjstandup()
			gcStandUp.chairid = k
			gcStandUp.tableid = cgmsg.tableid
			local userList = GdmjWork.GetUserList(tInfo, cgmsg.userid)
			
			SendMessage(userList, PacketCode[2218].client, gcStandUp:ByteSize(), gcStandUp:SerializeToString())
			chairID = k
			tInfo.situser[k] = 0    --这里设置为0
			tInfo.playernum = tInfo.playernum - 1
			GdmjEvent.JulebuGameUpdate(tInfo,cgmsg.userid)
			break
		end
		
	end

	if chairID == 0 then
		--表示不在牌桌中，然后是
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.userid = cgmsg.userid
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	GdmjModel.DelUserTableID(cgmsg.userid)   --这里还需要把

	gcmsg.chairid = chairID
	gcmsg.userid = cgmsg.userid
	gcmsg.tableid = cgmsg.tableid
	GdmjModel.SetTableInfo(tInfo, 1)         --这里需要设置的
	ThreadManager.GdmjUnLock(cgmsg.tableid)
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end