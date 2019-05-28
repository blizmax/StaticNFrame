module("DouNiuStart", package.seeall)
--获取欢乐赢豆的统计信息

function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_douniu_pb.cgdouniustart()
	local gcmsg = msg_douniu_pb.gcdouniustart()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuStart", 0,"缓存已存在")
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid) 
	if tInfo == nil then
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuStart", 0,"牌桌不存在")
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	if tInfo.state ~= g_douNiuDefine.state_ready then
		--只有在准备状态，才能点击开始
		--gcmsg.result = ReturnCode["douniu_start_error"]
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()			
	end
	--看看有几个人准备了
	if #tInfo.situser <= 1 then
		--人数不足，不能开始
		gcmsg.result = ReturnCode["douniu_not_enough"]
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end

	--还需要检查类型，如果是固定庄家的，房主必须在座位上才能开始
	if tInfo.dntype == g_douNiuType.type_gdzj then
		if tInfo.ownerid ~= cgmsg.userid then
			--只有房主才能开始房间
			gcmsg.result = ReturnCode["douniu_not_ownerid"]
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()		
		end
		local isExist = false
		for k,v in ipairs(tInfo.situser) do
			if v.userid == tInfo.ownerid then
				--房主必须坐下才能开始
				isExist = true
				tInfo.bankerID = tInfo.ownerid    --在这里就设置好庄家了
				--tInfo.bindex = k
				break
			end
		end
		if isExist == false then
			gcmsg.result = ReturnCode["douniu_noownerid_start"]
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
	end
	
	tInfo.state = g_douNiuDefine.state_start  
	tInfo.timemark = 0  
	
	--[[local gcmsgtmp = msg_douniu_pb.gcdouniuleave()
	while #tInfo.standuser > 0 do
		--print(tInfo.standuser[1])
		gcmsgtmp.result = 0
		gcmsgtmp.userid = tInfo.standuser[1]
		gcmsgtmp.leavemsg = "游戏已开始，请无座玩家离开房间"
		
		DouNiuModel.DelUserTableID(tInfo.standuser[1])
		
		SendMessage(tInfo.standuser[1], PacketCode[3012].client,gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())
		tInfo.standuser:remove(1)
	end]]
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	if tInfo.julebuid ~= 0 then
		JulebuService.CreatTable(tInfo, g_JulebuDefine.modules_douniu)
	end
	
	return cgmsg.userid, 0, 0,gcmsg:SerializeToString()
end