module("GdmjDissolveOpt", package.seeall)

--该操作是玩家同意或者不同意的时候，

function execute(packetID, operateID, buffer)
	local cgmsg = msg_gdmj_pb.cggdmjdissolveopt()
	local gcmsg = msg_gdmj_pb.gcgdmjdissolveopt()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjDissolveOpt", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end	
	
	ThreadManager.GdmjLock(cgmsg.tableid)
	
	local tInfo = GdmjModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		LogFile("error","GdmjDissolveOpt table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	
	local allowNum = tInfo.maxplayernum - 2   --这里设置为2就行了
	local chairID = 0
	for k,v in ipairs(tInfo.situser) do
		if v == cgmsg.userid then
			chairID = k   --取到chairid
			break
		end
	end
	
	if chairID  == 0 then
		gcmsg.result = ReturnCode["gdmj_opt_error"]
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString() --操作错误
	end
	
	gcmsg.userid = cgmsg.userid
	gcmsg.tableid = cgmsg.tableid
	gcmsg.result = 0
	gcmsg.opttype = cgmsg.opttype
	if cgmsg.opttype == 1 then
		--不同意解散房间
		local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
		
		gcmsg.msg = "由于玩家["..pInfo.nickname.."]不同意解散房间，游戏继续"
		
		for k,v in ipairs(tInfo.situser) do
			if v == cgmsg.userid then
				tInfo.dissolveinfo = tInfo.dissolveinfo + 3*math.pow(10,4 - k)
			end
		end
		
		tInfo.status = tInfo.tablemark
		
		GdmjModel.SetTableInfo(tInfo, 1)
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.dissolveinfo = tInfo.dissolveinfo
		local userList = GdmjWork.GetUserList(tInfo,cgmsg.userid)
		SendMessage(userList, PacketCode[2226].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString() --操作错误
	end
		
	local dissolveInfo = tInfo.dissolveinfo
	for i = 1,4 do  --在这里需要for 1~4；因为顺序是相反的。否则三人房的时候，没法解散
		local disTemp = math.mod(dissolveInfo,10)
		dissolveInfo = math.floor(dissolveInfo/10)
		if disTemp == 2 then
			allowNum = allowNum - 1
		end
	end
	
	tInfo.dissolveinfo = tInfo.dissolveinfo + 2*math.pow(10, 4 - chairID)
	
	gcmsg.dissolveinfo = tInfo.dissolveinfo
	
	
	local userList = GdmjWork.GetUserList(tInfo)
	SendMessage(userList, PacketCode[2226].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	
	if allowNum <= 0 then
		--表示需要解散了
		--在这里需要发送结算的协议的
		--在这里调用接口发送结算的协议
		--在这里设置状态，然后进入结算，不在这里调用结算的接口
		tInfo.timemark = 0
		tInfo.status = g_gdmjStatus.status_dissolve
	elseif allowNum == 1 and (tInfo.mjtype == g_gdmjType.type_hzz or tInfo.mjtype == g_gdmjType.type_hdz) then
		tInfo.timemark = 0
		tInfo.status = g_gdmjStatus.status_dissolve	
	end
	GdmjModel.SetTableInfo(tInfo, 1)
	ThreadManager.GdmjUnLock(cgmsg.tableid)
	return cgmsg.userid, 0, 0, gcmsg:SerializeToString()
	
end