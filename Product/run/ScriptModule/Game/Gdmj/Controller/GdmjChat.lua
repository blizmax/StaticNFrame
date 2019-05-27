


module("GdmjChat", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_gdmj_pb.cggdmjchat()
	local gcmsg = msg_gdmj_pb.gcgdmjchat()
	
	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.senderid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.senderid, "hlyd", "HlydEnter", 0,"缓存已存在")
		return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local tInfo = GdmjModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	if cgmsg.msg == '[word]0-1' or cgmsg.msg == '[word]1-1' or  cgmsg.msg == '[word]2-1' then
		if true == SpecialModel.MarkSetType(cgmsg.senderid, g_logintable.type_gdmj) then
			gcmsg.result = 0
			return cgmsg.senderid,0,0,gcmsg:SerializeToString()
		end
	elseif cgmsg.msg == '[word]0-2' or cgmsg.msg == '[word]1-2' or  cgmsg.msg == '[word]2-2' then
		if true == SpecialModel.MarkDecByType(cgmsg.senderid, g_logintable.type_gdmj) then
			gcmsg.result = 0
			return cgmsg.senderid,0,0,gcmsg:SerializeToString()
		end		
	elseif cgmsg.msg == '[word]0-3' or cgmsg.msg == '[word]1-3' or cgmsg.msg == '[word]2-3' then
		if true == SpecialModel.MarkSetType(cgmsg.senderid, g_logintable.majiang_win) then
			gcmsg.result = 0
			return cgmsg.senderid,0,0,gcmsg:SerializeToString()
		end			
	end	

	
	

	
	gcmsg.chatdate = TimeUtils.GetTime()
	gcmsg.senderid = cgmsg.senderid
	gcmsg.sender = cgmsg.sender
	gcmsg.chattype = cgmsg.chattype
	gcmsg.msg = cgmsg.msg          --这里后面需要对发送的内容做关键字的过滤。
	gcmsg.result = 0
	local userIDList =  GdmjWork.GetUserList( tInfo, cgmsg.senderid )
	SendMessage(userIDList, PacketCode[2236].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end