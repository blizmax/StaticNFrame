--进入小马快跑


module("DouNiuSitdown", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_pb.cgdouniusitdown()
	local gcmsg = msg_douniu_pb.gcdouniusitdown()
	
	cgmsg:ParseFromString(buffer)
	--print(cgmsg)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "sdb", "SdbSitDown", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end

	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuLeave", 0,"牌桌不存在")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	--不允许重复坐下
	local isownerid = false
	for k,v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then 
			gcmsg.result = ReturnCode["douniu_sit_ing"]
			LogBehavior.Warning(cgmsg.userid, "douniu", "douniu_sit_ing", 0,"已经在座位上")
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
		if tInfo.ownerid == v.userid then
			isownerid = true
		end
	end
	
	--固定庄家，房主一定要坐下
	if tInfo.dntype == g_douNiuType.type_gdzj then
		if #tInfo.situser == tInfo.usermaxnum - 1 and isownerid == false  and cgmsg.userid ~= tInfo.ownerid then
			gcmsg.result = ReturnCode["douniu_isownerid_no"]
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	--判断房间是否满人
	if #tInfo.situser >= tInfo.usermaxnum then
		gcmsg.result = ReturnCode["douniu_full"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuLeave", 0,"房间满人了")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()		
	end
	
	if tInfo.julebutype == 2 then
		if false == DouNiuEvent.CheckJiFen(tInfo.julebuid, cgmsg.userid) then
			gcmsg.result = ReturnCode["jlb__enter_jetton_error"]
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()				
		end
	end

	
	--坐下
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
	
	addItem.userid = cgmsg.userid
	gcmsg.chairid = addItem.chairid
	if tInfo.usevipnum == 0 or tInfo.state == g_douNiuTime.next_time then
		addItem.state =  g_douniuPlayStatus.status_next
	else
		addItem.state = g_douniuPlayStatus.status_ready 
	end
	tInfo.usernum = tInfo.usernum + 1
	--如果是固定庄家 在房主的积分先赋值
	--在游戏开始后坐下的玩家，暂时不加入玩家人数，在结算完成后才算上 ，在通比牛牛结算时有用
	
	for k,v in ipairs(tInfo.standuser) do
		if v == cgmsg.userid then
			tInfo.standuser:remove(k)
			break
		end
	end

	
	local userIDList = DouNiuService.GetUserIDList(tInfo,cgmsg.userid)

	PlayerModel.ChangeSimpleInfo(pInfo,gcmsg.psinfo)
	if tInfo.dntype == g_douNiuType.type_gdzj and cgmsg.userid == tInfo.ownerid then
		if tInfo.julebutype == 2 then
			local member = JulebuModel.GetUserMemberInfo(tInfo.julebuid, cgmsg.userid)
			addItem.carryjetton = member.limitjifen
			gcmsg.psinfo.jifen = member.limitjifen
		else
			addItem.carryjetton = tInfo.integralmax
			gcmsg.psinfo.jifen = tInfo.integralmax
		end
	else
		if tInfo.julebutype == 2 then
			local member = JulebuModel.GetUserMemberInfo(tInfo.julebuid, cgmsg.userid)
			addItem.carryjetton = member.limitjifen
			gcmsg.psinfo.jifen = member.limitjifen
		else
			addItem.carryjetton = 0
			gcmsg.psinfo.jifen = 0
		end
	end
	
	--游戏后坐下的需要在这里扣费
	if tInfo.usevipnum > 0 and tInfo.payway == g_douNiuDefine.payment_A then
		if false == DouNiuEvent.IsBiSai(tInfo.julebuid) then
			PlayerModel.DecMoney(pInfo, tInfo.paynum, "douniu","sit")  --创建房间
			PlayerModel.SetPlayerInfo(pInfo)	
			PlayerModel.SendPlayerInfo(pInfo,{"money"})
		end
	end
	
	if #tInfo.situser == 1 then
		local gcCap = msg_douniu_2_pb.gcdouniucaptainchangeinform()
		gcCap.result = 0
		gcCap.captainid = tInfo.situser[1].userid
		local userlist = DouNiuService.GetUserIDList(tInfo)
		SendMessage(userlist, PacketCode[3064].client,gcCap:ByteSize(), gcCap:SerializeToString())
	end
	
	
	DouNiuModel.SetTableInfo(tInfo,1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	--更新玩家数量		
	DouNiuEvent.JulebuGameUpdate(tInfo.julebuid, tInfo.tableid, #tInfo.situser, tInfo.julebutype, cgmsg.userid)  
	gcmsg.result = 0
	--print(gcmsg)
	SendMessage(userIDList, PacketCode[3014].client,gcmsg:ByteSize(), gcmsg:SerializeToString()) --发送给其他玩家
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end