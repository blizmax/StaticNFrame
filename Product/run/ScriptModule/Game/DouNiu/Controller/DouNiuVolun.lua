module("DouNiuVolun", package.seeall)

--这是设置自动亮牌的
--设置了自动亮牌后2秒自动完成看牌（针对玩家）



function execute(packetID, operateID, buffer)
	local cgmsg = msg_douniu_2_pb.cgdouiuvolun()
	local gcmsg = msg_douniu_2_pb.gcdouiuvolun()
	
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
		gcmsg.result = ReturnCode["douniu_not_exist"]
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local is = false
	for i = 1 , #tInfo.situser do
		if cgmsg.userid == tInfo.situser[i].userid then
			tInfo.situser[i].volun = cgmsg.volun
			is = true
		end
	end
	
	if is == false then 
		gcmsg.result = ReturnCode["douniu_opt_error"]
		ThreadManager.DouNiuUnLock(cgmsg.tableid)	
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)	
	
	--print("cgmsg.volun = "..cgmsg.volun)
	gcmsg.userid = cgmsg.userid
	gcmsg.volun = cgmsg.volun
	gcmsg.result = 0
	--local userIDList =  DouNiuService.GetUserIDList( tInfo, cgmsg.userid )
	--SendMessage(userIDList, PacketCode[3048].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end

