module("UpdatePlayerMoney", package.seeall)

--更新玩家钻石

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgupdateplayermoney()
	local gcmsg = msg_human_pb.gcupdateplayermoney()
	cgmsg:ParseFromString(buffer)

	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		gcmsg.result = -1
	else
		gcmsg.result = 0
		gcmsg.userid = cgmsg.userid
		gcmsg.money = pInfo.money
	end
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end