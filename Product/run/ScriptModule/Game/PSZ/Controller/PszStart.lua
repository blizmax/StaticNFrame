module("PszStart", package.seeall)


function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_psz_pb.cgpszstart()
	local gcmsg = msg_psz_pb.gcpszstart()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszStart", 0,"缓存已存在")
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	ThreadManager.PszLock(cgmsg.tableid)
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid) 
	if tInfo == nil then
		gcmsg.result = ReturnCode["psz_not_exist"]
		LogBehavior.Warning(cgmsg.userid, "psz", "PszStart", 0,"牌桌不存在")
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	if tInfo.state ~= g_PszDefine.state_ready then
		--只有在准备状态，才能点击开始
		gcmsg.result = ReturnCode["psz_start_error"]
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()			
	end
	--看看有几个人准备了
	if #tInfo.situser <= 1 then
		--人数不足，不能开始
		gcmsg.result = ReturnCode["psz_not_enough"]
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	--[[local isok = true
	for k,v in ipairs(tInfo.situser)do 
		if v.state ~= g_PszPlayStatus.status_next then
			isok = false
		end
	end
	
	if isok == false then
		--人数不足，不能开始
		gcmsg.result = ReturnCode["psz_not_read"]
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end]]--
		
	--if tInfo.ownerid ~= cgmsg.userid then
		--只有房主才能开始房间
		--gcmsg.result = ReturnCode["psz_owner_start"]
		--ThreadManager.PszUnLock(cgmsg.tableid)
		--return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()		
	--end
	
	tInfo.state = g_PszDefine.state_start  
	tInfo.timemark = g_PszTime.start_time 
 
	if tInfo.julebuid ~= 0 then
		JulebuService.CreatTable(tInfo, g_JulebuDefine.modules_psz)
	end
	
	local gcmsgtmp = msg_psz_pb.gcpszleave()
	while #tInfo.standuser > 0 do
		gcmsgtmp.result = 0
		gcmsgtmp.userid = tInfo.standuser[1]
		gcmsgtmp.leavemsg = "游戏已开始，请无座玩家离开房间"
		
		PszModel.DelUserTableID(tInfo.standuser[1])
		
		SendMessage(tInfo.standuser[1], PacketCode[3012].client,gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())
		tInfo.standuser:remove(1)
	end

	PszModel.SetTableInfo(tInfo, 1)
	ThreadManager.PszUnLock(cgmsg.tableid)
	return cgmsg.userid, 0, 0,gcmsg:SerializeToString()
end