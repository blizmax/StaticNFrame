
module("JingSaiDel", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaidel()
	local gcmsg = msg_jingsai_pb.gcjingsaidel()
	
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
	
	if jsInfo.state ~= g_jingSaiDefine.state_ready then
		
		gcmsg.result = ReturnCode["jingsai_del_state"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	JingSaiModel.DelJingSai(jsInfo.jingsaiid, jsInfo.julebuid)
	gcmsg.jingsaiid = cgmsg.jingsaiid
	local sendList = JulebuModel.GetOnlineList(jsInfo.julebuid) -- 同时广播给其他人
	SendMessage(sendList, PacketCode[3406].client, gcmsg:ByteSize(), gcmsg:SerializeToString())   --同时广播给其他人
		
	gcmsg.result = 0
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end