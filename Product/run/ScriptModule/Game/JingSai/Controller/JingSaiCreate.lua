
module("JingSaiCreate", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaicreate()
	local gcmsg = msg_jingsai_pb.gcjingsaicreate()
	
	cgmsg:ParseFromString(buffer)

	--创建竞赛
	--在目前的情况下，创建的竞赛是立刻开始的 
	--预定开始的时间
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "jingsai", "JingSaiCreate", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	
	local jlbInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jlbInfo == nil then
		gcmsg.result = ReturnCode["jingsai_error_jlb"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	if cgmsg.name == nil or cgmsg.name == '' then
		gcmsg.result = ReturnCode["jlb_name_nil"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
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
	
	local currTime = TimeUtils.GetTime()    --获取当前的时间
	gcmsg.jsinfo.jingsaiid = JingSaiModel.CreateJingSaiID()
	gcmsg.jsinfo.julebuid = cgmsg.julebuid
	gcmsg.jsinfo.name = cgmsg.name
	--gcmsg.jsinfo.maxmembernum = 999
	--gcmsg.jsinfo.membernum = 999
	gcmsg.jsinfo.des = cgmsg.des
	gcmsg.jsinfo.ownerid = cgmsg.userid
	gcmsg.jsinfo.moduletype = cgmsg.moduletype
	gcmsg.jsinfo.strcreate = cgmsg.strcreate

	gcmsg.jsinfo.starttime = cgmsg.starttime
	gcmsg.jsinfo.longtime = cgmsg.longtime
	gcmsg.jsinfo.state = currTime >= cgmsg.starttime and g_jingSaiDefine.state_start or g_jingSaiDefine.state_ready

	gcmsg.jsinfo.lasttime = cgmsg.longtime
	
	
	gcmsg.jsinfo.rule = cgmsg.rule
	gcmsg.jsinfo.payway = cgmsg.payway == 0 and g_gamePayWay.pay_AA or cgmsg.payway
	gcmsg.jsinfo.paytype = cgmsg.paytype
	gcmsg.jsinfo.paynum = cgmsg.paynum
	JingSaiModel.AddJingSai(gcmsg.jsinfo)
	gcmsg.result = 0
	local sendList = JulebuModel.GetOnlineList(cgmsg.julebuid)
	SendMessage(sendList, PacketCode[3404].client, gcmsg:ByteSize(), gcmsg:SerializeToString())   --同时广播给其他人
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end