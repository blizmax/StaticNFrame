

-- 获取玩家邀请到的好友信息
Net.CmdGetInviteFriendInfoCmd_C = function(cmd, laccount)
	local res = {}
	res["do"] = "Cmd.GetInviteFriendInfoCmd_S"
	local uid = laccount.Id
	local code, desc, inviteFriendsInfo, progressRewarded, askFriendFiveReward = InviteFriendsMgr.GetInviteFriendsInfo(uid)

	res["data"] = {
		resultCode 	= code,
		desc 		= desc,
		data 	    = inviteFriendsInfo,
		progressRewarded = progressRewarded,
		askFriendFiveReward = askFriendFiveReward,	--0没有领取，1已经领取
	}
	return res
end

-- 获取5人领取奖励
Net.CmdGetAskFriendFiveReward_C = function(cmd, laccount)
	local res = {}
	res["do"] = "Cmd.GetAskFriendFiveReward_S"
	local uid = laccount.Id
	local friendData = FriendManager:GetOrNewFriendInfo(uid)

	local tmp = friendData:GetMeAskPlayerUidsAndFirstLogin()
	if #tmp >= GlobalConst.Invitation_Role_Times and friendData:GetAskFriendFiveReward() == 0 then
		local travelData = friendData:GetUserTravel()
		local data = travelHead[GlobalConst.Invitation_Role_Character]
		if data ~= nil then
			travelData:AddTravelHeadBackup(data.head)

			local req = {}
			req["do"] = "Cmd.NotifyAddUserTravelHead_S"
			req["data"] = {
				head = data.head,
			}
			unilight.success(laccount, req)
		end

		friendData:SetAskFriendFiveReward()
	else
		res["data"] = {
			resultCode = 1,
			desc = "",
		}
	end

	res["data"] = {
		resultCode = 0,
		desc = "",
	}
	return res
end

 --领取 邀请好友 获得的奖励
Net.CmdGetInviteFriendRewardCmd_C = function(cmd, laccount)
	local res = {}
	res["do"] = "Cmd.GetInviteFriendRewardCmd_S"
	local uid = laccount.Id
	local friendUid = cmd.data.friendUid
	print("CmdGetInviteFriendRewardCmd_C, uid="..uid..", friendUid="..friendUid)
	local  ret, desc, rewardId = InviteFriendsMgr.GetInviteFriendReward(uid, friendUid)

	res["data"] = {
		resultCode 	= ret,
		desc 		= desc,
		rewardId 	= rewardId,
	}
	return res
end


--领取 邀请好友进度 获得的奖励
Net.CmdGetProgressRewardCmd_C = function(cmd, laccount)
	local res = {}
	res["do"] = "Cmd.GetProgressRewardCmd_S"
	local uid = laccount.Id
	local progressRewardId = cmd.data.progressRewardId
	local  ret, desc = InviteFriendsMgr.GetProgressReward(uid, progressRewardId)

	res["data"] = {
		resultCode 	= ret,
		desc 		= desc,
		progressRewardId 	= progressRewardId,
	}
	return res
end