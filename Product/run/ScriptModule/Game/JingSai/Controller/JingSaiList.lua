
module("JingSaiList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsailist()
	local gcmsg = msg_jingsai_pb.gcjingsailist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "jingsai", "JingSaiList", 0, "获取竞赛列表，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "jingsai", "JulebuList", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	
	local tempList = JingSaiModel.GetJulebuAllList(cgmsg.julebuid) --安卓开始的时间的先后来排序
	local getList = {}
	local function insertList(jingSaiID, startTime)
		local addItem = {}
		addItem[1] = jingSaiID
		addItem[2] = tonumber(startTime)
		
		table.insert(getList, addItem)
		
		for i = #getList, 2, -1 do
			
			if getList[i][2] < getList[i-1][2] then
				local temp1 = getList[i-1][1]
				local temp2 = getList[i-1][2]
				getList[i-1][1] = getList[i][1]
				getList[i-1][2] = getList[i][2]
				getList[i][1] = temp1
				getList[i][2] = temp2
				--交换了他们连个的值
			else
				break
			end
			
		end
	end
	for k,v in pairs(tempList) do
		insertList(k,v)
	end	
	local currTime = TimeUtils.GetTime()
	for k,v in ipairs(getList) do
		local addItem = gcmsg.jslist:add()
		if nil ~= JingSaiModel.GetJingSaiInfo(v[1], addItem) then
			
			if addItem.state == g_jingSaiDefine.state_start then
				--只有开始的状态才会获取剩余时间
				addItem.lasttime =  addItem.longtime - (currTime - addItem.starttime)
				addItem.membernum = JingSaiModel.GetMemberNum(v[1])
			end
		end		
	end
	
	gcmsg.result = 0
	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end