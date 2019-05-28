--进入小马快跑


module("DouNiuLeave", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_pb.cgdouniuleave()
	local gcmsg = msg_douniu_pb.gcdouniuleave()
	
	--print(gcmsg.userid)
	--print(gcmsg.tableid)
	--print(gcmsg.chairid)
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuLeave", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = 0
		gcmsg.userid = cgmsg.userid
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	--站立玩家随时能离开
	local isExist = false
	local pos = 0
	local paytype1 = 1
	for k,v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then
			isExist = true
			gcmsg.chairid = v.chairid
			pos = k
			paytype1 = v.payedtype
			break
		end
	end
	

	--坐下的玩家
	if isExist == true then

		if tInfo.state ~= g_douNiuDefine.state_ready then 
			--SpecialModel.MarkAdd(cgmsg.userid)
			--离开的状态是，只有这个才能离开
			gcmsg.result = ReturnCode["douniu_leave_state"]

			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end	

		
		tInfo.usernum = tInfo.usernum - 1 
		tInfo.situser:remove(pos)
		local gcmsgup = msg_douniu_pb.gcdouniustandup()
		
		--更新玩家数量		
		DouNiuEvent.JulebuGameUpdate(tInfo.julebuid, tInfo.tableid, #tInfo.situser, tInfo.julebutype, cgmsg.userid)  
		
		gcmsgup.chairid = gcmsg.chairid
		gcmsgup.result = 0
		local userIDList =  DouNiuService.GetUserIDList(tInfo, cgmsg.userid) 
		SendMessage(userIDList, PacketCode[3016].client, gcmsgup:ByteSize(), gcmsgup:SerializeToString())
	--没坐下的玩家
	else
		for k,v in ipairs(tInfo.standuser) do
			if v == cgmsg.userid then
				tInfo.standuser:remove(k)
				break
			end
		end
	end
	
	if pos == 1 and #tInfo.situser >= 1 then
		local gcCap = msg_douniu_2_pb.gcdouniucaptainchangeinform()
		gcCap.result = 0
		gcCap.captainid = tInfo.situser[1].userid
		local userlist = DouNiuService.GetUserIDList(tInfo)
		SendMessage(userlist, PacketCode[3064].client,gcCap:ByteSize(), gcCap:SerializeToString())
	end
	
	
	DouNiuModel.DelUserTableID(cgmsg.userid)
	
	DouNiuModel.SetTableInfo(tInfo,1)
	
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	--这里如果是房主，需要把对应的ID加入到删除的列表中，同时在线程的循环那边把这个删除
	

	
	
	
	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end