
module("JulebuQuit", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuquit()
	local gcmsg = msg_julebu_pb.gcjulebuquit()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuKict", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local sqlCase = "select id from dy_julebu_member where userid="..cgmsg.userid.." and julebuid="..cgmsg.julebuid
	
	mysqlItem:executeQuery(sqlCase)
	local memberID = mysqlItem:fetch()
	if memberID == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	gcmsg.julebuid = cgmsg.julebuid
	if cgmsg.userid ~= jInfo.ownerid then
		sqlCase = "update dy_julebu_member set status="..g_JulebuDefine.mstatus_quit.." where userid="..cgmsg.userid.." and julebuid="..cgmsg.julebuid
		mysqlItem:execute(sqlCase)
		JulebuModel.DelFromUserJulebuList(cgmsg.userid, cgmsg.julebuid)
		
		jInfo.membernum = jInfo.membernum - 1
		sqlCase = "update dy_julebu set membernum="..jInfo.membernum.." where id="..cgmsg.julebuid
		mysqlItem:execute(sqlCase)
		JulebuService.CheckDelJuelbuPlayer(jInfo, cgmsg.userid)
		JulebuModel.SetJulebuInfo(jInfo)
	else
		
		--检查一下是否还有比赛或者房间正在进行，如果有，则不能解散
		
		local gameList = JulebuModel.GetGameList(cgmsg.julebuid, 1, 1)
		
		if gameList ~= nil and #gameList ~= 0 then
			--这里是不能解散的
			gcmsg.result = ReturnCode["jlb_dissolve_game"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		local gameList = JulebuModel.GetGameList(cgmsg.julebuid, 1, 2)
		
		if gameList ~= nil and #gameList ~= 0 then
			--这里是不能解散的
			gcmsg.result = ReturnCode["jlb_dissolve_game"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		local tempList = JingSaiModel.GetJulebuAllList(cgmsg.julebuid)
		if tempList ~= nil and #tempList ~= 0 then
			gcmsg.result = ReturnCode["jlb_dissolve_game"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()			
		end
		--成员列表
		local getList = JulebuModel.GetMemberList(cgmsg.julebuid)
		for k,v in pairs(getList) do
			local gInfo = st_julebu_pb.julebumember()
			gInfo:ParseFromString(v)
			if gInfo.limitjifen ~= 0 then
				gcmsg.result = ReturnCode["jlb_dissolve_game_2"]
				return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
		end
	
		local onLineList = JulebuModel.GetOnlineList(cgmsg.julebuid)
		sqlCase = "select userid from dy_julebu_member where julebuid="..cgmsg.julebuid.." and status="..g_JulebuDefine.mstatus_normal
		local userIDList = {}
		mysqlItem:executeQuery(sqlCase)
		while true do
			local sqlData = mysqlItem:fetch()
			if sqlData == nil then
				break
			end
			--table.insert(userIDList, tonumber(sqlData))
			JulebuModel.DelFromUserJulebuList(sqlData, cgmsg.julebuid)
		end
		
		sqlCase = "update dy_julebu set status=81 where id="..cgmsg.julebuid
		mysqlItem:execute(sqlCase)
		
		sqlCase = "update dy_julebu_member set status="..g_JulebuDefine.mstatus_dissolve.." where julebuid="..cgmsg.julebuid.." and status="..g_JulebuDefine.mstatus_normal
		mysqlItem:execute(sqlCase)
		JulebuModel.DelJulebuPartnerInfo(cgmsg.julebuid)
		JulebuModel.DelPlayerHistory(cgmsg.julebuid)
		JulebuModel.DelJulebuInfo(cgmsg.julebuid)
		if onLineList ~= nil then
			SendMessage(onLineList, PacketCode[3222].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		end
	end
	
	--local aMember = JulebuModel.GetMember()
	--JulebuModel.DelFromApplyList
	
	JulebuModel.DelFromMemberList(cgmsg.julebuid, cgmsg.userid)   --从玩家列表中删除
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end