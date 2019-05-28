

module("DouNiuPour", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_pb.cgdouniupour()
	local gcmsg = msg_douniu_pb.gcdouniupour()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuPour", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuPour", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["douniu_jetton_notenough"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	ThreadManager.DouNiuLock(cgmsg.tableid)
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuPourLeave", 0,"牌桌不存在")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	--不在下注状态不能下注
	if tInfo.state ~= g_douNiuDefine.state_wait then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = 0
		--LogFile("error","DouNiuDissolve table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end	
	
	--这里要查到下注的最多的筹码
	--庄家不用下注
	--print("cgmsg.userid = "..cgmsg.userid)
	--print("tInfo.bankerID = "..tInfo.bankerID)
	if cgmsg.userid == tInfo.bankerID then
		gcmsg.result = ReturnCode["douniu_baker_pour"]
		--LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuPourLeave", 0,"牌桌不存在")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end

	--不是坐下的不玩家不能操作
	local issit = false
	local isbanker = false 
	for k,v in ipairs(tInfo.situser) do
		if cgmsg.userid == v.userid then
			issit = true
			if v.state == g_douniuPlayStatus.status_banker or v.state == g_douniuPlayStatus.status_buqiang then
				isbanker = true 
			end
		end
	end

	if issit == false then
		gcmsg.result = ReturnCode["douniu_opt_error"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuPour", 0,"没坐下或者是新坐下的")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	if isbanker == false then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = 0
		--LogFile("error","DouNiuDissolve table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	end
	
	--print("cgmsg.pourbei = "..cgmsg.pourbei)~
	for i = 1,#tInfo.situser do
		if cgmsg.userid == tInfo.situser[i].userid then			
			tInfo.situser[i].jetton = cgmsg.pourbei
			--print("tInfo.situser[i].userid = " ..tInfo.situser[i].userid)
			--print("tInfo.situser[i].jetton = " ..tInfo.situser[i].jetton)
			tInfo.situser[i].state = g_douniuPlayStatus.status_wait
			--有操作就把超时次数置 0
			tInfo.situser[i].overtimenum = 0
			--有操作就唤醒不托管
			if tInfo.situser[i].trusteeship == g_douNiuDefine.trusteeship_yes then
				tInfo.situser[i].trusteeship = g_douNiuDefine.trusteeship_no 
				local gcmsg = msg_douniu_2_pb.gcdouniutrusteeship()
				gcmsg.userid = tInfo.situser[i].userid
				gcmsg.trusteeship = g_douNiuDefine.trusteeship_no 
				gcmsg.result = 0
				
				local userIDList =  DouNiuService.GetUserIDList(tInfo)
				SendMessage(userIDList, PacketCode[3054].client, gcmsg:ByteSize(), gcmsg:SerializeToString())				
			end
			break 
		end
	end
	
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	gcmsg.pourbei = cgmsg.pourbei
	if tInfo.TZmax ~= g_douNiuTZ_no then
		local isTZ = false  --检查是否有推注
		for i = 1, #tInfo.situser do 
			if tInfo.situser[i].userid == gcmsg.userid then
				if tInfo.situser[i].double == 3 then
					if tInfo.situser[i].jetton == tInfo.situser[i].TZnum * 2 or tInfo.situser[i].jetton == tInfo.TZmax * g_douNiuPour_fenshu[tInfo.difen][1] then
						isTZ = true
					end
				else
					if tInfo.situser[i].jetton == tInfo.situser[i].TZnum then
						isTZ = true
					end
				end
				break
			end
		end
		
		if isTZ == true then
			gcmsg.isbolus = 1
		end
	end
	
	local isOverTime = false
	for k,v in ipairs(tInfo.situser) do
		if v.state == g_douniuPlayStatus.status_banker or v.state == g_douniuPlayStatus.status_buqiang then
			isOverTime = true
		end
	end
	
	if isOverTime == false then 
		DouNiuService.sendPour(tInfo)
	end
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)

	local userIDList = DouNiuService.GetUserIDList(tInfo, cgmsg.userid)
	SendMessage(userIDList, PacketCode[3020].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end






