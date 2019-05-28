module("PszSitdown", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_pb.cgpszsitdown()
	local gcmsg = msg_psz_pb.gcpszsitdown()
	
	cgmsg:ParseFromString(buffer)
	

	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszSitDown", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end

	
	ThreadManager.PszLock(cgmsg.tableid)
	
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		gcmsg.result = ReturnCode["psz_not_exist"]
		LogBehavior.Warning(cgmsg.userid, "psz", "PszLeave", 0,"牌桌不存在")
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	--不允许重复坐下
	local isownerid = false
	for k,v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then 
			gcmsg.result = ReturnCode["psz_sit_ing"]
			LogBehavior.Warning(cgmsg.userid, "psz", "psz_sit_ing", 0,"已经在座位上")
			ThreadManager.PszUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
		if tInfo.ownerid == v.userid then
			isownerid = true
		end
	end
	

	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	--判断房间是否满人
	if #tInfo.situser >= tInfo.maxuser then
		gcmsg.result = ReturnCode["psz_full"]
		LogBehavior.Warning(cgmsg.userid, "psz", "PszSitDown", 0,"房间满人了")
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()		
	end

	
	----把玩家加入坐下玩家列表
	UserSitdown(tInfo, pInfo)

	
	
	PszModel.SetTableInfo(tInfo,1)
	ThreadManager.PszUnLock(cgmsg.tableid)
	
	
	return cgmsg.userid, 0, 0, gcmsg:SerializeToString()
end

--把玩家加入坐下玩家列表
function UserSitdown(tInfo, pInfo)
	
	local gcmsgsit = msg_psz_pb.gcpszsitdown()
	
	--取得chairid
	local chairidList = {0,0,0,0,0,0,0,0,0,0}
	for k,v in ipairs(tInfo.situser) do
		chairidList[v.chairid] = 1
	end
	local addItem = tInfo.situser:add()
	for k,v in ipairs(chairidList) do
		if v == 0 then
			addItem.chairid = k
			break
		end
	end
	
	addItem.userid = pInfo.userid
	gcmsgsit.chairid = addItem.chairid
	addItem.state = g_PszPlayStatus.status_ready

	--把玩家提出站立玩家列表
	for k,v in ipairs(tInfo.standuser) do
		if v == pInfo.userid then
			tInfo.standuser:remove(k)
			break
		end
	end

	
	local userIDList = PszService.GetUserIDList(tInfo)

	PlayerModel.ChangeSimpleInfo(pInfo,gcmsgsit.psinfo)

	addItem.jifen = 0
	addItem.countjetton = 0
	if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
		addItem.jifen = JulebuModel.GetUserJiFen(tInfo.julebuid, pInfo.userid)
		--addItem.countjetton = addItem.jifen
	end
	
	gcmsgsit.psinfo.jifen = addItem.jifen
	if tInfo.tabletype ~= g_PszDefine.table_srf then
		addItem.jifen = pInfo.jetton
		gcmsgsit.psinfo.jifen = pInfo.jetton
		--如果金币场满人，就踢出空闲房间列表
		if #tInfo.situser + #tInfo.standuser >= tInfo.maxuser then
			PszModel.DelJettonTableList(tInfo.tabletype,pInfo.userid)
		end
		
	end
	--向俱乐部更新玩家数量
	PszEvent.JulebuGameUpdate(tInfo.julebuid, tInfo.tableid, #tInfo.situser,tInfo.julebutype,pInfo.userid)
	
	gcmsgsit.result = 0
	SendMessage(userIDList, PacketCode[3314].client,gcmsgsit:ByteSize(), gcmsgsit:SerializeToString()) --发送给其他玩家

end