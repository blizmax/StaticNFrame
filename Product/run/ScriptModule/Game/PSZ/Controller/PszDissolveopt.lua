module("PszDissolveopt", package.seeall)

--该操作是玩家同意或者不同意的时候，

function execute(packetID, operateID, buffer)
	local cgmsg = msg_psz_pb.cgpszdissolveopt()
	local gcmsg = msg_psz_pb.gcpszdissolveopt()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszDissolveOpt", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end	
	
	ThreadManager.PszLock(cgmsg.tableid)
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid) 
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.PszUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["psz_not_exist"]
		LogFile("error","PszDissolveOpt table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end	
	
	--如果是不是私人房间,不存在解散的概念，直接返回
	if tInfo.tabletype ~= g_PszDefine.table_srf then
		ThreadManager.PszUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["psz_dissolve_fail"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	end
	
	--在不dissolve状态，不能进行该操作
	if tInfo.state ~= g_PszDefine.state_dissolve then
		ThreadManager.PszUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["psz_dissolve_fail"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	
	--if math.floor( tInfo.dissolveinfo, math.pow(10, tInfo.maxuser - tInfo.situser[numIndex].chairid)) <= 0 then
	
	--end
	
	local allowNum = 0   --这里设置为2就行了
	local numIndex = 0
	for k,v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then
			numIndex = k   --取到chairid
			break
		end
	end
	
	if numIndex  == 0 then
		gcmsg.result = ReturnCode["psz_opt_error"]
		ThreadManager.PszUnLock(cgmsg.tableid)
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
			if v.userid == cgmsg.userid then
				local dissolveinfo = tonumber(tInfo.dissolveinfo)
				dissolveinfo = dissolveinfo + 3*math.pow(10, tInfo.maxuser - tInfo.situser[numIndex].chairid)
				tInfo.dissolveinfo = tostring(dissolveinfo)
				break
			end
		end
		
		gcmsg.dissolveinfo = tInfo.dissolveinfo
		
		tInfo.state = tInfo.tablemark
		tInfo.timemark = tInfo.tabletime
		tInfo.dissolveinfo = "0"
		PszModel.SetTableInfo(tInfo,1)
		ThreadManager.PszUnLock(cgmsg.tableid)
		
		local userIDList =  PszService.GetUserIDList(tInfo, cgmsg.userid) 
		SendMessage(userIDList, PacketCode[3308].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		
		
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString() 
			
	end
	local dissolveinfo = tonumber(tInfo.dissolveinfo)
	dissolveinfo = dissolveinfo + 2*math.pow(10, tInfo.maxuser - tInfo.situser[numIndex].chairid)
	tInfo.dissolveinfo = tostring(dissolveinfo)
	
	gcmsg.dissolveinfo = tInfo.dissolveinfo	
	local dissolveInfo = tInfo.dissolveinfo

	for i = 1,tInfo.maxuser do
		local disTemp = math.mod(dissolveInfo,10)
		dissolveInfo = math.floor(dissolveInfo/10)
		if disTemp == 2 then
			allowNum = allowNum + 1
		end
	end
	
	local userIDList =  PszService.GetUserIDList(tInfo, cgmsg.userid) 
	SendMessage(userIDList, PacketCode[3308].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	local tmpnum = 2 
	if #tInfo.situser == 2 then 
		tmpnum = #tInfo.situser
	elseif #tInfo.situser > 2 then
		tmpnum = #tInfo.situser - 1
	end
	
	if allowNum == tmpnum then
		tInfo.timemark = 0
		tInfo.state = g_PszDefine.state_dissolve
	end
		
	PszModel.SetTableInfo(tInfo,1)
	ThreadManager.PszUnLock(cgmsg.tableid)
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	
end