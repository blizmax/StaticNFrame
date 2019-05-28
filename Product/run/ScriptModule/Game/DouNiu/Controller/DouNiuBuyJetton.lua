module("DouNiuBuyJetton", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_2_pb.cgdouniubuyjetton()
	local gcmsg = msg_douniu_2_pb.gcdouniubuyjetton()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuBuyJetton", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuBuyJetton", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["douniu_jetton_notenough"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	ThreadManager.DouNiuLock(cgmsg.tableid)
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuBuyJetton", 0,"牌桌不存在")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	--不在下注状态不能下注
	if tInfo.state ~= g_douNiuDefine.state_wait then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = 0
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end	
	
	--这里要查到下注的最多的筹码
	--庄家不用下注
	if cgmsg.userid == tInfo.bankerID then
		gcmsg.result = ReturnCode["douniu_baker_pour"]
		--LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuPourLeave", 0,"牌桌不存在")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	--不是坐下的不玩家不能操作
	local issit = false
	for k,v in ipairs(tInfo.situser) do
		if cgmsg.userid == v.userid then
			issit = true
		end
	end
	
	if issit == false then
		gcmsg.result = ReturnCode["douniu_opt_error"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuPour", 0,"没坐下或者是新坐下的")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	--不能买庄家
	if cgmsg.buyuser == tInfo.bankerID then
		gcmsg.result = ReturnCode["douniu_buy_banker"]
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	local buyPos = 0
	for i = 1, #tInfo.situser do 
		if cgmsg.userid == tInfo.situser[i].userid then
			tInfo.situser[i].buyjetton.userid = cgmsg.buyuser 
			tInfo.situser[i].buyjetton.pour = cgmsg.pour 
			tInfo.situser[i].isbuy= 3
		end
		if cgmsg.buyuser == tInfo.situser[i].userid then			
			tInfo.situser[i].buyInfo = tInfo.situser[i].buyInfo + cgmsg.pour 
			buyPos = i
		end
	end
	
	

	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	gcmsg.buyuser = cgmsg.buyuser
	gcmsg.pour = tInfo.situser[buyPos].buyInfo
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)

	local userIDList = DouNiuService.GetUserIDList(tInfo, cgmsg.userid)
	SendMessage(userIDList, PacketCode[3058].client,gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end






