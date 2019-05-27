module("AlterSomePlayerInfo", package.seeall)

--更新玩家钻石

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgaltersomeplayerinfo()
	local gcmsg = msg_human_pb.gcaltersomeplayerinfo()
	cgmsg:ParseFromString(buffer)
	--print(cgmsg)
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil or cgmsg.optype == nil then
		gcmsg.result = -1
	else
		--print("***1**")
		gcmsg.result = 0
		gcmsg.userid = cgmsg.userid
		if cgmsg.optype == 3 then --设置玩家的被邀请权
			pInfo.caninvite = cgmsg.caninvite
		end
		local modMsg = {57,}
		PlayerModel.SetMysqlPlayerInfo(pInfo,modMsg)
		PlayerModel.SetPlayerInfo(pInfo)
	end
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end