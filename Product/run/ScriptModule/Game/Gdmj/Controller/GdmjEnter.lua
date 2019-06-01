
--广东麻将创建桌子

module("GdmjEnter", package.seeall)


function execute(packetID, operateID, buffer, isPrivate)

	local cgmsg = msg_gdmj_pb.cggdmjenter()
	local gcmsg = msg_gdmj_pb.gcgdmjenter()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjEnter", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"gdmj","GdmjEnter", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local tableID = cgmsg.tableid
	
	if tableID == 0 then
		tableID = GdmjModel.GetUserTableID(cgmsg.userid)
	end
	
	if tableID == nil or tableID == 0 then
		if isPrivate ~= nil then
			gcmsg.result = ReturnCode["gdmj_not_exist"]
			return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end

		--如果tableID 是0，表示进入的是金币场，这边需要随机分配
		--暂时先返回
		--先取得选择的类型

		
		if pInfo.jetton < 2000 then   --钱不够，立刻返回了
			gcmsg.result = ReturnCode["jetton_not_enough"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		if false == GdmjJinBiChang.Enter(pInfo, gcmsg.mjinfo, g_gdmjType.type_jbc, cgmsg.tabletype) then
			gcmsg.result = ReturnCode["gdmj_not_exist"]
		else
			for i = 1,#gcmsg.mjinfo.situser do
				if gcmsg.mjinfo.situser[i] ~= 0 then
					local addUser = gcmsg.userlist:add()
					GdmjModel.GetChairUser(gcmsg.mjinfo.tableid, gcmsg.mjinfo.situser[i], addUser)
				end
			end
		end
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	ThreadManager.GdmjLock(tableID)
	
	local tInfo = GdmjModel.GetTableInfo(tableID)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.GdmjUnLock(tableID)
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		GdmjModel.DelUserTableID(cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	if tInfo.julebuid ~= 0 then
		--检查是否是俱乐部的成员,如果不是俱乐部的成员，不能进去
		if false == JulebuService.IsJoinIn(cgmsg.userid, tInfo.julebuid) then
			gcmsg.result = ReturnCode["jlb_game_enter_error"]
			ThreadManager.GdmjUnLock(tableID)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
		end
		--看看积分是否足够

		if tInfo.julebutype == 2 then
			if false == GdmjEvent.CheckJiFen(tInfo.julebuid, cgmsg.userid) then
				--低于50就不能进入房间了
				ThreadManager.GdmjUnLock(tableID)
				gcmsg.result = ReturnCode["jlb_enter_error"]
				return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()				
			end
		end
	end	
	
	local isExist = false
	for k,v in ipairs(tInfo.situser) do
		if v == cgmsg.userid then
			isExist = true
		end
	end

	if isExist == true then
		for k,v in ipairs(tInfo.situser) do
			
			if v ~= 0 then
				local mjUser = gcmsg.userlist:add()
				GdmjModel.GetChairUser(tableID, v, mjUser)
			end
			
		end
		
		gcmsg.mjinfo:ParseFromString(tInfo:SerializeToString())
		
		local strVip = GdmjModel.GetRoomInfoStr(tableID)
		if strVip ~= nil then
			gcmsg.strvipinfo = strVip
		end
		gcmsg.result = 0
		ThreadManager.GdmjUnLock(tableID)
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--在这里，暂时判断牌桌是否已经满
	if tInfo.playernum >= tInfo.maxplayernum then   --私人房不提示这个
		ThreadManager.GdmjUnLock(tableID)
		gcmsg.result = ReturnCode["gdmj_full"]
		LogFile("info","GdmjEnter max player tableid="..tableID..",userid="..cgmsg.userid)
		return cgmsg.userid, 0 ,gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local strVipInfo = GdmjModel.GetRoomInfoStr(cgmsg.tableid)
	if strVipInfo ~= nil then
		--设置VIPInfo，在这里设定vipInfo的字符串
		gcmsg.strvipinfo = strVipInfo
	end
	
	local isStand = false
	for k, v in ipairs(tInfo.standuser) do
		if v == cgmsg.userid then
			isStand = true
		end
	end
	
	if isStand == false then
		tInfo.standuser:append(cgmsg.userid)
	end
	
	gcmsg.mjinfo:ParseFromString(tInfo:SerializeToString())
	
	for i = 1,#gcmsg.mjinfo.publicpoker do
		gcmsg.mjinfo.publicpoker[i] = 0   --这里不能够暴露公共牌的信息
	end
	
	local sitPos = 0
	for k,v in ipairs(gcmsg.mjinfo.situser) do
		if v ~= 0 then
			local addUser = gcmsg.userlist:add()   --把其他无座的玩家先加进来
			GdmjModel.GetChairUser(gcmsg.mjinfo.tableid,v,addUser)
			
			if v ~= cgmsg.userid then
				--这个是恢复的时候，非自己的公共牌不会下发
				for i = 1,#addUser.handpoker do
					addUser.handpoker[i] = 0
				end
			end
		end
	end
	
	--[[这里需要屏蔽坐下的功能
	if sitPos ~= 0 then
		local addUser = gcmsg.userlist:add()
		addUser.userid = cgmsg.userid
		PlayerModel.ChangeSimpleInfo(pInfo, addUser.psinfo)
		GdmjModel.SitDown(gcmsg.mjinfo, addUser, sitPos)  --玩家坐下来了
		
		--在这里，同时也要把消息广播给其他人
		local msgAdd = msg_gdmj_pb.gcgdmjsitdown()
		msgAdd.mjuser:ParseFromString(addUser:SerializeToString())
		msgAdd.result = 0
		
		local userIDList = GdmjWork.GetUserList(gcmsg.mjinfo,cgmsg.userid)
		
		SendMessage(userIDList,PacketCode[2216].client,msgAdd:ByteSize(),msgAdd:SerializeToString())
		--
	else
		--这里需要加入到无座玩家中
	end
	]]
	GdmjModel.SetUserTableID(cgmsg.userid, cgmsg.tableid)
	
	GdmjModel.SetTableInfo(tInfo,1)
	ThreadManager.GdmjUnLock(tableID)
	
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end