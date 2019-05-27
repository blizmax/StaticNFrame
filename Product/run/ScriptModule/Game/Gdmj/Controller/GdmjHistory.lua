--广东麻将获取历史记录

module("GdmjHistory", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_gdmj_pb.cggdmjhistory()
	local gcmsg = msg_gdmj_pb.gcgdmjhistory()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjHistory", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end

	local htyList = GdmjHistoryModel.GetPlayerHistory(cgmsg.userid, cgmsg.page)-- .GetPlayerHistory(cgmsg.userid, cgmsg.page)

	if htyList ~= nil then
		for k,v in ipairs(htyList) do
			if true == GdmjHistoryModel.HistoryExist(v) then
				local addItem = gcmsg.htylist:add()
				GdmjHistoryModel.GetHistroy(v, addItem)
			end
		end
	end

	gcmsg.result = 0
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end