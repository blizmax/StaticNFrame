

Zone.CmdBuildingLevelup_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

	local friendInfo = FriendManager:GetFriendInfo(uid)
	if friendInfo ~= nil then
		friendInfo.simpleData.star = userInfo.star
	end

	RankListMgr:UpdateRankNode(RankListMgr.rank_type_star, uid, userInfo.star)
end

Zone.CmdBuildingReBuild_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
end

Zone.CmdUserBuyBuild_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

	local friendInfo = FriendManager:GetFriendInfo(uid)
	if friendInfo ~= nil then
		friendInfo.simpleData.star = userInfo.star
		friendInfo.simpleData.money = userInfo.money
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
		friendInfo.simpleData.click = userInfo.click
		RankListMgr:UpdateRankNode(RankListMgr.rank_type_click, uid, friendInfo.simpleData.click)
	end
end

Zone.CmdSaveSettingsCmd_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local userInfo = cmd.data.userInfo

    	--暂时保存一份在好友系统里, 以后可能要做别的处理
	local friendData = FriendManager:GetOrNewFriendInfo(uid)
	friendData:SetUserSimpleData(userInfo.star, userInfo.gender, userInfo.signature, userInfo.area, userInfo.horoscope)
end