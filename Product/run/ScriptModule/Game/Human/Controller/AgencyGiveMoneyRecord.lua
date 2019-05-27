module("AgencyGiveMoneyRecord", package.seeall)

-- 获取代理赠送钻石记录
-- wyh 2018-3-13 21:09:44

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cggivemoneyrecord()
	local gcmsg = msg_human_pb.gcgivemoneyrecord()
	cgmsg:ParseFromString(buffer)
	
	repeat
		-- 检查权限
		local agency = AgencyModel.GetAgencyInfo(cgmsg.userid, gcmsg.agyinfo)
		if nil == agency then
			gcmsg.result = ReturnCode["not_agency"]
			break
		end
		
		local list = AgencyModel.GetGiveMoneyRecordList(cgmsg.userid, cgmsg.page)
		for _, v in ipairs(list) do
			local item = gcmsg.list:add()
			item:ParseFromString(v)
		end
		gcmsg.result = 0
	until true
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end