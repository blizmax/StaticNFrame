
module("InviteBind", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cginvitebind()
	local gcmsg = msg_human_pb.gcinvitebind()
	cgmsg:ParseFromString(buffer)

	if cgmsg.userid == tonumber(cgmsg.invitestr) then
		--不能过绑定自己
		gcmsg.result = ReturnCode["bind_not_self"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	if cgmsg.invitestr == nil or 0 == string.len(cgmsg.invitestr) then
		gcmsg.result = ReturnCode["agency_bind_error"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	AgencyModel.GetAgencyInfo(cgmsg.userid,gcmsg.agyinfo)
	if gcmsg.agyinfo.agent1 ~= 0 then
		--表示已经绑定过代理了
		gcmsg.result = ReturnCode["agency_is_band"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local toAgency = AgencyModel.GetAgencyInfo(tonumber(cgmsg.invitestr))
	

	if toAgency == nil then
		--说明被绑定的玩家还没有绑定别人。
		--gcmsg.result = 
		gcmsg.agyinfo.userid = 0
		gcmsg.result = ReturnCode["agency_bind_error"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if toAgency.level < 91 then
		gcmsg.result = ReturnCode["agency_bind_fail"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	--一旦绑定别人了，就会在后台插入一个记录
	local sqlCase = ""
	
	if toAgency.level == 0 then   --说明这个是玩家，绑定的代理跟该玩家一样的
		
		if gcmsg.agyinfo.userid == 0 then
		
			gcmsg.agyinfo.userid = cgmsg.userid
			gcmsg.agyinfo.invitestr = tostring(cgmsg.userid)
			gcmsg.agyinfo.level = 0
			gcmsg.agyinfo.soleagent = toAgency.level >= 99 and toAgency.userid or toAgency.soleagent
			gcmsg.agyinfo.agent1 = toAgency.agent1
			gcmsg.agyinfo.agent2 = toAgency.agent2
			gcmsg.agyinfo.agent3 = toAgency.agent3
			gcmsg.agyinfo.agent4 = toAgency.agent4
			gcmsg.agyinfo.qrcodeurl = AgencyModel.GetQrcodeUrl(cgmsg.userid)
			AgencyModel.InsertAgency(pInfo, gcmsg.agyinfo)
		else
			gcmsg.agyinfo.soleagent = toAgency.level >= 99 and toAgency.userid or toAgency.soleagent
			gcmsg.agyinfo.agent1 = toAgency.agent1
			gcmsg.agyinfo.agent2 = toAgency.agent2
			gcmsg.agyinfo.agent3 = toAgency.agent3
			gcmsg.agyinfo.agent4 = toAgency.agent4
			sqlCase = "update ag_player set soleagent="..gcmsg.agyinfo.soleagent..",agent1="..gcmsg.agyinfo.agent1..",agent2"
			..gcmsg.agyinfo.agent2..",agent3="..gcmsg.agyinfo.agent3..",agent4="..gcmsg.agyinfo.agent4.." where userid="..cgmsg.userid			
			mysqlItem:execute(sqlCase)   --开始设定
		end

		
		AgencyModel.SetAgency(cgmsg.userid, gcmsg.agyinfo)
		
	else
		if gcmsg.agyinfo.userid == 0 then
			gcmsg.agyinfo.userid = cgmsg.userid
			gcmsg.agyinfo.invitestr = tostring(cgmsg.userid)
			gcmsg.agyinfo.level = 0
			gcmsg.agyinfo.soleagent = toAgency.level >= 99 and toAgency.userid or toAgency.soleagent
			gcmsg.agyinfo.agent1 = toAgency.userid       --属于他自己的玩家
			gcmsg.agyinfo.agent2 = toAgency.agent1       --自己的玩家的上一级
			gcmsg.agyinfo.agent3 = toAgency.agent2    
			gcmsg.agyinfo.agent4 = toAgency.agent3
			gcmsg.agyinfo.qrcodeurl = AgencyModel.GetQrcodeUrl(cgmsg.userid)
			AgencyModel.InsertAgency(pInfo, gcmsg.agyinfo)
		else
			gcmsg.agyinfo.soleagent = toAgency.level >= 99 and toAgency.userid or toAgency.soleagent
			gcmsg.agyinfo.agent1 = toAgency.userid
			gcmsg.agyinfo.agent2 = toAgency.agent1
			gcmsg.agyinfo.agent3 = toAgency.agent2
			gcmsg.agyinfo.agent4 = toAgency.agent3
			sqlCase = "update ag_player set soleagent="..gcmsg.agyinfo.soleagent..",agent1="..gcmsg.agyinfo.agent1..",agent2"
			..gcmsg.agyinfo.agent2..",agent3="..gcmsg.agyinfo.agent3..",agent4="..gcmsg.agyinfo.agent4.." where userid="..cgmsg.userid				
			mysqlItem:execute(sqlCase)   --开始设定
		end
		
		AgencyModel.SetAgency(cgmsg.userid, gcmsg.agyinfo)		
	end


	--这里开始赠送了
	if g_servername == "run_zsmj" or g_servername == "run_xwpk" then
		--如果是掌上麻将，送的是掌上币，对应的是gold
		--首次绑定的玩家送150，第二次绑定的玩家送60
		PlayerModel.AddGold(pInfo, 150, "agency", "绑定赠送")
		local aPinfo = PlayerModel.GetPlayerInfo(cgmsg.invitestr)
		PlayerModel.AddGold(aPinfo, 60, "agency", "邀请绑定赠送")
		PlayerModel.SendPlayerInfo(pInfo,{"gold"})
		PlayerModel.SendPlayerInfo(aPinfo,{"gold"})
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SetPlayerInfo(aPinfo)
	elseif g_servername == "run_hubmj" then
		
		PlayerModel.AddMoney(pInfo, 15, "agency", "绑定赠送")
		local aPinfo = PlayerModel.GetPlayerInfo(cgmsg.invitestr)
		PlayerModel.AddMoney(aPinfo, 6, "agency", "邀请绑定赠送")
		PlayerModel.SendPlayerInfo(pInfo,{"money"})
		PlayerModel.SendPlayerInfo(aPinfo,{"money"})
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SetPlayerInfo(aPinfo)
	elseif g_servername == "run_gdmj" then
		PlayerModel.AddMoney(pInfo, 16, "agency", "绑定赠送")
		local aPinfo = PlayerModel.GetPlayerInfo(cgmsg.invitestr)
		PlayerModel.AddMoney(aPinfo, 8, "agency", "邀请绑定赠送")
		PlayerModel.SendPlayerInfo(pInfo,{"money"})
		PlayerModel.SendPlayerInfo(aPinfo,{"money"})
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SetPlayerInfo(aPinfo)		
	end

	
	local addItem = toAgency.invitelist:add()
	addItem.userid = tonumber(cgmsg.invitestr)
	addItem.binduserid = cgmsg.userid
	addItem.bindnickname = pInfo.nickname
	addItem.bindfaceid = pInfo.face_1
	addItem.binddate = TimeUtils.GetTimeString()
	toAgency.userid = tonumber(cgmsg.invitestr)
	AgencyModel.SetAgency(cgmsg.invitestr, toAgency)	
	sqlCase = "insert into dy_invite(userid,touserid,bindstate,binddate) values("..cgmsg.userid..","..cgmsg.invitestr..",0,'"..TimeUtils.GetTimeString().."'"..")"
	mysqlItem:execute(sqlCase)
	--OK，绑定完了，轮到下一步了
	--这里暂时不考虑送钱
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end