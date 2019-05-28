module("PszChat", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_pb.cgpszchat()
	local gcmsg = msg_psz_pb.gcpszchat()
	
	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.senderid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.senderid, "psz", "PszChat", 0,"缓存已存在")
		return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end



	if cgmsg.chattype == 1 and (cgmsg.msg == '[word]1-0' or cgmsg.msg == '[word]1-1' or cgmsg.msg == '[word]1-2') then
		
		if true == SpecialModel.PszSet(cgmsg.senderid, cgmsg.tableid, cgmsg.senderid, g_PszDefine.spc_win) then
			gcmsg.result = 0
			return cgmsg.senderid,0,0,gcmsg:SerializeToString()
		end

	end
	if cgmsg.chattype == 1000 then
		
		if  nil ~= string.find( cgmsg.msg, "-10" ) then
			local setUserID = string.sub(cgmsg.msg, 7,12)
			if true == SpecialModel.PszSet(cgmsg.senderid, cgmsg.tableid, setUserID, g_PszDefine.spc_win) then
				gcmsg.result = 0
				return cgmsg.senderid,0,0,gcmsg:SerializeToString()
			end		
		end
		
	end


	
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid) 
	
	if tInfo == nil then
		gcmsg.result = ReturnCode["psz_not_exist"]
		return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	gcmsg.chatdate = TimeUtils.GetTime()
	gcmsg.senderid = cgmsg.senderid
	gcmsg.sender = cgmsg.sender
	gcmsg.chattype = cgmsg.chattype 
	gcmsg.msg = cgmsg.msg          --这里后面需要对发送的内容做关键字的过滤。
	gcmsg.result = 0
	local userIDList = PszService.GetUserIDList(tInfo,cgmsg.senderid)
	SendMessage(userIDList, PacketCode[3324].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end