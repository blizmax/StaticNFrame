
module("JulebuEnter", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuenter()
	local gcmsg = msg_julebu_pb.gcjulebuenter()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuEnter", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuEnter", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""		
	end
	
	if false == JulebuModel.IsJoinIn(cgmsg.userid, cgmsg.julebuid) then
		gcmsg.result = ReturnCode["jlb_enter_fail"] 
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	--AgencyModel.ZsmjDivide(cgmsg.userid, 100, "test")
	if nil == JulebuModel.GetJulebuInfo(cgmsg.julebuid, gcmsg.jinfo) then
		gcmsg.jinfo.julebuid = 0
		gcmsg.result = ReturnCode["jlb_enter_fail"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jlbList = JulebuModel.GetUserJulebuList(cgmsg.userid)
	for k,v in ipairs(jlbList) do
		if tonumber(v) ~= cgmsg.julebuid then
			--为什么在这里需要把其他的给清空了，主要是为了防止聊天乱串的，阿仙反馈，另外一个俱乐部的聊天会串到其他俱乐部
			JulebuModel.DelFromOnline(v,cgmsg.userid)
		end
	end
	JulebuService.CheckoutUpdateJuelbuPlayInfo(gcmsg.jinfo)
	JulebuModel.AddOnline(cgmsg.julebuid, cgmsg.userid)
	gcmsg.result = 0
	
	

	local cgTest = msg_julebu2_pb.cggetjulebuawardinfo()
	cgTest.julebuid = cgmsg.julebuid
	cgTest.userid = cgmsg.userid
	
	--local cgtest = msg_julebu2_pb.cggetjulebuawardinfo()
	local packeID, controller = TcpManager.CreateController(3265)
	--controller.execute(3265, operateID, cgTest:SerializeToString())	
	
	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end