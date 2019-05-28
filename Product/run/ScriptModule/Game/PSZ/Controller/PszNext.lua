module("PszNext", package.seeall)

--这里为每局游戏结束后的准备操作


function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_pb.cgpsznext()
	local gcmsg = msg_psz_pb.gcpsznext()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszNext", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"psz","PszNext", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	--这里开始业务
	
	local tableID = cgmsg.tableid
	
	if tableID == 0 then
		tableID = PszModel.GetUserTableID(cgmsg.userid)
	end
	
	if tableID == nil or tableID == 0 then
		if isPrivate ~= nil then
			gcmsg.result = ReturnCode["psz_not_exist"]
			return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end		
	end

	ThreadManager.PszLock(tableID)
	
	local tInfo = PszModel.GetTableInfo(tableID)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.PszUnLock(tableID)
		gcmsg.result = ReturnCode["psz_not_sit"]
		LogFile("error","PszEnter table not exist tableid="..tableID..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--不在等待下一局状态，不能准备
	if tInfo.state ~= g_PszDefine.state_next and tInfo.state ~= g_PszDefine.state_ready then
		ThreadManager.PszUnLock(tableID)
		gcmsg.result = 0
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	gcmsg.userid = cgmsg.userid
--	if tInfo.state ~= g_PszDefine.state_next then
--		ThreadManager.PszUnLock(tableID)
--		gcmsg.result = ReturnCode["psz_opt_error"]
--		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
--	end



	--不是坐下的玩家不能准备
	local isSit = false
	local isNext = true
	for j = 1, #tInfo.situser do
		if tInfo.situser[j].userid == cgmsg.userid then
			isSit = true
			gcmsg.chairid = tInfo.situser[j].chairid
			tInfo.situser[j].state = g_PszPlayStatus.status_next 
		end
		
		if tInfo.situser[j].state ~= g_PszPlayStatus.status_next  then
			isNext = false
		end
	end
	
	if isSit == false then
		ThreadManager.PszUnLock(tableID)
		gcmsg.result = ReturnCode["psz_not_sit"]
		LogFile("error","PszEnter table not exist tableid="..tableID..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	--判断是不是都准备完成了
	if isNext == true and tInfo.state == g_PszDefine.state_next then
		tInfo.timemark = 0
	end
	
	PszModel.SetTableInfo(tInfo,1)
	ThreadManager.PszUnLock(tableID)
	
	local userList = PszService.GetUserIDList(tInfo, cgmsg.userid)
	gcmsg.result = 0
	SendMessage(userList, PacketCode[3326].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()

end




