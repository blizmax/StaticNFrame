-- 登录获取个人信息
Net.CmdUserInfoSynLobbyCmd_C = function(cmd, laccount)
	local uid 		= laccount.Id

	local userInfo = UserInfo.GetOrNewUserInfo(uid)
	local friendData = FriendManager:UserLoginFriend(uid)

	userInfo.online = true
	userInfo.laccount = laccount

	local isFirstLogin = false
	if userInfo.firstLogin == 1 then
		isFirstLogin = true
	end

	local startLoginReq = {}
	startLoginReq["do"] = "Cmd.UserInfoLoginCenter_C"
	startLoginReq["data"] = {
		cmd_uid = uid,
	}
	unilobby.SendCmdToLobby(startLoginReq["do"], startLoginReq["data"])
end

Lby.CmdUserInfoLoginCenter_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
	local userInfo = UserInfo.GetOrNewUserInfo(uid)

	local isFirstLogin = false
	if userInfo.firstLogin == 1 then
		isFirstLogin = true
	end

	userInfo.online = true

	--清理玩家离线定时器
	if userInfo["offline_timer"] ~= nil then
		unilight.stoptimer(userInfo["offline_timer"])
	end

	if userInfo["savedb_timer"] == nil then
		userInfo["savedb_timer"] = unilight.addtimer("UserInfo.SaveOneUserInfoToDB",static_const.Static_Const_User_Save_Data_DB_Time, userInfo.uid)
	end

	if userInfo["update_user_sec_timer"] == nil then
		userInfo["update_user_sec_timer"] = unilight.addtimer("UserInfo.UpdateUserSec", 1, userInfo.uid)
	end

	--有可能需要重置每日任务数据
	userInfo.dailyTask:Reset()
	userInfo.achieveTask:LoadConfig()
	userInfo.mainTask:LoadConfig()
	--userInfo.nickName = name or userInfo.nickName
	--userInfo.head = head or userInfo.head
	--userInfo.sex = sex or userInfo.sex

	userInfo.dailyTask:addProgress(TaskConditionEnum.LoginEvent, 1)
	userInfo.mainTask:addProgress(TaskConditionEnum.LoginEvent, 1)

	--只同步客户端需要的数据，UserInfo下面存有服务器需要的数据

	--处理属性重置
	UserProps:dealLoginInitProps(userInfo)

	if userInfo.firstLogin ~= 1 then
		UserInfo.DealOfflinePrize(userInfo, false, 0)
	end

	local friendData = FriendManager:UserLoginFriend(uid)
	local travelData = friendData:GetUserTravel()

	--登录时主动清理零点数据
	friendData:AutoZeroClear()

	travelData:CalcAddontion()

	userInfo.friendAddontion = friendData:GetAddontion()

	friendData.isFirstLogin = userInfo.firstLogin

	--玩家产量初始化计算 依赖好友系统的加成计算
	userInfo.world:recalc()

	local res = {}
	res["do"] = "Cmd.UserInfoSynLobbyCmd_S"
	res["data"] = {
		resultCode = 0,
		userInfo = UserInfo.GetClientData(userInfo),
		is_first_login = isFirstLogin,
		shield_count = travelData:GetShieldCount(),
		user_props = UserProps:GetUserProps(userInfo),
	}
	unilight.response(userInfo.laccount, res)

	--属性同步
	--UserProps:sendUserProps(userInfo)

	--下发广告信息  属性那块做好后 这里可以不发
	UserProps:sendUserLookMediaInfo(userInfo)
	--下发在线奖励信息
	UserProps:sendOnlineRandBoxInfo(userInfo)
	
	userInfo.dailyLogin:DealWithLogin()
	userInfo.dailySharing:DealWithLogin()
	userInfo.dailyDiamondReward:DealWithLogin()
	userInfo.dailyLotteryDraw:DealWithLogin()
	userInfo.dailyWelfare:DealWithLogin()
end

function Net.CmdPing_C(cmd, laccount)
	--[
	local res = {}
	res["do"] = "Cmd.Ping_S"
	res["data"] = {
		resultCode = 0,
	}

	return res
end
