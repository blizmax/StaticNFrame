module("JulebuAddNextPartner", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebuaddnextplayer()
	local gcmsg = msg_julebu2_pb.gcjulebuaddnextplayer()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuApply", 0, "重复申请，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuApply", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	
	if false == JulebuModel.IsJoinIn(cgmsg.userid, cgmsg.julebuid) then
		gcmsg.result = ReturnCode["jlb_apply_joinin"]
		return cgmsg.userid, 0, gcmsg:SerializeToString(), gcmsg:ByteSize()
	end

	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--查看partnerid 是不是合伙人
	local isPartner = false
	local istargetPartner = false
	for i = 1, #jInfo.partnerlist do 
		if jInfo.partnerlist[i] == cgmsg.partnerid then
			isPartner = true
		end
		if jInfo.partnerlist[i] == cgmsg.targetid then
			istargetPartner = true
		end

	end
	
	if isPartner == false then
		gcmsg.result = ReturnCode["jlb_partner_error2"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
 
	if cgmsg.requesttype == 1 then
		
		--查看 targetid 是不是该俱乐部的玩家
		if false == JulebuModel.IsJoinIn(cgmsg.targetid, cgmsg.julebuid) then
			gcmsg.result = ReturnCode["jlb_partner_error4"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		--管理员不能成为下线
		for i = 1, #jInfo.adminlist do 
			if jInfo.adminlist[i] == cgmsg.targetid then
				gcmsg.result = ReturnCode["jlb_partner_error3"]
				return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
		end
		--合伙人不能成为下线
		if istargetPartner == true then
			gcmsg.result = ReturnCode["jlb_partner_error3"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		--已经是下线的不能再成为下线
		local tmpPartnerid = JulebuService.checkoutsuperiorid(cgmsg.julebuid, cgmsg.targetid)
		if 0 ~= tmpPartnerid then
			local istmpPartner = false 
			for i = 1, #jInfo.partnerlist do 
				if jInfo.partnerlist[i] == tmpPartnerid then
					istmpPartner = true
				end
			end
			if istmpPartner == true then
				gcmsg.result = ReturnCode["jlb_partner_error1"]
				return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
		end
		local tmpresult_1 = addNextPartner(cgmsg) 
		if tmpresult_1 ~= 0 then
			gcmsg.result = tmpresult_1
			return cgmsg.userid, 0, gcmsg:SerializeToString(), gcmsg:ByteSize()
		end
		
	elseif cgmsg.requesttype == 2 then 
		--查看targetid是不是partnerid 的下线
		if cgmsg.partnerid ~= JulebuService.checkoutsuperiorid(cgmsg.julebuid, cgmsg.targetid) then
			gcmsg.result = ReturnCode["jlb_partner_error2"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		local sqlCase = "update dy_julebu_member set superiorid=0 where userid="..cgmsg.targetid.." and julebuid="..cgmsg.julebuid
		mysqlItem:executeQuery(sqlCase)
		JulebuModel.DelPartnerInfo(cgmsg.julebuid, cgmsg.targetid)
	elseif cgmsg.requesttype == 3 then
		--查看 targetid 是不是该俱乐部的玩家
		if true == JulebuModel.IsJoinIn(cgmsg.targetid, cgmsg.julebuid) then
			gcmsg.result = ReturnCode["jlb_Invite_error2"]
			return cgmsg.userid, 0, gcmsg:SerializeToString(), gcmsg:ByteSize()
		end
		
		local tmpresult = addJulebuPlayer(cgmsg.targetid, cgmsg.julebuid, jInfo)
		if tmpresult ~= 0 then
			gcmsg.result = tmpresult
			return cgmsg.userid, 0, gcmsg:SerializeToString(), gcmsg:ByteSize()
		end
		local tmpresult_1 = addNextPartner(cgmsg) 
		if tmpresult_1 ~= 0 then
			gcmsg.result = tmpresult_1
			return cgmsg.userid, 0, gcmsg:SerializeToString(), gcmsg:ByteSize()
		end
	end
	gcmsg.julebuid = cgmsg.julebuid
	gcmsg.requesttype = cgmsg.requesttype
		
	local sqlCase = "select userid from dy_julebu_member  where superiorid="..cgmsg.partnerid.." and julebuid="..cgmsg.julebuid
	mysqlItem:executeQuery(sqlCase)
	local count = 0
	while true do
		local sqlData = mysqlItem:fetch()
		if sqlData == nil or tonumber(sqlData) == 0  then
			break
		end
		local tmp = JulebuModel.GetPartnerInfo(cgmsg.julebuid, tonumber(sqlData))
		local PartnerInfo = gcmsg.partnerlist:add()
		PartnerInfo:ParseFromString(tmp)
		if count > 9 then
			break
		end
		count = count + 1
	end

	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
	
end


function addNextPartner(cgmsg)
	local PartnerInfo = st_julebu_pb.julebupartnerinfo()
	local tpInfo = PlayerModel.GetPlayerInfo(cgmsg.targetid)
	if tpInfo == nil then
		return ReturnCode["player_not_exist"]
	end
	PartnerInfo.userid = tpInfo.userid
	PartnerInfo.nickname = tpInfo.nickname
	PartnerInfo.yesterdayscore = 0
	PartnerInfo.yesterdaydayingjia = 0
	PartnerInfo.yesterdaygamenum = 0
	PartnerInfo.yesterdayconsume = 0
	PartnerInfo.gamenum = 0
	PartnerInfo.todayscore = 0
	PartnerInfo.todaydayingjia = 0
	PartnerInfo.todaygamenum = 0
	PartnerInfo.todayconsume = 0
	JulebuModel.SetPartnerInfo(cgmsg.julebuid, cgmsg.targetid, PartnerInfo)
		
	local sqlCase = "update dy_julebu_member set superiorid="..cgmsg.partnerid.." where userid="..cgmsg.targetid.." and julebuid="..cgmsg.julebuid
	mysqlItem:executeQuery(sqlCase)
	
	return 0
end

function addJulebuPlayer(inviteuserid, julebuid,jInfo)
	
	local InviteInfo = PlayerModel.GetPlayerInfo(inviteuserid)
	if InviteInfo == nil then
		return ReturnCode["player_not_exist"]
	end
	
	local memberID = 0
	local sqlCase = "select id,status from dy_julebu_member where userid="..inviteuserid.." and julebuid="..julebuid
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData ~= nil then
		if g_JulebuDefine.mstatus_normal == tonumber(sqlData[2]) then
			return ReturnCode["jlb_Invite_error2"]
		else
			sqlCase = "update dy_julebu_member set status="..g_JulebuDefine.mstatus_normal.." where userid="..inviteuserid.." and julebuid="..julebuid
			mysqlItem:execute(sqlCase)
		end
		memberID = sqlData[1]
		JulebuModel.DelFromApplyList(julebuid, tonumber(sqlData[1]))
	else
		local sqlCase="insert into dy_julebu_member(userid,julebuid,status) values("..inviteuserid..","..julebuid..","..g_JulebuDefine.mstatus_normal..")"
		mysqlItem:execute(sqlCase)
		local sqlCase = "select id from dy_julebu_member where userid="..inviteuserid.." and julebuid="..julebuid
		mysqlItem:executeQuery(sqlCase)
		sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			return ReturnCode["jlb_Invite_error4"]
		end
		memberID = tonumber(sqlData)
	end
	
	local aItem = st_julebu_pb.julebumember()
	aItem.userid = inviteuserid
	aItem.nickname = InviteInfo.nickname
	aItem.faceid = InviteInfo.face_1
	JulebuModel.AddToMemberList(julebuid, aItem)
	JulebuModel.SetUserJulebuList(inviteuserid, julebuid)
	
	
	jInfo.membernum = jInfo.membernum + 1
	JulebuModel.SetJulebuInfo(jInfo)
	return 0
end