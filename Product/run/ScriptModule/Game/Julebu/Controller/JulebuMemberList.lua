
module("JulebuMemberList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebumemberlist()
	local gcmsg = msg_julebu_pb.gcjulebumemberlist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuMemberList", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuMemberList", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	
	local getList = JulebuModel.GetMemberList(cgmsg.julebuid)
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	
	local temptime = os.date("*t", os.time())
	if jInfo ~= nil and temptime.day ~= jInfo.updateday and temptime.hour > 7 then
		jInfo.updateday = temptime.day
		jInfo.yesterdaypump = jInfo.todaypump
		jInfo.todaypump = 0
	end
	--print(jInfo.updateday..jInfo.yesterdaypump..jInfo.todaypump)
	local headcount = 0
	if getList ~= nil then
		if cgmsg.checktype == 1 then
			local count = 1
			for k, v in pairs(getList) do
				if true == JulebuModel.IsJoinIn(k,cgmsg.julebuid) then
					if cgmsg.checkoutid == 0 then
						--这里需要检查一下是否在当前俱乐部，因为有可能删除或者退出了，但是没有从这个列表中清空
						if count > (cgmsg.pagenum - 1)*10 and count <= cgmsg.pagenum*10 then
							local addItem = gcmsg.jmlist:add()
							addItem:ParseFromString(v)
							addItem.state = OnlineModel.CheckOnline(addItem.userid) == nil and 0 or 1
						end
						count = count + 1
						if count > cgmsg.pagenum*10 then
							break
						end	
					else
						if cgmsg.checkoutid == tonumber(k) then
							local addItem = gcmsg.jmlist:add()
							addItem:ParseFromString(v)
							addItem.state = OnlineModel.CheckOnline(addItem.userid) == nil and 0 or 1
							break
						end
					end
				end	
			end
			
			local alljifen = 0
			for k,v in pairs(gcmsg.jmlist) do
				if v.limitjifen then
					alljifen = alljifen + v.limitjifen
				end
			end
			jInfo.totaljifen = alljifen
			
			for k, v in pairs(getList) do --不知道为什么#getList是0 这样遍历却不是
				headcount = headcount + 1
			end
		elseif cgmsg.checktype == 2 then
			local tmplist = {}
			for k, v in pairs(getList) do
				local jInfo = st_julebu_pb.julebumember()
				jInfo:ParseFromString(v)
				local tmp = {}
				tmp[1] = k
				tmp[2] = jInfo.limitjifen
				table.insert(tmplist, tmp)
			end
			
			table.sort(tmplist, function(a,b) return a[2] > b[2] end)
			
			local count = 1
			for k, v in pairs(tmplist) do
				if true == JulebuModel.IsJoinIn(v[1],cgmsg.julebuid) then
					if count > (cgmsg.pagenum - 1)*10 and count <= cgmsg.pagenum*10 then
						local addItem = gcmsg.jmlist:add()
						addItem:ParseFromString(getList[v[1]])
						addItem.state = OnlineModel.CheckOnline(addItem.userid) == nil and 0 or 1
					end
					count = count + 1
					if count > cgmsg.pagenum*10 then
						break
					end	
				end
			end
		end
	end
	JulebuModel.SetJulebuInfo(jInfo)
	gcmsg.yesterdaypump = jInfo.yesterdaypump
	gcmsg.totaljifen = jInfo.totaljifen
	gcmsg.headcount = headcount
	gcmsg.checkoutid = cgmsg.checkoutid
	gcmsg.checktype = cgmsg.checktype
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end