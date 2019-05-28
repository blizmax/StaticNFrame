module("DouNiuBanker", package.seeall)

--这是抢庄操作
--只有明牌抢庄和自由抢庄才有这操作



function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_douniu_pb.cgdouniubanket()
	local gcmsg = msg_douniu_pb.gcdouniubanket()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.senderid, "douniu", "DouNiuBanker", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_not_exist"]
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	
	--print("cgmsg.bankerstate = " ..cgmsg.bankerstate)
	--print("cgmsg.bankermul =" ..cgmsg.bankermul)
	--print("cgmsg.bankermul = "..cgmsg.bankermul)
	--print("tInfo.bankermultiple = "..tInfo.bankermultiple)
	if cgmsg.bankermul > tInfo.bankermultiple and tInfo.dntype == g_douNiuType.type_mpqz then  --明牌抢庄限制最高抢庄倍数
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_bankermul_error"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuBanker", 0,"抢庄的倍数太高")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	--不在抢庄状态不能抢庄
	if tInfo.state ~= g_douNiuDefine.state_banker then
		
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = 0
		--LogFile("error","DouNiuDissolve table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	--不是坐下的不玩家不能操作
	local issit = false
	local isbanker = false 
	for k,v in ipairs(tInfo.situser) do
		if cgmsg.userid == v.userid then
			issit = true
			if v.state == g_douniuPlayStatus.status_next then
				isbanker = true 
			end
			--检查是否够分数抢庄
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
				if tInfo.bankercount > v.carryjetton then
					gcmsg.result = ReturnCode["jlb__banker_jetton_error"]
					ThreadManager.DouNiuUnLock(cgmsg.tableid)
					return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
				end
			end
		end
	end
	
	if isbanker == false then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = 0
		--LogFile("error","DouNiuDissolve table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	end
	
	
	if issit == false then
		gcmsg.result = ReturnCode["douniu_opt_error"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuBanker", 0,"没坐下或者是新坐下的")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	if tInfo.julebutype == 2 then
		--检查参与抢庄的分数
		if false == DouNiuEvent.CheckBankerJiFen(tInfo.julebuid, cgmsg.userid) then
			gcmsg.result = ReturnCode["douniu_opt_error"]
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
	end
	
	
	for j = 1, #tInfo.situser do
		if cgmsg.userid == tInfo.situser[j].userid then
			tInfo.situser[j].basicsmul = cgmsg.bankermul
			--print("tInfo.situser[j].basicsmul = "..tInfo.situser[j].basicsmul)
			tInfo.situser[j].state = cgmsg.bankerstate
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
			gcmsg.chairid = tInfo.situser[j].chairid
		end
	end
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	
	
	gcmsg.userid = cgmsg.userid
	gcmsg.bankermul = cgmsg.bankermul
	gcmsg.bankerstate = cgmsg.bankerstate
	
	gcmsg.result = 0
	local userIDList =  DouNiuService.GetUserIDList( tInfo, cgmsg.userid )
	SendMessage(userIDList, PacketCode[3030].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	local isOverTime = false
	for k,v in ipairs(tInfo.situser) do
		if v.state == g_douniuPlayStatus.status_next then
			isOverTime = true
		end
	end
	
	if isOverTime == false then
		DouNiuService.sendBanker(tInfo)
	end
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	
	return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	
end
