

-- 客户端获得旅行团信息
Net.CmdGetUserTravelInfo_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.GetUserTravelInfo_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"] = {cmd_uid = uid,}
    res["data"].level = travelData:GetLevel()
    res["data"].head = travelData:GetTravelHead()
    res["data"].capture_times = travelData:GetCaptureTimes()
    res["data"].unlock_slot_count = travelData:GetUnlockSlotCount() - travelData:GetMemberCount()
    res["data"].today_buy_capture_times = travelData:GetTodayBuyCaptureTimes()
    res["data"].anger = travelData:GetAnger()
    res["data"].anger_click_count = travelData:GetAngerClickCount()
    res["data"].head_backup = {}

    local head_backup = travelData:GetTravelHeadBackup()
    head_backup:ForEach(
        function(k,v)
            table.insert(res["data"].head_backup, k)
        end
    )

    res["data"].member = {}

    --轮询收集每个旅行团成员的数据
    travelData:MembersForEach(
        function(m_uid, m_time)
            if m_time + static_const.Static_Const_TRAVEL_Employ_MAX_TIME > os.time() then
                local member = { }
                member.uid = m_uid
                if m_uid == static_const.Static_Const_Friend_Travel_GOLD_GUEST_UID then
                    member.head = travelData:GetTravelHead()
                    member.name = static_const.Static_Const_Friend_Travel_GOLD_GUEST_NAME
                    member.star =  friendData:GetStar()
                    member.sex =  friendData:GetSex()
                    member.signature = friendData:GetSignature()
                    member.area =  friendData:GetArea()
                    member.horoscope =  friendData:GetHoroscope()
                    member.relation_ship = 0
                    member.travel_level = 0
                    member.level_time = (static_const.Static_Const_TRAVEL_Employ_MAX_TIME+m_time) - os.time()
                    table.insert(res["data"].member, member)
                else
                    local member_friendData = FriendManager:GetFriendInfo(m_uid); 
                    if member_friendData ~= nil then
                        local member_travelData = member_friendData:GetUserTravel()
                        member.head = member_travelData:GetTravelHead()
                        member.name = member_friendData:GetName()
                        member.star =  member_friendData:GetStar() 
                        member.sex =  member_friendData:GetSex() 
                        member.signature =  member_friendData:GetSignature()
                        member.area =  member_friendData:GetArea()
                        member.horoscope =  member_friendData:GetHoroscope()
                        member.relation_ship = travelData:GetRelationShip(m_uid)
                        member.travel_level = member_travelData:GetLevel()
                        member.level_time = (static_const.Static_Const_TRAVEL_Employ_MAX_TIME+m_time) - os.time()
                        table.insert(res["data"].member, member)
                    end
                end
            end
        end
    )

    return res
end

Lby.CmdGetUserTravelInfo_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 打开好友雇佣界面信息
Net.CmdGetTravelEmployFriend_C = function(cmd, laccount)
    local uid = laccount.Id

    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdGetTravelEmployFriend_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 打开推荐雇佣界面信息
Net.CmdGetTravelEmployRecommend_C = function(cmd, laccount)
    local uid = laccount.Id

    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdGetTravelEmployRecommend_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 雇佣或抓捕玩家
Net.CmdEmployFriendToTravel_C = function(cmd, laccount)
    local uid = laccount.Id

    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    cmd.data.cmd_uid = uid
    cmd.data.cur_money = userInfo.money
    cmd.data.cur_diamond = userInfo.diamond
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdEmployFriendToTravel_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

Lby.CmdNotifyUserBuyShieldCount_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

Lby.CmdNotifyUserTravelCapture_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 清楚雇佣CD时间
Net.CmdClearEmployFriendCD_C = function(cmd, laccount)
    local uid = laccount.Id

    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    cmd.data.cmd_uid = uid
    cmd.data.cur_money = userInfo.money
    cmd.data.cur_diamond = userInfo.diamond
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdClearEmployFriendCD_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 解除雇佣关系
Net.CmdRescissionEmployFriendShip_C = function(cmd, laccount)
    local uid = laccount.Id

    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdRescissionEmployFriendShip_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 团长升级
Net.CmdUserTravelLevelUp_C = function(cmd, laccount)
    local uid = laccount.Id

    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    cmd.data.cmd_uid = uid
    cmd.data.cur_money = userInfo.money
    cmd.data.cur_diamond = userInfo.diamond
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdUserTravelLevelUp_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end


--更改玩家的旅行团头像
Net.CmdChangeUserTravelHead_C = function(cmd, laccount)
    local uid = laccount.Id

    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdChangeUserTravelHead_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

--怒气值满了，点击
Net.CmdReleaseTravelAnger_C = function(cmd, laccount)
    local uid = laccount.Id

    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdReleaseTravelAnger_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end


--怒气值满了，点击后看视屏回调
Net.CmdReleaseAngerSeeSceen_C = function(cmd, laccount)
    local uid = laccount.Id

    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdReleaseAngerSeeSceen_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

