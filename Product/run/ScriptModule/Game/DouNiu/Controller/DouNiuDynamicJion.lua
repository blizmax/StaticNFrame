module("DouNiuDynamicJion", package.seeall)
--没有用的这个协议
function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_douniu_2_pb.cgdouiudynamicjion()
	local gcmsg = msg_douniu_2_pb.gcdouiudynamicjion()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)

	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuDynamicJion", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuDynamicJion", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	--在游戏开始以后，动态加入的玩家先把他坐下来标记为新坐下的玩家，但不要通知客户端，在下局游戏开始是，把玩家状态设置为不是新坐下并通知客户端玩家坐下
	local tableID = DouNiuModel.GetUserTableID(cgmsg.userid)
	if tableID == nil then	
		if cgmsg.tableid ~= 0 then
			tableID = cgmsg.tableid
		else --这里
			--tableID = BrnnService.GetTableID(pInfo.jetton)
			LogBehavior.Error(cgmsg.userid, "douniu", "DouNiuDynamicJion", ReturnCode["douniu_not_exist"], "牌桌不存在，tableid="..tableID)
			
			gcmsg.result = ReturnCode["douniu_not_exist"]
				
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()			
		end
	end
	
	ThreadManager.DouNiuLock(tableID) 
	local tInfo = DouNiuModel.GetTableInfo(tableID)
	
	if tInfo == nil then
		LogBehavior.Error(cgmsg.userid, "douniu", "DouNiuDynamicJion", ReturnCode["douniu_not_exist"], "牌桌不存在，tableid="..tableID)
		
		gcmsg.result = ReturnCode["douniu_not_exist"]
		ThreadManager.DouNiuUnLock(tableID)
				
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end 
		
	--不允许重复坐下
	for k,v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then 
			gcmsg.result = ReturnCode["douniu_sit_ing"]
			LogBehavior.Warning(cgmsg.userid, "douniu", "douniu_sit_ing", 0,"已经在座位上")
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
	end
	
	if #tInfo.situser >= g_douNiuDefine.max_user then
		gcmsg.result = ReturnCode["douniu_full"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuLeave", 0,"座位满人了")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()		
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	--AA付费扣钻石
	if tInfo.payway == 2 then --AA付费扣钻石
		money1 = math.ceil((tInfo.maxplaynum - tInfo.usevipnum) / 10)
		if money1 >= tInfo.paynum then
			PlayerModel.DecMoney(pInfo, money1, "douniu","create room")  --创建房间
			PlayerModel.SetPlayerInfo(pInfo)	
			PlayerModel.SendPlayerInfo(pInfo,{"money"})
		else
			gcmsg.result = ReturnCode["douniu_money_short"]
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()		
		end
	end	
	
	tInfo.dnuser:append(cgmsg.userid)

	DouNiuModel.SetTableInfo(tInfo,1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	
	gcmsg.userid = cgmsg.userid
	gcmsg.result = 0
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	
end