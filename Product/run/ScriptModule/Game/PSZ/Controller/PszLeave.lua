module("PszLeave", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_pb.cgpszleave()
	local gcmsg = msg_psz_pb.gcpszleave()

	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszLeave", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	ThreadManager.PszLock(cgmsg.tableid)
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		--房间不存在也可以离开
		gcmsg.result = 0
		gcmsg.userid = cgmsg.userid
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	if tInfo.state == g_PszDefine.state_delete then
		gcmsg.result = 0
		gcmsg.userid = cgmsg.userid
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	
	--站立玩家随时能离开
	local isExist = false
	local pos = 0
	for k,v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then
			isExist = true
			gcmsg.chairid = v.chairid
			pos = k
			break
		end
	end
	

	--坐下的玩家
	if isExist == true then
		--如果是金币场不管哪个状态都能退出
		if tInfo.tabletype == g_PszDefine.table_srf then
			if tInfo.julebuid == 0 then
				if tInfo.state ~= g_PszDefine.state_ready then 
					--离开的状态是，只有这个才能离开
					gcmsg.result = ReturnCode["psz_leave_state"]
		--			LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuLeave", 0,"牌桌不存在")
					ThreadManager.PszUnLock(cgmsg.tableid)
					return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
				end	
			else
				if tInfo.state ~= g_PszDefine.state_ready and tInfo.state ~= g_PszDefine.state_next then 
					--离开的状态是，只有这个才能离开
					gcmsg.result = ReturnCode["psz_leave_state"]
		--			LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuLeave", 0,"牌桌不存在")
					ThreadManager.PszUnLock(cgmsg.tableid)
					return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
				end	
				if tInfo.usevipnum > 0 then
					PszService.PszddJiFen_History(tInfo, cgmsg.userid)
				end
			end
		end
		tInfo.usernum = tInfo.usernum - 1 
		--金币场且游戏已经开始：先把玩家从行动玩家列表中删除
		if tInfo.tabletype ~= g_PszDefine.table_srf and tInfo.state ~= g_PszDefine.state_ready then 
			--如果当前玩家 == 行动玩家，就要把行动玩家设为下一个玩家
			if tInfo.actionuser == cgmsg.userid then
				tInfo.deluser = cgmsg.userid
				PszService.NextAction(tInfo)
			else
				tInfo.actionlist:remove(pos)
			end	
		end
		
		tInfo.situser:remove(pos)
		
		local gcmsgup = msg_psz_pb.gcpszstandup()
		
		gcmsgup.chairid = gcmsg.chairid
		gcmsgup.userid = cgmsg.userid
		gcmsgup.result = 0
		local userIDList =  PszService.GetUserIDList(tInfo, cgmsg.userid) 
		SendMessage(userIDList, PacketCode[3334].client, gcmsgup:ByteSize(), gcmsgup:SerializeToString())
		--向俱乐部更新玩家数量
		PszEvent.JulebuGameUpdate(tInfo.julebuid, tInfo.tableid, #tInfo.situser,tInfo.julebutype,cgmsg.userid)
		
		
	--没坐下的玩家
	else
		for k,v in ipairs(tInfo.standuser) do
			if v == cgmsg.userid then
				tInfo.standuser:remove(k)
				break
			end
		end
	end
	
	if tInfo.tabletype ~= g_PszDefine.table_srf then 
		if #tInfo.standuser + #tInfo.situser == 5 then
			PszModel.SetJettonTableList(tInfo.tabletype,tInfo.tableid)
		
		--把多余空闲的房间删除掉
		elseif #tInfo.standuser + #tInfo.situser == 0	then
			local jettonlist =  PszModel.GetJettonTableList(tInfo.tabletype)
			if #jettonlist > 10 then
				tInfo.state = g_PszDefine.state_delete
			end
		end	
	end	
	
	
	
	PszModel.DelUserTableID(cgmsg.userid)
	
	PszModel.SetTableInfo(tInfo,1)
	
	ThreadManager.PszUnLock(cgmsg.tableid)
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid

	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end