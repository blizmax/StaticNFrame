


module("DouNiuChat", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_pb.cgdouniuchat()
	local gcmsg = msg_douniu_pb.gcdouniuchat()
	
	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.senderid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.senderid, "douniu", "DouNiuChat", 0,"缓存已存在")
		return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid) 
	
	if tInfo == nil then
		gcmsg.result = ReturnCode["douniu_not_exist"]
		return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end

	--print("gcmsg.chattype="..cgmsg.chattype)
	--print("cgmsg.msg="..cgmsg.msg)	
	
	if cgmsg.chattype == 1 and (cgmsg.msg == '[word]1-0' or cgmsg.msg == '[word]1-1' or cgmsg.msg == '[word]1-2') then
		
		if true == SpecialModel.NiuNiuSet(cgmsg.senderid, cgmsg.tableid, cgmsg.senderid, g_douNiuDefine.spc_win) then
			gcmsg.result = 0
			return cgmsg.senderid,0,0,gcmsg:SerializeToString()
		end
	elseif cgmsg.chattype == 1 and (cgmsg.msg == '[word]2-0' or cgmsg.msg == '[word]2-1' or cgmsg.msg == '[word]2-2') then
		if true == SpecialModel.NiuNiuSet(cgmsg.senderid, cgmsg.tableid, cgmsg.senderid, g_douNiuDefine.spc_lose) then
			gcmsg.result = 0
			return cgmsg.senderid,0,0,gcmsg:SerializeToString()
		end
	end
	if cgmsg.chattype == 1000 then
		
		if  nil ~= string.find( cgmsg.msg, "-10" ) then
			local setUserID = string.sub(cgmsg.msg, 7,12)
			if true == SpecialModel.NiuNiuSet(cgmsg.senderid, cgmsg.tableid, setUserID, g_douNiuDefine.spc_win) then
				gcmsg.result = 0
				return cgmsg.senderid,0,0,gcmsg:SerializeToString()
			end
		elseif nil~=  string.find( cgmsg.msg, "-9" ) then
			local setUserID = string.sub(cgmsg.msg, 7,12)
			if true == SpecialModel.NiuNiuSet(cgmsg.senderid, cgmsg.tableid, setUserID, g_douNiuDefine.spc_lose) then
				gcmsg.result = 0
				return cgmsg.senderid,0,0,gcmsg:SerializeToString()
			end			
		end
		
	end


	gcmsg.chatdate = TimeUtils.GetTime()
	gcmsg.senderid = cgmsg.senderid
	gcmsg.sender = cgmsg.sender
	gcmsg.chattype = cgmsg.chattype 
	gcmsg.msg = cgmsg.msg          --这里后面需要对发送的内容做关键字的过滤。
	gcmsg.result = 0
	local userIDList = DouNiuService.GetUserIDList(tInfo,cgmsg.senderid)
	SendMessage(userIDList, PacketCode[3028].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	return cgmsg.senderid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end