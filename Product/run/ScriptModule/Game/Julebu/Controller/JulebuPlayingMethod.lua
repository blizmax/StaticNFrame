
module("JulebuPlayingMethod", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebuplayingmethod()
	local gcmsg = msg_julebu2_pb.gcjulebuplayingmethod()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuApply", 0, "重复申请，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if nil == jInfo then
		--俱乐部不存在
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuPlayingMethod", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""	
	end
	
	
	if cgmsg.requesttype == 1 then	
		if cgmsg.playingmethod == "" or cgmsg.playingmethod == nil then
			gcmsg.result = ReturnCode["jlb_setwanfa_error1"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		--查看有没设置的权限
		local isJurisdiction = false 
		if jInfo.ownerid == cgmsg.userid then
			isJurisdiction = true
		end
			
		local len = #jInfo.adminlist
		for i = len,1,-1 do
			--从后面往回看
			if jInfo.adminlist[i] == cgmsg.userid then
				isJurisdiction = true
			end
		end
		
		if isJurisdiction == false then
			gcmsg.result = ReturnCode["jlb_is_admin2"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		JulebuModel.SetPlayingMethod(cgmsg.julebuid, cgmsg.julebutype, cgmsg.playingmethod,cgmsg.gametype)
	else
		local tmp = JulebuModel.GetPlayingMethod(cgmsg.julebuid, cgmsg.julebutype)
		if tmp ~= nil then
			local gInfo = st_julebu_pb.julebuplayingmethodinfo()
			gInfo:ParseFromString(tmp)
			gcmsg.playingmethod = gInfo.playingmethod
			gcmsg.gametype = gInfo.gametype
		end
	end
	
	gcmsg.julebuid = cgmsg.julebuid
	gcmsg.result = 0
	
	local onLine = JulebuModel.GetOnlineList(cgmsg.julebuid)
	SendMessage(onLine, PacketCode[3250].client, gcmsg:ByteSize(), gcmsg:SerializeToString())  --统一发送给全部人
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end