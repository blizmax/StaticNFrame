module("GdmjHistoryStep", package.seeall)


function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_gdmj_pb.cggdmjhistorystep()
	local gcmsg = msg_gdmj_pb.gcgdmjhistorystep()
	
	cgmsg:ParseFromString(buffer)
	
	--local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	--if checkMsg ~= nil then
	--	gcmsg:ParseFromString(checkMsg)
	--  LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjHistoryStep", 0,"缓存已存在")
	--	return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	--end
	
	gcmsg.steplen = GdmjHistoryModel.GetActionLen(cgmsg.tablepalyid)
		
	if gcmsg.steplen >= cgmsg.stepid then
		GdmjHistoryModel.GetAction(cgmsg.tablepalyid, cgmsg.stepid, gcmsg.htypair)
	end
	
	gcmsg.result = 0
	return 0, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	
end