module("AgencyOpt", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgagencyopt()
	local gcmsg = msg_human_pb.gcagencyopt()
	
	cgmsg:ParseFromString(buffer)
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"gdmj","GdmjCreate", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	if cgmsg.opttype == 1 then
		--绑定代理
		if cgmsg.userid == tonumber(cgmsg.optstr) then
			--不能过绑定自己
			gcmsg.result = ReturnCode["bind_not_self"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
	
		local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
		if pInfo == nil then
			gcmsg.result = ReturnCode["player_not_exist"]
			LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, cgmsg.userid, "player_not_exist")
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
		end
		
		if cgmsg.optstr == nil or 0 == string.len(cgmsg.optstr) then
			LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, cgmsg.userid, "cgmsg.optstr is error")
			gcmsg.result = ReturnCode["agency_bind_error"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
		end
		
		local myAgency = AgencyModel.GetAgencyInfo(cgmsg.userid)
		if myAgency ~= nil and myAgency.agent1 ~= 0 then
			--说明还没有绑定代理
			LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, cgmsg.userid, "agency_is_band")
			gcmsg.result = ReturnCode["agency_is_band"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()				
		else
			myAgency = st_human_pb.agencyinfo()
		end
	
		
		
		local toAgency = AgencyModel.GetAgencyInfo(tonumber(cgmsg.optstr))
		
		
		if toAgency == nil then
			--说明被绑定的玩家还没有绑定别人。
			--gcmsg.result = 
			gcmsg.result = ReturnCode["agency_bind_error"]
			LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, cgmsg.userid, "agency_bind_error optstr="..cgmsg.optstr)
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		--一旦绑定别人了，就会在后台插入一个记录
		local sqlCase = ""
		
		if toAgency.level == 0 then   --说明这个是玩家，绑定的代理跟该玩家一样的
			if myAgency.userid == 0 then
				myAgency.userid = cgmsg.userid
				myAgency.invitestr = tostring(cgmsg.userid)
				myAgency.level = 0
				myAgency.soleagent = toAgency.soleagent
				myAgency.agent1 = toAgency.agent1
				myAgency.agent2 = toAgency.agent2
				myAgency.agent3 = toAgency.agent3
				myAgency.agent4 = toAgency.agent4
				
				sqlCase = "insert into ag_player(userid,nickname,account,state,level,soleagent,agent1,agent2,agent3,agent4) values("
				..cgmsg.userid..",'"..pInfo.nickname.."','"..tonumber(cgmsg.userid).."',"..myAgency.level..",0,"..myAgency.soleagent..","
				..myAgency.agent1..","..myAgency.agent2..","..myAgency.agent3..","..myAgency.agent4..")" 
			else
				myAgency.soleagent = toAgency.soleagent
				myAgency.agent1 = toAgency.agent1
				myAgency.agent2 = toAgency.agent2
				myAgency.agent3 = toAgency.agent3
				myAgency.agent4 = toAgency.agent4	
				sqlCase = "update ag_player set soleagent="..myAgency.soleagent..",agent1="..myAgency.agent1..",agent2"
				..myAgency.agent2..",agent3="..myAgency.agent3..",agent4="..myAgency.agent4.." where userid="..cgmsg.userid
			end
			mysqlItem:execute(sqlCase)   --开始设定
			AgencyModel.SetAgency(cgmsg.userid, myAgency)			
		else
			if myAgency.userid == 0 then
				myAgency.userid = cgmsg.userid
				myAgency.invitestr = tostring(cgmsg.userid)
				myAgency.level = 0
				myAgency.soleagent = toAgency.soleagent   --总代不变
				myAgency.agent1 = toAgency.userid       --属于他自己的玩家
				myAgency.agent2 = toAgency.agent1       --自己的玩家的上一级
				myAgency.agent3 = toAgency.agent2    
				myAgency.agent4 = toAgency.agent3
				
				sqlCase = "insert into ag_player(userid,nickname,account,state,level,soleagent,agent1,agent2,agent3,agent4) values("
				..cgmsg.userid..",'"..pInfo.nickname.."','"..tonumber(cgmsg.userid).."',"..myAgency.level..",0,"..myAgency.soleagent..","
				..myAgency.agent1..","..myAgency.agent2..","..myAgency.agent3..","..myAgency.agent4..")" 
			else
				myAgency.soleagent = toAgency.soleagent
				myAgency.agent1 = toAgency.userid
				myAgency.agent2 = toAgency.agent1
				myAgency.agent3 = toAgency.agent2
				myAgency.agent4 = toAgency.agent3	
				sqlCase = "update ag_player set soleagent="..myAgency.soleagent..",agent1="..myAgency.agent1..",agent2"
				..myAgency.agent2..",agent3="..myAgency.agent3..",agent4="..myAgency.agent4.." where userid="..cgmsg.userid				
			end
			mysqlItem:execute(sqlCase)   --开始设定
			AgencyModel.SetAgency(cgmsg.userid, myAgency)		
		end
		
		local addItem = toAgency.invitelist:add()
		addItem.userid = tonumber(cgmsg.optstr)
		addItem.binduserid = cgmsg.userid
		addItem.bindnickname = pInfo.nickname
		addItem.bindfaceid = pInfo.face_1
		addItem.binddate = TimeUtils.GetTimeString()
		toAgency.userid = tonumber(cgmsg.optstr)
		AgencyModel.SetAgency(cgmsg.optstr, toAgency)	
		sqlCase = "insert into dy_invite(userid,touserid,bindstate,binddate) values("..cgmsg.userid..","..cgmsg.optstr..",0,'"..TimeUtils.GetTimeString().."'"..")"
		mysqlItem:execute(sqlCase)
		--OK，绑定完了，轮到下一步了
		--这里暂时不考虑送钱
		gcmsg.str = "绑定成功"
		gcmsg.result = 0
		
	else 
		
		local bindState = math.floor(pInfo.agencystatus/10)
		if bindState ~= 0 then
			--表示已经绑定过了
			gcmsg.result = ReturnCode["bind_is_apply"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		local sqlCase = "insert into ag_apply(userid,nickname,applycode) values("..pInfo.userid..",'"..pInfo.nickname.."','"..cgmsg.optstr.."')"
		mysqlItem:execute(sqlCase)
		sqlCase = "select id from ag_apply where userid="..pInfo.userid
		mysqlItem:executeQuery(sqlCase)
		local sqlData = mysqlItem:fetch()
		if sqlData == nil then
			gcmsg.result = ReturnCode["apply_fail"]
		else
			pInfo.agencystatus = pInfo.agencystatus + 10
			sqlCase = "update db_player set agencystatus="..pInfo.agencystatus.." where userid="..pInfo.userid
			SqlServer.rpush(sqlCase)
		end
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SendPlayerInfo(pInfo,{"agencystatus"})
	end
	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end