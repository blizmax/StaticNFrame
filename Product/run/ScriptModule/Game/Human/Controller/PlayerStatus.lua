
module("PlayerStatus", package.seeall)

--玩家退到后台已经从后台进入的时候发送该协议
--同时检查玩家是否在牌桌中
--
function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_human_pb.cgplayerstatus()
	local gcmsg = msg_human_pb.gcplayerstatus()
	cgmsg:ParseFromString(buffer)
	
	gcmsg.userid = cgmsg.userid
	gcmsg.status = cgmsg.status
	PlayerModel.PlayerStatus(cgmsg.userid,cgmsg.status)
	
	--在这里做退出已经进入的处理
	if cgmsg.status == g_playerState.state_resume then
		local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
		if pInfo ~= nil then
			OnlineModel.PlayerLogin(pInfo)
		end
	else
		OnlineModel.PlayerExit(cgmsg.userid)
	end

	return cgmsg.userid, 0, 0, gcmsg:SerializeToString()  --这里是不用给他自己发的
end
