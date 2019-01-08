
Zone.CmdUserInfoLoginCenter_C = function(cmd,zonetask)

	local uid = cmd.data.cmd_uid

	--玩家好友数据创建或登录
	local friendData = FriendManager:UserLoginFriend(uid, zonetask)
end

Zone.CmdForceUserOffline_S = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid

	--玩家好友数据创建或登录
	local friendData = FriendManager:GetFriendInfo(uid)
	if friendData == nil then
		unilight.error("login error, friend info not exist.......uid:"..tostring(uid))
		return
	end

	local res = {}
	res["do"] = "Cmd.UserInfoLoginCenter_S"
	res["data"] = {
		cmd_uid = uid,
	}
	FriendManager.SendCmdByFriend(res["do"], res["data"], friendData)
end

Zone.CmdUserDisconnected_C = function(cmd,zonetask)
	local uid = cmd.data.cmd_uid

	local friendData = FriendManager:GetFriendInfo(uid)
	if friendData == nil then
		unilight.error("login error, friend info not exist.......uid:"..tostring(uid))
		return
	end

	--同步玩家数据到好友数据
	friendData:SetStar(cmd.data.userInfo.star)
	friendData:SetMoney(cmd.data.userInfo.money)
	friendData:SetProduct(cmd.data.userInfo.product)

    FriendManager:UserLogoutFriend(uid, zonetask)
end

Zone.CmdUserReconncted_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid

	--玩家好友数据创建或登录
	local friendData = FriendManager:GetOrNewFriendInfo(uid)

	friendData:SetOnline()
    friendData:ClearLastLogoutTime()

    friendData.gameid = zonetask.GetGameId()
    friendData.zoneid = zonetask.GetZoneId()
    friendData.last_gameid = 0
    friendData.last_zoneid = 0

	--同步玩家数据到好友数据
	friendData:SetStar(cmd.data.userInfo.star)
	friendData:SetMoney(cmd.data.userInfo.money)
	friendData:SetProduct(cmd.data.userInfo.product)
end