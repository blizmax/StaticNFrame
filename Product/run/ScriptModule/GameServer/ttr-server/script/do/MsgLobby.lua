-- 登录获取个人信息
Net.CmdUserInfoSynLobbyCmd_C = function(cmd, laccount)
	unilight.debug("收到获取个人信息")

	local uid 		= laccount.Id
	local userdata 	= nil
	local fm = nil
	--local head = cmd.data.head
	--local name = cmd.data.name
	--local sex = cmd.data.sex
	
	--临时注释掉上面的代码，模拟用户的数据

	local userInfo = UserInfo.GetUserInfoById(uid)

	local isFirstLogin = false
	if userInfo == nil then
		local dbUser = unilight.getdata("userinfo", uid)

		if dbUser == nil then
			userInfo = UserInfo.CreateTempUserInfo(uid)
			isFirstLogin = true
		else
			userInfo = UserInfo.CreateUserByDb(uid, dbUser)
		end

		UserInfo.GlobalUserInfoMap[uid] = userInfo
	end

	--清理玩家离线定时器
	if userInfo["offline_timer"] ~= nil then
		unilight.stoptimer(userInfo["offline_timer"])
	end

	--有可能需要重置每日任务数据
	userInfo.dailyTask:Reset()
	userInfo.achieveTask:LoadConfig()
	userInfo.mainTask:LoadConfig()
	--userInfo.nickName = name or userInfo.nickName
	--userInfo.head = head or userInfo.head
	--userInfo.sex = sex or userInfo.sex
	userInfo.laccount = laccount
	userInfo.dailyTask:addProgress(TaskConditionEnum.LoginEvent, 1)
	userInfo.mainTask:addProgress(TaskConditionEnum.LoginEvent, 1)

	--只同步客户端需要的数据，UserInfo下面存有服务器需要的数据

	userInfo.dailyLogin:Login()

	--处理属性重置
	UserProps:dealLoginInitProps(userInfo)

	--玩家好友数据创建或登录
	local friendData = FriendManager:UserLoginFriend(uid)
	local travelData = friendData:GetUserTravel()
	local friendVisitData = friendData:GetFriendVisit()

	--登录时主动清理零点数据
	friendData:AutoZeroClear()

	--保存一次玩家互访的数据，并把互访的钱给玩家
	friendVisitData:Rebuild(userInfo)

	--同步玩家数据到好友数据
	friendData:SetStar(userInfo.star)
	friendData:SetMoney(userInfo.money)
	travelData:CalcAddontion()

	if userInfo.firstLogin ~= 1 then
		UserInfo.DealOfflinePrize(userInfo, false, 0)
	end

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
	userInfo.dailyDiamondReward:DealWithLogin()
	userInfo.dailyLotteryDraw:DealWithLogin()
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
