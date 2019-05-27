
module("JulebuInvite", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebuinvite()
	local gcmsg = msg_julebu2_pb.gcjulebuinvite()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuInvite", 0, "重复申请，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_enter_fail"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuInvite", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""	
	end
	
	if true ~= JulebuModel.IsJoinIn(cgmsg.userid, cgmsg.julebuid) then
		gcmsg.result = ReturnCode["jlb_Invite_error3"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	local InviteInfo = PlayerModel.GetPlayerInfo(cgmsg.inviteuserid)
	if InviteInfo == nil then
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if InviteInfo.caninvite and InviteInfo.caninvite == 1 then
		--玩家设置了不接受邀请
		gcmsg.result = ReturnCode["jlb_Invite_error5"]
		--print(gcmsg.result)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if true == JulebuModel.IsJoinIn(cgmsg.inviteuserid, cgmsg.julebuid) then
		gcmsg.result = ReturnCode["jlb_Invite_error2"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local memberID = 0
	local sqlCase = "select id,status from dy_julebu_member where userid="..cgmsg.inviteuserid.." and julebuid="..cgmsg.julebuid
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData ~= nil then
		if g_JulebuDefine.mstatus_normal == tonumber(sqlData[2]) then
			gcmsg.result = ReturnCode["jlb_Invite_error2"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		else
			sqlCase = "update dy_julebu_member set status="..g_JulebuDefine.mstatus_normal.." where userid="..cgmsg.inviteuserid.." and julebuid="..cgmsg.julebuid
			mysqlItem:execute(sqlCase)
		end
		memberID = sqlData[1]
		JulebuModel.DelFromApplyList(cgmsg.julebuid, tonumber(sqlData[1]))
	else
		local sqlCase="insert into dy_julebu_member(userid,julebuid,status) values("..cgmsg.inviteuserid..","..cgmsg.julebuid..","..g_JulebuDefine.mstatus_normal..")"
		mysqlItem:execute(sqlCase)
		local sqlCase = "select id from dy_julebu_member where userid="..cgmsg.inviteuserid.." and julebuid="..cgmsg.julebuid
		mysqlItem:executeQuery(sqlCase)
		sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			gcmsg.result = ReturnCode["jlb_Invite_error4"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		memberID = tonumber(sqlData)
	end
	
	local aItem = st_julebu_pb.julebumember()
	aItem.userid = cgmsg.inviteuserid
	aItem.nickname = InviteInfo.nickname
	aItem.faceid = InviteInfo.face_1
	JulebuModel.AddToMemberList(cgmsg.julebuid, aItem)
	JulebuModel.SetUserJulebuList(cgmsg.inviteuserid, cgmsg.julebuid)
	
	
	jInfo.membernum = jInfo.membernum + 1
	JulebuModel.SetJulebuInfo(jInfo)
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end