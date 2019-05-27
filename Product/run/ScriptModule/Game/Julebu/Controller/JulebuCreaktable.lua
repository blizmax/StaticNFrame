
module("JulebuCreaktable", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebucreaktable()
	local gcmsg = msg_julebu2_pb.gcjulebucreaktable()
	
	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuCreaktable", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""		
	end
	
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid,  0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--检查是不是俱乐部测成员
	if true ~= JulebuModel.IsJoinIn(cgmsg.userid, cgmsg.julebuid) then
		gcmsg.result = ReturnCode["jlb_not_member"]
		return cgmsg.userid, 0, gcmsg:SerializeToString(), gcmsg:ByteSize()
	end
	
	if jInfo.setroom == 0 or cgmsg.julebutype == 2 then
		if false == JulebuService.CheckIsAdmin(jInfo, cgmsg.userid) then
			gcmsg.result = ReturnCode["jlb_is_admin2"]
			return cgmsg.userid,  0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
	end
	
	
	local msg = JulebuModel.GetPlayingMethod(cgmsg.julebuid, cgmsg.julebutype)
	local ret = 0
	if msg ~= nil then
		local gInfo = st_julebu_pb.julebuplayingmethodinfo()
		gInfo:ParseFromString(msg)
		
		if gInfo.gametype == g_JulebuDefine.modules_douniu then
			local cgCr = msg_douniu_2_pb.cgdouniucreate()
			cgCr:ParseFromString(gInfo.playingmethod)
			cgCr.userid = cgmsg.userid
			cgCr.julebuid = cgmsg.julebuid
			cgCr.julebutype = cgmsg.julebutype
			ret = DouNiuEvent.CreateGame(cgCr, jInfo.ownerid)
		elseif gInfo.gametype == g_JulebuDefine.modules_pdk then
			local cgCr = msg_pdk_pb.cgpdkcreate()
			cgCr:ParseFromString(gInfo.playingmethod)
			cgCr.userid = cgmsg.userid
			cgCr.julebuid = cgmsg.julebuid
			cgCr.julebutype = cgmsg.julebutype
			ret = PdkEvent.CreateGame(cgCr, jInfo.ownerid)
		elseif gInfo.gametype == g_JulebuDefine.modules_psz then
			local cgCr = msg_psz_pb.cgpszcreate()
			cgCr:ParseFromString(gInfo.playingmethod)
			cgCr.userid = cgmsg.userid
			cgCr.julebuid = cgmsg.julebuid
			cgCr.menpai = cgCr.menpai + 1
			cgCr.julebutype = cgmsg.julebutype
			ret = PszEvent.CreateGame(cgCr, jInfo.ownerid)
		end
	end
	
	gcmsg.result = ret
	local msglen = 0
	if ret ~= 0 then
		msglen = gcmsg:ByteSize()
	end
	
	return cgmsg.userid, 0 , msglen, gcmsg:SerializeToString()
end