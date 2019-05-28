module("PszEndShow", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_2_pb.cgpszendshow()
	local gcmsg = msg_psz_2_pb.gcpszendshow()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszEndShow", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	ThreadManager.PszLock(cgmsg.tableid)
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		gcmsg.result = ReturnCode["psz_not_exist"]
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	--当前牌局不结束不能亮牌
	if tInfo.state ~= g_PszDefine.state_count and tInfo.state ~= g_PszDefine.state_next then
		gcmsg.result = ReturnCode["psz_endshow_no"]
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].userid == cgmsg.userid then
			gcmsg.chairid = tInfo.situser[i].chairid
			tInfo.situser[i].endshow = 1
		end
	end
	
	gcmsg.result = 0;
	gcmsg.userid = cgmsg.userid;
	
	PszModel.SetTableInfo(tInfo, 1)
	ThreadManager.PszUnLock(cgmsg.tableid)

	local userIDList = PszService.GetUserIDList(tInfo,cgmsg.userid)
	SendMessage(userIDList, PacketCode[3340].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end

