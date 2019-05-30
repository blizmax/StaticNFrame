
--广东麻将创建桌子

module("GdmjAction", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_gdmj_pb.cggdmjaction()
	local gcmsg = msg_gdmj_pb.gcgdmjaction()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjAction", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"gdmj","GdmjAction", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end


	ThreadManager.GdmjLock(cgmsg.tableid)
	
	local tItem = GdmjService.CreateItem(cgmsg.tableid)	
	
	if tItem == nil then
		--牌桌不存在
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		LogFile("warning","GdmjAction table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	local isPlaying = false
	for k,v in ipairs(tItem.m_tInfo.situser) do
		if v == cgmsg.userid then
			isPlaying = true
			break
		end
	end
	
	if isPlaying == false then
		--检查是否在牌桌中，如果不在，就该该玩家发送离开的协议
		local gcLeave = msg_gdmj_pb.gcgdmjleave()
		gcLeave.userid = cgmsg.userid
		gcLeave.tableid = cgmsg.tableid
		gcmsg.result = 0
		SendMessage(cgmsg.userid,PacketCode[2212].client,gcLeave:ByteSize(),gcLeave:SerializeToString())
		return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()   --这里就不用下发这个协议了。
	end
	
	--在这里需要发送一炮多响的
	local isExsit = false
	for k,v in ipairs(tItem.m_tInfo.nextinfo.actchairid) do
		if v ~= 0 and k == cgmsg.actchairid then  
			--一定要是当钱玩家才有,记住，这里是看，不是v
			isExsit = true
			break
		end
	end	
	
	if isExsit == false then
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()  --这里不能够再返回了
	end
	
	--这里是行动的开始
	
	local isCheckLegol = 0
	
	--在这里检查合法性
	if cgmsg.actiontype == g_gdmjAction.type_play then
		--如果是打牌的，手上必须有这张牌
		local pokerCount = 0
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].handpoker) do
			if v == cgmsg.actpokerid then
				pokerCount = 1
				break
			end
		end

		for k,v in ipairs(tItem.m_tInfo.nextinfo.actchairid) do
			if v ~= 0 and k == cgmsg.actchairid then  
				--一定要是当钱玩家才有,记住，这里是看，不是v
				pokerCount = pokerCount + 1
				break
			end
		end

		if pokerCount < 2 then
			isCheckLegol = 1    --表示非法的操作
		end
	elseif cgmsg.actiontype == g_gdmjAction.type_peng then
		local pokerCount = 0
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].handpoker) do
			if v == cgmsg.actpokerid then
				pokerCount = pokerCount + 1
				
			end
		end
		if pokerCount < 2 then
			--小于两张这样的牌，根本不能碰
			isCheckLegol = 2
		end
	elseif cgmsg.actiontype == g_gdmjAction.type_gang then
		local pokerCount = 0
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].handpoker) do
			if v == cgmsg.actpokerid then
				pokerCount = pokerCount + 1
			end
		end
		
		if pokerCount < 2 then
			--如果手上有两张，下面就不可能有碰的牌了
			for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].mjpokerlist) do
				
				if v.pokerlist[1] == cgmsg.actpokerid then
					if v.pokertype == g_gdmjAction.type_peng then
						pokerCount = 3
					end
				end
				
			end
		end

		--饶平麻将， 当牌堆减去马牌剩最后四张牌是， 不能杠
		if tItem.m_tInfo.mjtype == g_gdmjType.type_rpmj then
			if #tItem.m_tInfo.publicpoker <= #tItem.m_tInfo.manum + 4 then
				pokerCount = 0
			end
		end
		
		if pokerCount < 2 then
			--小于两张这样的牌，根本不能碰
			isCheckLegol = 3
		end 
	elseif cgmsg.actiontype == g_gdmjAction.type_buhua then
		local pokerCount = 0
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].handpoker) do
			--对补花在不在手牌进行检查
			if v == cgmsg.actpokerid then
				pokerCount = pokerCount + 1
			end
		end
		if pokerCount < 1 then
			isCheckLegol = 6
		end
	elseif cgmsg.actiontype == g_gdmjAction.type_hu then
		
		local pokerCount = 0

		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].tinglist) do
			if v == tItem.m_tInfo.nextinfo.actpokerid or v == -1 then
				pokerCount = 1
				break
			end
		end
		if pokerCount == 0 then
			for k,v in ipairs(tItem.m_tInfo.guilist) do
				if v == tItem.m_tInfo.nextinfo.actpokerid and #tItem.m_userList[cgmsg.actchairid].tinglist > 0 then
					pokerCount = 1
					break
				end
			end
		end
		if pokerCount == 0 and tItem.m_tInfo.mjtype == g_gdmjType.type_hzz or tItem.m_tInfo.mjtype == g_gdmjType.type_hdz or tItem.m_tInfo.mjtype == g_gdmjType.type_hbz then
			--多花胡牌的检查
			if tItem.m_vipRoomInfo.duohuahupai == 1 then
				--看看符不符合多花胡牌
				local huaNum = 0
				for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].outpoker) do
					if v > 60 then
						huaNum = huaNum + 1
					end
				end
				if huaNum >= 7 then
					pokerCount = 1
				end
			end			
		end
				
		if pokerCount == 0 then
			isCheckLegol = 4
		end		
	elseif cgmsg.actiontype == g_gdmjAction.type_qiangganghu or cgmsg.actiontype == g_gdmjAction.type_chihu then
		local pokerCount = 0


		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].tinglist) do
			if v == cgmsg.actpokerid or v == -1 then
				pokerCount = 1
				break
			end
		end

		if pokerCount == 0 then
			for k,v in ipairs(tItem.m_tInfo.guilist) do
				if v == cgmsg.actpokerid and #tItem.m_userList[cgmsg.actchairid].tinglist > 0 then
					pokerCount = 1
					break
				end
			end
		end
		
		if pokerCount == 0 and tItem.m_tInfo.mjtype == g_gdmjType.type_hzz or tItem.m_tInfo.mjtype == g_gdmjType.type_hdz or tItem.m_tInfo.mjtype == g_gdmjType.type_hbz  then
			--多花胡牌的检查
			if tItem.m_vipRoomInfo.duohuahupai == 1 then
				--看看符不符合多花胡牌
				local huaNum = 0
				for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].outpoker) do
					if v > 60 then
						huaNum = huaNum + 1
					else
						break
					end
				end
				if huaNum >= 7 then
					pokerCount = 1
				end
			end			
		end

		--饶平麻将， 当牌堆减去马牌剩最后四张牌是， 不能杠, 只能自摸
		if tItem.m_tInfo.mjtype == g_gdmjType.type_rpmj then
			if #tItem.m_tInfo.publicpoker <= #tItem.m_tInfo.manum + 4 then
				pokerCount = 0
			end
		end
				
		if pokerCount == 0 then
			isCheckLegol = 5
		end
	end
	
	if isCheckLegol ~= 0 then
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_opt_error"]
		LogFile("warning","gdmj opt error="..isCheckLegol..",tableid="..cgmsg.tableid..",actchairid="..cgmsg.actchairid..",pokerid="..cgmsg.actpokerid..",action="..cgmsg.actiontype..",tarcharid="..cgmsg.tarchairid)
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	GdmjService.PlayerAction(tItem,cgmsg,gcmsg)
	--行动完后，开始CheckMoving
	if tItem.m_tInfo.status == g_gdmjStatus.status_playing and tItem.m_tInfo.userstate == g_gdmjUserState.state_waiting then
		--只有玩家的装修是在等待的时候，才是检查更新的时候
		--只有这时才checkmoving
		if cgmsg.actiontype ~= g_gdmjAction.type_buhua then
			GdmjService.CheckMoving(tItem,false)   --这里先不action，为了查出乱牌的问题
		end
	end
	
--[[	for k,v in ipairs(tItem.m_userModify) do
		if v ~= 0 then
			GdmjModel.SetChairUser(tItem.m_tInfo.tableid,tItem.m_userList[k])
		end
	end--]]
	tItem:SetRedis()	
	GdmjModel.SetTableInfo(tItem.m_tInfo, 1)
	
	
	ThreadManager.GdmjUnLock(cgmsg.tableid)
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()  --这里不能够再返回了
end