
module("JulebuList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebulist()
	local gcmsg = msg_julebu_pb.gcjulebulist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuList", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuList", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	local getList = {}
	if cgmsg.gettype == 2 then  
		--这个是获取推荐列表的
		--首先，从上一级代理找起，如果代理存在俱乐部的，就不用找了。如果没有存在，就继续往上找
		local agency = AgencyModel.GetAgencyInfo(cgmsg.userid)   --找到代理信息
			if agency ~= nil then 
			--从下玩上找
			local isExist = false
			
			
			getList = JulebuModel.GetUserJulebuList(agency.agent1)

			if #getList > 0 then
				table.sort(getList, function(a,b) return a < b end)
				for k,v in ipairs(getList) do    --
					
					local jInfo = JulebuModel.GetJulebuInfo(v)
					
					if jInfo.ownerid == agency.agent1 then
						if false == JulebuModel.IsJoinIn(cgmsg.userid, jInfo.julebuid) then
							gcmsg.julebuid:append(jInfo.julebuid)
							gcmsg.name:append(jInfo.name)
							gcmsg.membernum:append(jInfo.membernum)
							gcmsg.maxmembernum:append(jInfo.maxmembernum)
							local pInfo = PlayerModel.GetPlayerInfo(jInfo.ownerid)
							gcmsg.ownerfaceid:append(pInfo.face_1)
							gcmsg.ownernick:append(pInfo.nickname)
							gcmsg.tablenum:append(0)
						end
						isExist = true
					end
				end					
			end
			
			if isExist == false then
				getList = JulebuModel.GetUserJulebuList(agency.agent2)
				
				table.sort(getList, function(a,b) return a < b end)
				for k,v in ipairs(getList) do    --
					
					local jInfo = JulebuModel.GetJulebuInfo(v)
						
					if jInfo.ownerid == agency.agent2 then
						if false == JulebuModel.IsJoinIn(cgmsg.userid, jInfo.julebuid) then
							gcmsg.julebuid:append(jInfo.julebuid)
							gcmsg.name:append(jInfo.name)
							gcmsg.membernum:append(jInfo.membernum)
							gcmsg.maxmembernum:append(jInfo.maxmembernum)
							local pInfo = PlayerModel.GetPlayerInfo(jInfo.ownerid)
							gcmsg.ownerfaceid:append(pInfo.face_1)
							gcmsg.ownernick:append(pInfo.nickname)
							gcmsg.tablenum:append(0)
						end
						isExist = true
					end
				end					
			end
			if isExist == false then
				getList = JulebuModel.GetUserJulebuList(agency.agent3)
				table.sort(getList, function(a,b) return a < b end)
				for k,v in ipairs(getList) do    --
					local jInfo = JulebuModel.GetJulebuInfo(v)
						
					if jInfo.ownerid == agency.agent3 then
						if false == JulebuModel.IsJoinIn(cgmsg.userid, jInfo.julebuid) then
							gcmsg.julebuid:append(jInfo.julebuid)
							gcmsg.name:append(jInfo.name)
							gcmsg.membernum:append(jInfo.membernum)
							gcmsg.maxmembernum:append(jInfo.maxmembernum)
							local pInfo = PlayerModel.GetPlayerInfo(jInfo.ownerid)
							gcmsg.ownerfaceid:append(pInfo.face_1)
							gcmsg.ownernick:append(pInfo.nickname)
							gcmsg.tablenum:append(0)
						end
						isExist = true
					end
				end					
			end		
			if isExist == false then
				getList = JulebuModel.GetUserJulebuList(agency.agent4)
				table.sort(getList, function(a,b) return a < b end)
				for k,v in ipairs(getList) do    --
					local jInfo = JulebuModel.GetJulebuInfo(v)
						
					if jInfo.ownerid == agency.agent4 then
						if false == JulebuModel.IsJoinIn(cgmsg.userid, jInfo.julebuid) then
							gcmsg.julebuid:append(jInfo.julebuid)
							gcmsg.name:append(jInfo.name)
							gcmsg.membernum:append(jInfo.membernum)
							gcmsg.maxmembernum:append(jInfo.maxmembernum)
							local pInfo = PlayerModel.GetPlayerInfo(jInfo.ownerid)
							gcmsg.ownerfaceid:append(pInfo.face_1)
							gcmsg.ownernick:append(pInfo.nickname)
							gcmsg.tablenum:append(0)
						end
						isExist = true
					end
				end					
			end		
			if isExist == false then
				getList = JulebuModel.GetUserJulebuList(agency.soleagent)
				table.sort(getList, function(a,b) return a < b end)
				for k,v in ipairs(getList) do    --
					local jInfo = JulebuModel.GetJulebuInfo(v)
						
					if jInfo.ownerid == agency.soleagent then
						if false == JulebuModel.IsJoinIn(cgmsg.userid, jInfo.julebuid) then
							gcmsg.julebuid:append(jInfo.julebuid)
							gcmsg.name:append(jInfo.name)
							gcmsg.membernum:append(jInfo.membernum)
							gcmsg.maxmembernum:append(jInfo.maxmembernum)
							local pInfo = PlayerModel.GetPlayerInfo(jInfo.ownerid)
							gcmsg.ownerfaceid:append(pInfo.face_1)
							gcmsg.ownernick:append(pInfo.nickname)
							gcmsg.tablenum:append(0)
						end
						isExist = true
					end
				end					
			end		
		end
		--如果实在是找不到， 我也没有办法了
	else
		--这个是获取自己的牌友圈列表的，这里只能是这样的了
		getList = JulebuModel.GetUserJulebuList(cgmsg.userid)
		table.sort(getList, function(a,b) return a < b end)
		for k,v in ipairs(getList) do    --
			local jInfo = JulebuModel.GetJulebuInfo(v)
			if jInfo ~= nil then
				gcmsg.julebuid:append(jInfo.julebuid)
				gcmsg.name:append(jInfo.name)
				gcmsg.membernum:append(jInfo.membernum)
				gcmsg.maxmembernum:append(jInfo.maxmembernum)
				local pInfo = PlayerModel.GetPlayerInfo(jInfo.ownerid)
				gcmsg.ownerfaceid:append(pInfo.face_1)
				gcmsg.ownernick:append(pInfo.nickname)
				gcmsg.tablenum:append(0)
				local addadminlist = gcmsg.adminlist:add()
				for k,v in ipairs(jInfo.adminlist)do 
					addadminlist.adminlist:append(v)
				end
			else
				JulebuModel.DelFromMemberList(v, cgmsg.userid)
			end
		end		
	end
	
	gcmsg.result = 0
	gcmsg.gettype = cgmsg.gettype
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end