module("DouNiuTrusteeship", package.seeall)

--这是托管

function execute(packetID, operateID, buffer)
	local cgmsg = msg_douniu_2_pb.cgdouniutrusteeship()
	local gcmsg = msg_douniu_2_pb.gcdouniutrusteeship()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.senderid, "douniu", "DouNiuTrusteeship", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end

	ThreadManager.DouNiuLock(cgmsg.tableid)
	
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		gcmsg.result = ReturnCode["douniu_not_exist"]
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local is = false
	for i = 1 , #tInfo.situser do
		if cgmsg.userid == tInfo.situser[i].userid then
			tInfo.situser[i].trusteeship = cgmsg.trusteeship
			if cgmsg.trusteeship == g_douNiuDefine.trusteeship_no then
				tInfo.situser[i].overtimenum = 0
				tInfo.situser[i].tgbet = 1
				tInfo.situser[i].tgbanker = 0
				tInfo.situser[i].tgbolus = 0
			elseif cgmsg.trusteeship == g_douNiuDefine.trusteeship_yes then
				tInfo.situser[i].tgbet = cgmsg.tgbet
				tInfo.situser[i].tgbanker = cgmsg.tgbanker
				tInfo.situser[i].tgbolus = cgmsg.tgbolus
				gcmsg.tgbet = cgmsg.tgbet
				gcmsg.tgbanker = cgmsg.tgbanker
				gcmsg.tgbolus = cgmsg.tgbolus
			end
			is = true
		end
	end
	
	if is == false then 
		gcmsg.result = ReturnCode["douniu_opt_error"]
		ThreadManager.DouNiuUnLock(cgmsg.tableid)	
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)	
	
	--print("cgmsg.volun = "..cgmsg.volun)
	gcmsg.userid = cgmsg.userid
	gcmsg.trusteeship = cgmsg.trusteeship
	gcmsg.result = 0
	
	local userIDList =  DouNiuService.GetUserIDList( tInfo, cgmsg.userid )
	SendMessage(userIDList, PacketCode[3054].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end

