module("PszAudoCingl", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_2_pb.cgpszaudocingl()
	local gcmsg = msg_psz_2_pb.gcpszaudocingl()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszAudoCingl", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	ThreadManager.PszLock(cgmsg.tableid)
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		gcmsg.result = ReturnCode["psz_not_exist"]
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local is = false
	for i = 1 , #tInfo.situser do
		if cgmsg.userid == tInfo.situser[i].userid then
			tInfo.situser[i].audocingl = cgmsg.audocingl
			is = true
		end
	end
	
	if is == false then 
		gcmsg.result = ReturnCode["psz_not_sit"]
		ThreadManager.PszUnLock(cgmsg.tableid)	
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	gcmsg.userid = cgmsg.userid
	gcmsg.audocingl = cgmsg.audocingl
	gcmsg.tableid = cgmsg.tableid
	gcmsg.result = 0
	
	
	if cgmsg.audocingl == g_PszDefine.autoCingl_OK then
		local gcmsgb = msg_psz_pb.gcpszconcealbutton()
		gcmsgb.result = 0
		gcmsgb.flag = 0
		SendMessage(cgmsg.userid, PacketCode[3336].client, gcmsgb:ByteSize(), gcmsgb:SerializeToString())
		if cgmsg.userid == tInfo.actionuser then
			--跟注
			local gcmsgGz = msg_psz_pb.gcpszaction()
			local jetton = PszService.followjetton(tInfo)
			gcmsgGz.actiontype = g_PszDefine.followjetton
			gcmsgGz.addjetton = jetton
			gcmsgGz.oncepour = jetton
			
			gcmsgGz.guodi = tInfo.guodi
			gcmsgGz.result = 0
			gcmsgGz.userid = tInfo.actionuser
			local userIDList = PszService.GetUserIDList(tInfo)
			SendMessage(userIDList, PacketCode[3320].client, gcmsgGz:ByteSize(), gcmsgGz:SerializeToString())
			
			--到下一位玩家
			PszService.NextAction(tInfo)
			if #tInfo.actionlist == 1 or tInfo.presentbout > tInfo.boutcapp then
				tInfo.state = g_PszDefine.state_play
				tInfo.timemark = -2	
			end
		end
	end
	
	PszModel.SetTableInfo(tInfo, 1)
	ThreadManager.PszUnLock(cgmsg.tableid)	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end
