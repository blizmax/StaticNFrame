module("DouNiuJoinRoom", package.seeall)

function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_douniu_pb.cgdouniujoinroom()
	local gcmsg = msg_douniu_pb.gcdouniujoinroom()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)

	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuJoinRoom", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuJoinRoom", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local tableID = DouNiuModel.GetUserTableID(cgmsg.userid)
	if tableID ~= 0 and tableID ~= nil then
		local tInfo =  DouNiuModel.GetTableInfo(tableID)
		if tInfo == nil or tInfo == 0 then
			gcmsg.result = ReturnCode["douniu_not_exist"]
			DouNiuModel.DelUserTableID(cgmsg.userid)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		for k,v in ipairs(tInfo.situser) do	
			if v ~= 0 then
				local tempInfo = PlayerModel.GetPlayerInfo(v.userid)
				local addItem = gcmsg.pslist:add()
				PlayerModel.ChangeSimpleInfo(tempInfo, addItem)
				addItem.jifen = v.carryjetton
			end
		end
		
		gcmsg.strvipinfo = tInfo.specialtype
		gcmsg.hinfo:ParseFromString(tInfo:SerializeToString())
		if tInfo.state == g_douNiuDefine.state_banker or tInfo.state == g_douNiuDefine.state_start or tInfo.state == g_douNiuDefine.state_wait then
			for i = 1, #gcmsg.hinfo.situser do	
				if cgmsg.userid == gcmsg.hinfo.situser[i].userid then
					if tInfo.dntype == g_douNiuType.type_mpqz then
						--print(cgmsg.userid)
						if #gcmsg.hinfo.situser[i].handpoker > 0 then
							gcmsg.hinfo.situser[i].handpoker[#gcmsg.hinfo.situser[i].handpoker] = 0
						end
					else
						for j = 1, #gcmsg.hinfo.situser[i].handpoker  do 
							gcmsg.hinfo.situser[i].handpoker[j] = 0
						end
					end
				else
					gcmsg.hinfo.situser[i].nntype = 0
					for j = 1, #gcmsg.hinfo.situser[i].handpoker  do 
						gcmsg.hinfo.situser[i].handpoker[j] = 0
					end
				end
			end
		end
		
		gcmsg.result = 0	
		SendMessage(cgmsg.userid, PacketCode[3010].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		--[[for k,v in ipairs(tInfo.standuser) do
			if cgmsg.userid == v and tInfo.usevipnum > 0 then
				local gcmsgtmp = msg_douniu_2_pb.gcdouiudynamicjion()
				gcmsgtmp.userid = cgmsg.userid
				gcmsgtmp.result = 0
				SendMessage(cgmsg.userid, PacketCode[3050].client, gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())	
			end
		end]]
		return cgmsg.userid, 0, 0, gcmsg:SerializeToString()	
	end
	
	
	tableID = cgmsg.tableid
	if tableID == 0 or tableID == nil then
		--tableID = BrnnService.GetTableID(pInfo.jetton)
		LogBehavior.Error(cgmsg.userid, "douniu", "DouNiuJoinRoom", ReturnCode["douniu_not_exist"], "牌桌不存在，tableid="..tableID)
		gcmsg.result = ReturnCode["douniu_not_exist"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()			
	end
	
	ThreadManager.DouNiuLock(tableID) 
	local tInfo = DouNiuModel.GetTableInfo(tableID)
	
	if tInfo == nil then
		LogBehavior.Error(cgmsg.userid, "douniu", "DouNiuJoinRoom", ReturnCode["douniu_not_exist"], "牌桌不存在，tableid="..tableID)

		gcmsg.result = ReturnCode["douniu_not_exist"]
		ThreadManager.DouNiuUnLock(tableID)
		DouNiuModel.DelUserTableID(cgmsg.userid)	
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end 
	
	if tInfo.julebuid ~= 0 then
		--这里需要检查俱乐部的成员关系
		if false == JulebuService.IsJoinIn(cgmsg.userid, tInfo.julebuid) then
			gcmsg.result = ReturnCode["jlb_game_enter_error"]
			ThreadManager.DouNiuUnLock(tableID)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
		end
	end	
	
	--AA付费查看钻石够不够
	if false ==  DouNiuService.DouNiucheck(tInfo, pInfo) then 
		gcmsg.result = ReturnCode["douniu_money_short"]
		ThreadManager.DouNiuUnLock(tableID)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	--房间满人的情况
	--if #tInfo.situser  >= tInfo.usermaxnum then
		--ThreadManager.DouNiuUnLock(tableID)
		--gcmsg.result = ReturnCode["douniu_full"]
		--return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	--end
	
	if tInfo.usevipnum > 0 then
		--中途禁入
		if tInfo.restrict == 1 then
			gcmsg.result = ReturnCode["douniu_room_close"]
			ThreadManager.DouNiuUnLock(tableID)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
		end
		
		--游戏开始后坐下玩家+站立玩家 满人的情况
		--if #tInfo.situser + #tInfo.standuser >= tInfo.usermaxnum then
			--ThreadManager.DouNiuUnLock(tableID)
			--gcmsg.result = ReturnCode["douniu_full"]
		--	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
		--end
		--没确定进入房间且是AA付费
		if cgmsg.resolute == 0 and tInfo.payway == g_douNiuDefine.payment_A then
			local gcmsgtmp = msg_douniu_2_pb.gcdouiuroominfo()
			gcmsgtmp.tableid = cgmsg.tableid
			local tmp = tInfo.maxplaynum - tInfo.usevipnum
			moneytmp = math.ceil(tmp / 10)
			--gcmsgtmp.msg = "当前房间还剩"..tmp.."局，需要消耗"..(moneytmp * 10) .."个玉，如玉不够就会扣消耗"..moneytmp.."个钻石"
			gcmsgtmp.msg = "当前房间还剩"..tmp.."局，需要消耗"..tInfo.paynum.."个钻石"
			gcmsgtmp.result = 0
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			SendMessage(cgmsg.userid, PacketCode[3052].client, gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())
			return cgmsg.userid, 0, 0, gcmsgtmp:SerializeToString()			
		end
		tInfo.standuser:append(cgmsg.userid)
		DouNiuModel.SetUserTableID(cgmsg.userid, tInfo.tableid)
		for k,v in ipairs(tInfo.situser) do	
			local tempInfo = PlayerModel.GetPlayerInfo(v.userid)
			local addItem = gcmsg.pslist:add()
			PlayerModel.ChangeSimpleInfo(tempInfo, addItem)
			addItem.jifen = v.carryjetton
		end
		gcmsg.strvipinfo = tInfo.specialtype
		gcmsg.hinfo:ParseFromString(tInfo:SerializeToString())
		for i = 1, #gcmsg.hinfo.situser do
			gcmsg.hinfo.situser[i].nntype = 0
			for j = 1, #gcmsg.hinfo.situser[i].handpoker  do 
				gcmsg.hinfo.situser[i].handpoker[j] = 0
			end
		end
		gcmsg.result = 0	
		SendMessage(cgmsg.userid, PacketCode[3010].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		--local gcmsgtmp = msg_douniu_2_pb.gcdouiudynamicjion()
		--gcmsgtmp.userid = cgmsg.userid
		--gcmsgtmp.result = 0
		--SendMessage(cgmsg.userid, PacketCode[3050].client, gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())
		DouNiuModel.SetTableInfo(tInfo,1)
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, 0, gcmsg:SerializeToString()
	end
	
	--把玩家加入站立列表
	tInfo.standuser:append(cgmsg.userid)
	DouNiuModel.SetUserTableID(cgmsg.userid, tInfo.tableid)
	
	if tInfo.autoready == g_douNiuDefine.autoready_OK then
		if tInfo.dntype == g_douNiuType.type_gdzj then
			local isownerid = false 
			for k,v in ipairs(tInfo.situser) do
				if tInfo.ownerid == v.userid then
					isownerid = true
				end
				--固定庄家，房主一定要坐下
				if #tInfo.situser == tInfo.usermaxnum - 1 and isownerid == false  and cgmsg.userid ~= tInfo.ownerid then
					gcmsg.result = ReturnCode["douniu_isownerid_no"]
					ThreadManager.DouNiuUnLock(cgmsg.tableid)
					return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
				end
			end
		end
		
		--把玩家从站立列表中删除
		for k,v in ipairs(tInfo.standuser) do
			if v == cgmsg.userid then
				tInfo.standuser:remove(k)
				break
			end
		end
		--玩家坐下
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
		addItem.userid = cgmsg.userid
		addItem.state = g_douniuPlayStatus.status_ready
		gcSit = msg_douniu_pb.gcdouniusitdown()
		PlayerModel.ChangeSimpleInfo(pInfo,gcSit.psinfo)
		
		if tInfo.dntype == g_douNiuType.type_gdzj and cgmsg.userid == tInfo.ownerid then
			addItem.carryjetton = tInfo.integralmax
			gcSit.psinfo.jifen = tInfo.integralmax
		else
			addItem.carryjetton = 0
			gcSit.psinfo.jifen = 0
		end
		gcSit.chairid = addItem.chairid
		gcSit.result = 0
		local userIDList = DouNiuService.GetUserIDList(tInfo, cgmsg.userid)
		SendMessage(userIDList, PacketCode[3014].client,gcSit:ByteSize(), gcSit:SerializeToString()) --发送给其他玩家
	end
	
	DouNiuModel.SetTableInfo(tInfo,1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)	
	--更新玩家数量		
	DouNiuEvent.JulebuGameUpdate(tInfo.julebuid, tInfo.tableid, #tInfo.situser, tInfo.julebutype, cgmsg.userid) 
	
	if tInfo.julebuid > 0 then
		JulebuModel.DelFromOnline(tInfo.julebuid, cgmsg.userid)
	end

	for k,v in ipairs(tInfo.situser) do	
		local tempInfo = PlayerModel.GetPlayerInfo(v.userid)
		local addItem = gcmsg.pslist:add()
		PlayerModel.ChangeSimpleInfo(tempInfo, addItem)
		addItem.jifen = v.carryjetton
	end
	gcmsg.strvipinfo = tInfo.specialtype
	gcmsg.hinfo:ParseFromString(tInfo:SerializeToString())
	gcmsg.result = 0	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end