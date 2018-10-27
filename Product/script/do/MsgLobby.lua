-- 登录获取个人信息
Net.CmdUserInfoSynLobbyCmd_C = function(cmd, laccount)
	unilight.debug("收到获取个人信息")

	local uid 		= laccount.Id
	local lobbyId 	= cmd.data.lobbyId
    local subPlatid = cmd.data.subPlatid
	local userdata 	= nil
	local fm = nil

	--[[
	local userdata = UserInfo.GetUserDataById(uid)
	if userdata == nil or userdata.mahjong == nil then
		unilight.warn("没有找到该玩家信息！将创建该玩家")
		userdata = UserInfo.UserRegister(uid, lobbyId)
    end

	local data = {
		userInfo = UserInfo.GetUserDataBaseInfo(userdata),
	}
	--]]
	
	--临时注释掉上面的代码，模拟用户的数据

	local userInfo = UserInfo.GetUserInfoById(uid)

	if userInfo == nil then
		local dbUser = unilight.getdata("userinfo", uid)

		if dbUser == nil then
			userInfo = UserInfo.CreateTempUserInfo(uid)
		else
			userInfo = UserInfo.CreateUserByDb(uid, dbUser)
		end

		UserInfo.GlobalUserInfoMap[uid] = userInfo
	end

	userInfo.laccount = laccount
	userInfo.taskMgr.addProgress(TaskConditionEnum.Login)

	--只同步客户端需要的数据，UserInfo下面存有服务器需要的数据

	local res = {}
	res["do"] = "Cmd.UserInfoSynLobbyCmd_S"
	res["data"] = {
		resultCode = 0,
		userInfo = UserInfo.GetClientData(userInfo)
	}

	return res

	-- 消息回复
	--RoomInfo.SendCmdToUser("Cmd.UserInfoSynLobbyCmd_S", data, laccount)
end

function Net.CmdPing_C(cmd, laccount)
	--[
	local res = {}
	res["do"] = "Cmd.Ping_S"
	res["data"] = {
		resultCode = 0,
	}

	--local userInfo = UserInfo.GetUserInfoById(laccount.Id)
	--message.give(11947586, userInfo, MsgTypeEnum.FriendApply)

	return res
	--]
end
