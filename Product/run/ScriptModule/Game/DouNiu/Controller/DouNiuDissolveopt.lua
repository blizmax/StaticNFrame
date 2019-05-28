module("DouNiuDissolveOpt", package.seeall)

--该操作是玩家同意或者不同意的时候，

function execute(packetID, operateID, buffer)
	local cgmsg = msg_douniu_pb.cgdouniudissolveopt()
	local gcmsg = msg_douniu_pb.gcdouniudissolveopt()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuDissolveOpt", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end	
	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid) 
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogFile("error","DouNiuDissolveOpt table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end	
	
	
	local allowNum = 0   --这里设置为2就行了
	local numIndex = 0
	for k,v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then
			numIndex = k   --取到chairid
			break
		end
	end
	
	if numIndex  == 0 then
		gcmsg.result = ReturnCode["douniu_opt_error"]
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
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
				local num = tonumber(tInfo.dissolveinfo)
				num = num + 3*math.pow(10,#tInfo.situser - tInfo.situser[numIndex].chairid)
				tInfo.dissolveinfo = tostring(num)
				break
			end
		end
		
		gcmsg.dissolveinfo = tInfo.dissolveinfo
		
		tInfo.state = tInfo.tablemark
		tInfo.timemark = tInfo.tabletime
		tInfo.dissolveinfo = "0"
		DouNiuModel.SetTableInfo(tInfo,1)
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		
		local userIDList =  DouNiuService.GetUserIDList(tInfo, cgmsg.userid) 
		SendMessage(userIDList, PacketCode[3008].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		
	
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString() 
	
	end

	--print("tInfo.dissolveinfo2222 = "..tInfo.dissolveinfo)
	--print("numIndex = "..numIndex)
	--print("tInfo.situser[numIndex].chairid = "..tInfo.situser[numIndex].chairid)
	--tInfo.dissolveinfo = tInfo.dissolveinfo + 2*math.pow(10, #tInfo.situser - numIndex)
	local num = tonumber(tInfo.dissolveinfo)
	num = num + 2*math.pow(10, tInfo.usermaxnum - tInfo.situser[numIndex].chairid)
	tInfo.dissolveinfo = tostring(num)
	
	gcmsg.dissolveinfo = tInfo.dissolveinfo	
	local dissolveInfo = tonumber(tInfo.dissolveinfo)
	--print("dissolveInfo = "..dissolveInfo)
	--print("#tInfo.situser = "..#tInfo.situser)
	for i = 1,tInfo.usermaxnum do
		local disTemp = math.mod(dissolveInfo,10)
		--print("disTemp = "..disTemp)
		dissolveInfo = math.floor(dissolveInfo/10)
		if disTemp == 2 then
			allowNum = allowNum + 1
		end
	end
	
	local userIDList =  DouNiuService.GetUserIDList(tInfo, cgmsg.userid) 
	SendMessage(userIDList, PacketCode[3008].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	if allowNum == #tInfo.situser - 1 then
		tInfo.timemark = 0
		tInfo.state = g_douNiuDefine.state_dissolve
	end
		
	DouNiuModel.SetTableInfo(tInfo,1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	
end