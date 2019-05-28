module("DouNiuHintPoker", package.seeall)

--这里为每局游戏结束后的准备操作


function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_2_pb.cgdouniuhintpoker()
	local gcmsg = msg_douniu_2_pb.gcdouniuhintpoker()
	
	cgmsg:ParseFromString(buffer)


	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuHintPoker", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuHintPoker", ReturnCode["player_not_exist"], "该玩家不存在")
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

	for k,v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then
			gcmsg.chairid = v.chairid
			if v.nntype > g_douNiuCard_type.niu_niu then
				gcmsg.hintType = 3
			elseif v.nntype > g_douNiuCard_type.wu_niu then
				gcmsg.hintType = 2
				local inList = {}
				local pokerList = {}
				for z, y in ipairs(v.handpoker) do
					table.insert(inList, y)
				end
				DouNiuHuUtils.GetCardType(inList, tInfo, pokerList)
				for i = 1, #pokerList do
					gcmsg.pokerList:append(pokerList[i])
				end
			else
				gcmsg.hintType = 1
			end
			
			
		end
	end
	
	DouNiuModel.SetTableInfo(tInfo,1)
	ThreadManager.DouNiuUnLock(tableID)
	
	gcmsg.userid = cgmsg.userid
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()

end