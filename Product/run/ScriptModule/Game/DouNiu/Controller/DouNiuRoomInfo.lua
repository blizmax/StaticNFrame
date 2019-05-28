module("DouNiuRoomInfo", package.seeall)

--没用到
function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_2_pb.cgdouiuroominfo()
	local gcmsg = msg_douniu_2_pb.gcdouiuroominfo()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.senderid, "douniu", "DouNiuRoomInfo", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogBehavior.Error(cgmsg.userid, "douniu", "DouNiuRoomInfo", ReturnCode["douniu_not_exist"], "牌桌不存在，tableid="..cgmsg.tableid)
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	gcmsg.tableid = cgmsg.tableid
	gcmsg.inninged = tInfo.usevipnum
	gcmsg.inninging = tInfo.maxplaynum - tInfo.usevipnum
	gcmsg.paynum =	math.ceil((tInfo.maxplaynum - tInfo.usevipnum) / 10)
	gcmsg.result = 0
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	


end