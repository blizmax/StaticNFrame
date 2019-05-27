module("ReConnect", package.seeall)

--重连

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgreconnect()
	local gcmsg = msg_human_pb.gcreconnect()
	cgmsg:ParseFromString(buffer)
	gcmsg.result = ReturnCode["success"]
	gcmsg.msgbegin = tostring(math.myrandom(1000, 9999))

	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		gcmsg.result = -1
	else
		--检查是否已经在线
		--在这里检查是否需要做恢复
		--重连的时候需要做恢复
		--if false == OnlineModel.CheckOnline(cgmsg.userid) then
			local tableID,userIDList = PlayerModel.GetCurrTableID(cgmsg.userid)
			gcmsg.tableid = tonumber(tableID)
			PlayerModel.PlayerLogin(pInfo, false)
			OnlineModel.PlayerLogin(pInfo)  --从来年也要走到这里s
		--elseif g_isDebug ~= 1 then
		--	gcmsg.result  = -1
		--end
	end
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString(), gcmsg.msgbegin
end