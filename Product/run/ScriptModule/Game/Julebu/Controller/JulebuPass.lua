
module("JulebuPass", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebupass()
	local gcmsg = msg_julebu_pb.gcjulebupass()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuPass", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuPass", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""	
	end

	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid, gcmsg.jinfo)
	if jInfo == nil then
		gcmsg.jinfo.julebuid = 0
		gcmsg.result = ReturnCode["jlb_enter_fail"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	local sqlCase = "select id from dy_julebu_member where userid="..cgmsg.applyid.." and julebuid="..cgmsg.julebuid
	
	
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch()
	if sqlData == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	gcmsg.result = 0
	gcmsg.julebuid = cgmsg.julebuid
	gcmsg.applyid = cgmsg.applyid
	gcmsg.opttype = cgmsg.opttype
	if cgmsg.opttype == 2 then
		--不同意
		sqlCase = "update dy_julebu_member set status="..g_JulebuDefine.mstatus_notpass.." where userid="..cgmsg.applyid.." and julebuid="..cgmsg.julebuid
		mysqlItem:execute(sqlCase)
	else
		sqlCase = "update dy_julebu_member set status="..g_JulebuDefine.mstatus_normal.." where userid="..cgmsg.applyid.." and julebuid="..cgmsg.julebuid
		mysqlItem:execute(sqlCase)
		local aItem = st_julebu_pb.julebumember()
		local pApply = PlayerModel.GetPlayerInfo(cgmsg.applyid)
		aItem.userid = cgmsg.applyid
		aItem.nickname = pApply.nickname
		aItem.faceid = pApply.face_1
		aItem.id = tonumber(sqlData)
		
		JulebuModel.AddToMemberList(cgmsg.julebuid, aItem)
		
		jInfo.membernum = jInfo.membernum + 1
		sqlCase = "update dy_julebu set membernum="..jInfo.membernum.." where id="..cgmsg.julebuid
		mysqlItem:execute(sqlCase)
		JulebuModel.SetJulebuInfo(jInfo)
		JulebuModel.SetUserJulebuList(cgmsg.applyid, cgmsg.julebuid)
		SendMessage(cgmsg.applyid, PacketCode[3216].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	end
	JulebuModel.DelFromApplyList(cgmsg.julebuid, sqlData)
	
	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end