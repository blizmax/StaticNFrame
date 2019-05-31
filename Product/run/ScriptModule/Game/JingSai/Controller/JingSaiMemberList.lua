
module("JingSaiMemberList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaimemberlist()
	local gcmsg = msg_jingsai_pb.gcjingsaimemberlist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "JingSai", "JingSaiMemberList", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "JingSai", "JingSaiMemberList", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	

	local tempList = JingSaiModel.GetAllUserJiFen(cgmsg.jingsaiid)
	local userList = {}
	--这里需要排序
	
	local function insertList(userID, jiFen)
		local addItem = {}
		addItem[1] = userID
		addItem[2] = tonumber(jiFen)
		
		table.insert(userList, addItem)
		
		for i = #userList, 2, -1 do
			
			if userList[i][2] > userList[i-1][2] then
				local tempUserID = userList[i-1][1]
				local tempJiFen = userList[i-1][2]
				userList[i-1][1] = userList[i][1]
				userList[i-1][2] = userList[i][2]
				userList[i][1] = tempUserID
				userList[i][2] = tempJiFen
				--交换了他们连个的值
			else
				break
			end
			
		end
	end
	for k,v in pairs(tempList) do
		insertList(k,v)
	end
	local nickList = JingSaiModel.GetAllUserNick(cgmsg.jingsaiid)
	local faceList = JingSaiModel.GetAllUserFaceID(cgmsg.jingsaiid)
	
	for k,v in ipairs(userList) do
		gcmsg.useridlist:append(tonumber(v[1]))
		gcmsg.jifenlist:append(v[2])
		if nickList[ v[1] ] ~= nil then
			gcmsg.nicklist:append( nickList[ v[1] ] )
		else
			gcmsg.nicklist:append( '' )
		end
		
		if faceList[ v[1] ] ~= nil then
			gcmsg.faceidlist:append(faceList[ v[1] ])
		else
			gcmsg.faceidlist:append()
		end
	end	

	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end