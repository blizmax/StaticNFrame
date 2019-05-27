
module("JulebuSetAdmin", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebusetadmin()
	local gcmsg = msg_julebu_pb.gcjulebusetadmin()
	
	cgmsg:ParseFromString(buffer)
	if nil == JulebuModel.GetJulebuInfo(cgmsg.julebuid, gcmsg.jinfo) then
		--俱乐部不存在
		--只有群主才能设置管理员
		gcmsg.jinfo.julebuid = 0
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	local uInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if uInfo == nil then
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if cgmsg.userid ~= gcmsg.jinfo.ownerid then
		--只有群主才能设置管理员
		gcmsg.result = ReturnCode["jlb_set_error2"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if cgmsg.requesttype == 2 then
		for k, v in ipairs(gcmsg.jinfo.adminlist)do 
			local pInfo = PlayerModel.GetPlayerInfo(v)
			gcmsg.jinfo.adminfaclist:append(pInfo.face_1)
		end
		gcmsg.result = 0
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--检查这个ID是否是管理员
	local adminList = {}

	if cgmsg.settype == 1 then
		
		local adminInfo = PlayerModel.GetPlayerInfo(cgmsg.adminid)
		if adminInfo == nil then
			gcmsg.result = ReturnCode["player_not_exist"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		if true ~= JulebuModel.IsJoinIn(cgmsg.adminid, cgmsg.julebuid) then
			gcmsg.result = ReturnCode["jlb_setAdmin_error1"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		if cgmsg.userid == cgmsg.adminid then
			gcmsg.result = 0
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		gcmsg.jinfo.ownerid = cgmsg.adminid
		local sqlCase = "update dy_julebu set ownerid='"..cgmsg.adminid.."' where id="..cgmsg.julebuid
		mysqlItem:execute(sqlCase)
		
		local len = #gcmsg.jinfo.adminlist
		local isex = false 
		for i = len,1,-1 do
			--从后面往回看
			if gcmsg.jinfo.adminlist[i] == cgmsg.adminid then
				isex = true
			elseif gcmsg.jinfo.adminlist[i] == cgmsg.userid then
				gcmsg.jinfo.adminlist:remove(i)
			else
				table.insert(adminList,gcmsg.jinfo.adminlist[i])  --这是一个列表
			end
		end
		if isex == false then
			gcmsg.jinfo.adminlist:append(cgmsg.adminid)
			table.insert(adminList,cgmsg.adminid)
		end
		
		local sqlCase = "update dy_julebu set adminlist='"..luajson.encode(adminList).."' where id="..cgmsg.julebuid
		mysqlItem:execute(sqlCase)
		
		
	elseif cgmsg.settype == 2 then
		
		local adminInfo = PlayerModel.GetPlayerInfo(cgmsg.adminid)
		if adminInfo == nil then
			gcmsg.result = ReturnCode["player_not_exist"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		if true ~= JulebuModel.IsJoinIn(cgmsg.adminid, cgmsg.julebuid) then
			gcmsg.result = ReturnCode["jlb_setAdmin_error1"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
	
		if cgmsg.opttype == 1 then
		
			local isAdmin = false
			
			for k,v in ipairs(gcmsg.jinfo.adminlist) do
				if v == cgmsg.adminid then  --只有管理员才有权限修改公告
					isAdmin = true
					--break
				end
				table.insert(adminList,v)  --这是一个列表
			end
			
			if isAdmin == true then
				--改玩家已经是管理员了
				gcmsg.result = ReturnCode["jlb_admin_fail"]
				return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
			gcmsg.jinfo.adminlist:append(cgmsg.adminid)
		else
			if cgmsg.userid == cgmsg.adminid then
				--管理员自己不能取消自己的管理员资格
				--直接返回，不管了
				gcmsg.result = 0
				return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()			
			end
			
			local len = #gcmsg.jinfo.adminlist
			for i = len,1,-1 do
				--从后面往回看
				if gcmsg.jinfo.adminlist[i] == cgmsg.adminid then
					gcmsg.jinfo.adminlist:remove(i)
				else
					table.insert(adminList,gcmsg.jinfo.adminlist[i])  --这是一个列表
				end
			end
		end
		local sqlCase = "update dy_julebu set adminlist='"..luajson.encode(adminList).."' where id="..cgmsg.julebuid
		SqlServer.rpush(sqlCase)
		
	else
		gcmsg.jinfo.setroom = cgmsg.setroom
		gcmsg.jinfo.autoroom = cgmsg.autoroom
	end
	JulebuModel.SetJulebuInfo(gcmsg.jinfo)
	for k, v in ipairs(gcmsg.jinfo.adminlist)do 
		local pInfo = PlayerModel.GetPlayerInfo(v)
		gcmsg.jinfo.adminfaclist:append(pInfo.face_1)
	end
	gcmsg.result = 0
	local onLine = JulebuModel.GetOnlineList(cgmsg.julebuid)
	SendMessage(onLine, PacketCode[3244].client, gcmsg:ByteSize(), gcmsg:SerializeToString())  --统一发送给全部人
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end