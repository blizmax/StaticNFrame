
module("JingSaiHistory", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaihistory()
	local gcmsg = msg_jingsai_pb.gcjingsaihistory()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "jingsai", "JingSaiHistory", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if false == JulebuModel.IsJoinIn(cgmsg.userid, cgmsg.julebuid) then
		gcmsg.result = ReturnCode["jlb_not_member"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	local getList = JingSaiModel.GetHistoryList(cgmsg.julebuid)   --取到了历史记录
	for k,v in ipairs(getList) do
		local addItem = gcmsg.htylist:add()
		addItem:ParseFromString(v)
	end
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end