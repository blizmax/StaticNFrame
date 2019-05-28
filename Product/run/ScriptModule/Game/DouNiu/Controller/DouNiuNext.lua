module("DouNiuNext", package.seeall)

--这里为每局游戏结束后的准备操作


function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_pb.cgdouniunext()
	local gcmsg = msg_douniu_pb.gcdouniunext()
	
	cgmsg:ParseFromString(buffer)


	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuNext", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuNext", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--这里开始业务
	
	local tableID = cgmsg.tableid
	
	if tableID == 0 then
		tableID = DouNiuModel.GetUserTableID(cgmsg.userid)
	end
	
	if tableID == nil or tableID == 0 then
		if isPrivate ~= nil then
			gcmsg.result = ReturnCode["douniu_not_exist"]
			return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end		
	end

	ThreadManager.DouNiuLock(tableID)
	
	local tInfo = DouNiuModel.GetTableInfo(tableID)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.DouNiuUnLock(tableID)
		gcmsg.result = ReturnCode["douniu_not_sit"]
		LogFile("error","DouNiuEnter table not exist tableid="..tableID..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if tInfo.state ~= g_douNiuDefine.state_next then
		ThreadManager.DouNiuUnLock(tableID)
		gcmsg.result = 0
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	--不是坐下的玩家不能准备
	local isSit = false
	for j = 1, #tInfo.situser do
		
		if tInfo.situser[j].userid == cgmsg.userid then
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
				if tInfo.ruleset == 1 then
					if tInfo.situser[j].carryjetton <= 0 and tInfo.situser[j].state == g_douniuPlayStatus.status_ready then
						ThreadManager.DouNiuUnLock(tableID)
						gcmsg.result = 0
						return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
					end					
				end
			end
			
			isSit = true
			gcmsg.chairid = tInfo.situser[j].chairid
			tInfo.situser[j].state = g_douniuPlayStatus.status_next
			--有操作就把超时次数置 0
			tInfo.situser[j].overtimenum = 0
			--有操作就唤醒不托管
			if tInfo.situser[j].trusteeship == g_douNiuDefine.trusteeship_yes then
				tInfo.situser[j].trusteeship = g_douNiuDefine.trusteeship_no 
				local gcmsg = msg_douniu_2_pb.gcdouniutrusteeship()
				gcmsg.userid = tInfo.situser[j].userid
				gcmsg.trusteeship = g_douNiuDefine.trusteeship_no 
				gcmsg.result = 0
				
				local userIDList =  DouNiuService.GetUserIDList(tInfo)
				SendMessage(userIDList, PacketCode[3054].client, gcmsg:ByteSize(), gcmsg:SerializeToString())				
			end
		end
	end
	
	if isSit == false then
		ThreadManager.DouNiuUnLock(tableID)
		gcmsg.result = ReturnCode["douniu_not_sit"]
		LogFile("error","DouNiuEnter table not exist tableid="..tableID..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local isOverTime = true
	local readNum = 0
	for k,v in ipairs(tInfo.situser) do
		if v.state ~= g_douniuPlayStatus.status_next then
			local isT = false 
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
				if tInfo.ruleset == 1 then
					if v.carryjetton <= 0 and v.state == g_douniuPlayStatus.status_ready then
						isT = true 
					end					
				end
			end
			if isT == false then 
				isOverTime = false
			end
			if isOverTime == true then
				readNum = readNum + 1
			end
		end
	end
	
	if isOverTime == true and readNum >= 2 then
		tInfo.state =  g_douNiuDefine.state_start
		tInfo.timemark = 0
	end
	
	
	
	DouNiuModel.SetTableInfo(tInfo,1)
	ThreadManager.DouNiuUnLock(tableID)
	
	local userList = DouNiuModel.GetUserList(tInfo, cgmsg.userid)
	gcmsg.result = 0
	SendMessage(userList, PacketCode[3034].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()

end