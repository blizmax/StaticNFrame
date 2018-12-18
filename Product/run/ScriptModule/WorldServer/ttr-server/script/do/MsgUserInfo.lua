

Zone.CmdBuildingLevelup_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

	local friendInfo = FriendManager:GetFriendInfo(uid)
	if friendInfo ~= nil then
		friendInfo.simpleData.star = userInfo.star

		local friendvisitData = friendInfo:GetFriendVisit()
		if friendvisitData:GetCurMapId() == userInfo.mapid then
			friendvisitData:SetLevel(userInfo.buildid, userInfo.lv)
		end
	end

	RankListMgr:UpdateRankNode(RankListMgr.rank_type_star, uid, userInfo.star)
end

Zone.CmdBuildingReBuild_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

	local friendInfo = FriendManager:GetFriendInfo(uid)
	if friendInfo ~= nil then
		local friendvisitData = friendInfo:GetFriendVisit()
		if friendvisitData:GetCurMapId() == userInfo.mapid then
			friendvisitData:SetLevel(userInfo.buildid, userInfo.lv)
		end
	end
end

Zone.CmdUserBuyBuild_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

	local friendInfo = FriendManager:GetFriendInfo(uid)
	if friendInfo ~= nil then
		friendInfo.simpleData.star = userInfo.star
		friendInfo.simpleData.money = userInfo.money

        local friendvisitData = friendInfo:GetFriendVisit()
		if friendvisitData:GetCurMapId() == userInfo.mapid then
			friendvisitData:AddBuild(userInfo.buildid, 1, 1)
		end
	end

	RankListMgr:UpdateRankNode(RankListMgr.rank_type_star, uid, userInfo.star)
	RankListMgr:UpdateRankNode(RankListMgr.rank_type_money, uid, userInfo.money)
end

Zone.CmdUserUpdate_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

	local friendData = FriendManager:GetFriendInfo(uid)

	if friendData ~= nil then
        friendData:SetProduct(userInfo.product)
        friendData:SetMoney(userInfo.money)
	--更新产量榜
        RankListMgr:UpdateRankNode(RankListMgr.rank_type_product, uid, userInfo.product)
        RankListMgr:UpdateRankNode(RankListMgr.rank_type_money, uid, userInfo.money)
	end
end

Zone.CmdUserClick_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

	local friendInfo = FriendManager:GetFriendInfo(uid)
	if friendInfo ~= nil then
		friendInfo.simpleData.click = friendInfo.simpleData.click + userInfo.click
		RankListMgr:UpdateRankNode(RankListMgr.rank_type_click, uid, friendInfo.simpleData.click)

		local travelData = friendInfo:GetUserTravel()

		for i = 0, times do
			--旅行团团长自己也算一个
			travelData:AddAnger(travelData:GetMemberCount()+1)
		end

		travelData:UpdateAngerToClient()
	end
end

Zone.CmdUserOpenMap_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

    local friendData = FriendManager:GetFriendInfo(uid)
	if friendData ~= nil then
		local friendvisitData = friendData:GetFriendVisit()
		friendvisitData:SetCurMapId(userInfo.mapid)
	end
end

Zone.CmdUserDisconnected_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    FriendManager:UserLogoutFriend(uid)
end

Zone.CmdUserReconncted_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid

	--玩家好友数据创建或登录
	local friendData = FriendManager:UserLoginFriend(uid)
	local travelData = friendData:GetUserTravel()
	local friendVisitData = friendData:GetFriendVisit()

	--登录时主动清理零点数据
	friendData:AutoZeroClear()
	travelData:CalcAddontion()
end

Zone.CmdSaveSettingsCmd_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

    	--暂时保存一份在好友系统里, 以后可能要做别的处理
	local friendData = FriendManager:GetOrNewFriendInfo(uid)
	friendData:SetUserSimpleData(userInfo.star, userInfo.gender, userInfo.signature, userInfo.area, userInfo.horoscope)
end