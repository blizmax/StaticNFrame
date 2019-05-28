--广东麻将获取历史记录

module("DouNiuHistory", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_pb.cgdouniuhistory()
	local gcmsg = msg_douniu_pb.gcdouniuhistory()
	
	cgmsg:ParseFromString(buffer)
	if operateID ~= 0 then
		local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)

		if checkMsg ~= nil then
			gcmsg:ParseFromString(checkMsg)
			LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuHistory", 0,"缓存已存在")
			return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
	end
	local htyList = DouNiuHistoryModel.GetPlayerHistory(cgmsg.userid, cgmsg.page)  --.GetPlayerHistory(cgmsg.userid, cgmsg.page)

	if htyList ~= nil then
		for k,v in ipairs(htyList) do
			if true == DouNiuHistoryModel.HistoryExist(v) then
				local addItem = gcmsg.htylist:add()
				DouNiuHistoryModel.GetHistroy(v, addItem)
			end
		end
	end

	gcmsg.result = 0
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end
