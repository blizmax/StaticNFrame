
module("JulebuChatList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuchatlist()
	local gcmsg = msg_julebu_pb.gcjulebuchatlist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuChatList", 0, "重复申请，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	cgmsg.startchatid = cgmsg.startchatid == 0 and tonumber(JulebuModel.GetChatID(cgmsg.julebuid)) or cgmsg.startchatid
	local getList = JulebuModel.GetChatList(cgmsg.julebuid, cgmsg.startchatid)
	for k,v in pairs(getList) do
		local addItem = gcmsg.chatlist:add()
		addItem:ParseFromString(v)
	end
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end