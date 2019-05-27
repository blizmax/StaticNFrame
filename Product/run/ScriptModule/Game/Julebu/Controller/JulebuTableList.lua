
module("JulebuTableList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebutablelist()
	local gcmsg = msg_julebu_pb.gcjulebutablelist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuTableList", 0, "获取俱乐部列表，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuTableList", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	cgmsg.julebutype = cgmsg.julebutype == 0 and 1 or cgmsg.julebutype
	local getList = JulebuModel.GetGameList(cgmsg.julebuid, cgmsg.pagenum,cgmsg.julebutype)
	
	for k,v in ipairs(getList) do    --
		local strGet = JulebuModel.GetGameInfo(cgmsg.julebuid, v)
		if strGet ~= nil then
			local addItem = gcmsg.jlist:add()
			addItem:ParseFromString(strGet)
		else
			JulebuModel.RemoveGame(cgmsg.julebuid, v, cgmsg.julebutype) --如果没有取到，就删除
		end
	end
	gcmsg.julebutype = cgmsg.julebutype
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end