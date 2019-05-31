
module("JingSaiSetDes", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaisetdes()
	local gcmsg = msg_jingsai_pb.gcjingsaisetdes()
	
	cgmsg:ParseFromString(buffer)
	

	
	local jsInfo = JingSaiModel.GetJingSaiInfo(cgmsg.jingsaiid)
	
	if jsInfo == nil then
		gcmsg.result = ReturnCode["jingsai_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jlbInfo = JulebuModel.GetJulebuInfo(jsInfo.julebuid)
	if jlbInfo == nil then
		gcmsg.result = ReturnCode["jingsai_del_fail"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	local isAdmin = false
		for k,v in ipairs(jlbInfo.adminlist) do
		if v == cgmsg.userid then
			isAdmin = true
			break
		end
	end
	if isAdmin == false then
		--只有管理员才能获取全部人的历史记录
		gcmsg.result = ReturnCode["jingsai_error_admin"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end		
	

	
	jsInfo.des = cgmsg.des
	JingSaiModel.SetJingSaiInfo(jsInfo)
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end