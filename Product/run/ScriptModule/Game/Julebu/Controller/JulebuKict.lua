
module("JulebuKict", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebukict()
	local gcmsg = msg_julebu_pb.gcjulebukict()
	
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
	
	if jInfo.ownerid ~= cgmsg.userid then
		gcmsg.result = ReturnCode["jlb_kict_error"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local mInfo = JulebuModel.GetUserMemberInfo(cgmsg.julebuid, cgmsg.userid)
		if mInfo == nil then
			gcmsg.result = ReturnCode["jlb_not_exist"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
	
	local kictlist = {}
	for k1, v1 in ipairs(cgmsg.kictid) do
	
		if cgmsg.userid~= v1  then
			if true == JulebuModel.IsJoinIn(v1, cgmsg.julebuid) then
				--查看是否是管理员，不能踢管理员
				for k, v in ipairs(jInfo.adminlist) do
					if v == v1  then
						gcmsg.result = ReturnCode["jlb_kict_fail_1"]
						return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
					end
				end
				
				sqlCase = "update dy_julebu_member set status="..g_JulebuDefine.mstatus_kict.." where userid="..v1.." and julebuid="..cgmsg.julebuid
				mysqlItem:execute(sqlCase)
				
				if true == JulebuModel.IsOnline(cgmsg.julebuid, v1) then
					local gcQuit = msg_julebu_pb.gcjulebuquit()
					gcQuit.result = 0
					gcQuit.julebuid = cgmsg.julebuid
					SendMessage(v1, PacketCode[3222].client, gcQuit:ByteSize(), gcQuit:SerializeToString())
					JulebuModel.DelIsOnline(cgmsg.julebuid, v1)  -- 在在线列表中删除
				end
				--local aMember = JulebuModel.GetMember()
				--JulebuModel.DelFromApplyList
				jInfo.membernum = jInfo.membernum - 1
				sqlCase = "update dy_julebu set membernum="..jInfo.membernum.." where id="..cgmsg.julebuid
				mysqlItem:execute(sqlCase)	
				JulebuModel.SetJulebuInfo(jInfo)
				JulebuModel.DelFromMemberList(cgmsg.julebuid, v1)   --从玩家列表中删除
				JulebuModel.DelFromUserJulebuList(v1, cgmsg.julebuid)
				
				JulebuService.CheckDelJuelbuPlayer(jInfo, v1)
				gcmsg.kictid:append(v1)
				table.insert(kictlist, v1)
			end	
		end
		
	end
	
	gcmsg.result = 0
	SendMessage(kictlist, PacketCode[3218].client, gcmsg:ByteSize(), gcmsg:SerializeToString())   --这个是通知被踢的那个人

	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end