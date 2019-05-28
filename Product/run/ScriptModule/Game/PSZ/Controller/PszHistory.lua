--拼三张获取历史记录

module("PszHistory", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_pb.cgpszhistory()
	local gcmsg = msg_psz_pb.gcpszhistory()
	
	cgmsg:ParseFromString(buffer)
	if operateID ~= 0 then
		local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)

		if checkMsg ~= nil then
			gcmsg:ParseFromString(checkMsg)
			LogBehavior.Warning(cgmsg.userid, "psz", "PszHistory", 0,"缓存已存在")
			return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
	end
	local htyList = PszModel.GetPlayerHistory(cgmsg.userid, cgmsg.page)  --.GetPlayerHistory(cgmsg.userid, cgmsg.page)

	if htyList ~= nil then
		for k,v in ipairs(htyList) do
			local addItem = gcmsg.htylist:add()
			addItem:ParseFromString(v)
		end
	end
	gcmsg.result = 0
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end
