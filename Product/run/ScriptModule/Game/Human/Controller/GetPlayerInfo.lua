module("GetPlayerInfo", package.seeall)

--获取用户的信息

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cggetplayerinfo()
	local gcmsg = msg_human_pb.gcgetplayerinfo()
	cgmsg:ParseFromString(buffer)

	if nil == PlayerModel.GetPlayerInfo(cgmsg.userid, gcmsg.pinfo) then
		gcmsg.result = ReturnCode["player_not_exist"]
	else
		gcmsg.result = 0
	end
	--dump_pb(gcmsg.pinfo)
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	
end


