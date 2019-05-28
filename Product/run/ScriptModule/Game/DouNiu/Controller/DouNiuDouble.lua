module("DouNiuDouble", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_2_pb.cgdouniudouble()
	local gcmsg = msg_douniu_2_pb.gcdouniudouble()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuDouble", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuDouble", ReturnCode["player_not_exist"], "该玩家不存在")
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
	for i = 1,#tInfo.situser do
		if cgmsg.userid == tInfo.situser[i].userid then
			gcmsg.chairid = tInfo.situser[1].chairid
			tInfo.situser[i].double = 3
			for j = 1, #tInfo.situser[i].pourList do
				tInfo.situser[i].pourList[j] = tInfo.situser[i].pourList[j] * 2
				if tInfo.TZmax ~= g_douNiuTZ_no then
					if tInfo.situser[i].pourList[j] > g_douNiuPour_fenshu[tInfo.difen][1] * tInfo.TZmax then
						tInfo.situser[i].pourList[j] = g_douNiuPour_fenshu[tInfo.difen][1] * tInfo.TZmax
					end
				end
				gcmsg.pokerList:append(tInfo.situser[i].pourList[j])
			end
		end
	end
	
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	
	local userIDList = DouNiuService.GetUserIDList(tInfo, cgmsg.userid)
	SendMessage(userIDList, PacketCode[3062].client,gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end






