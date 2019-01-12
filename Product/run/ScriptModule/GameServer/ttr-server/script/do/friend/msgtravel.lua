

-- 客户端获得旅行团信息
Net.CmdGetUserTravelInfo_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.GetUserTravelInfo_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    local tmp = {}

    --查找有没有别的服务器的数据成员，有的话，就去中心服务器里取数据
    travelData:MembersForEach(
        function(m_uid, m_time)
            if m_uid == static_const.Static_Const_Friend_Travel_GOLD_GUEST_UID then
            else
                local member_friendData = FriendManager:GetFriendInfo(m_uid); 
                if member_friendData == nil then
                    local tmp_data = {
                        m_uid = m_uid,
                        relation_ship = travelData:GetRelationShip(m_uid),
                        level_time = (static_const.Static_Const_TRAVEL_Employ_MAX_TIME+m_time) - os.time()
                    }
                    table.insert(tmp, tmp_data)
                end
            end
        end
    )

    if #tmp > 0 then
        cmd.data.cmd_uid = uid
        cmd.data.tmp = tmp
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
        return
    end

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
    )

    return res
end

Lby.CmdGetUserTravelInfo_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local res = cmd
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

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

    res["data"].member = res["data"].member or {}

    --轮询收集每个旅行团成员的数据
    travelData:MembersForEach(
        function(m_uid, m_time)
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
    )

    unilight.response(userInfo.laccount, res)
end

-- 打开好友雇佣界面信息
Net.CmdGetTravelEmployFriend_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.GetTravelEmployFriend_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"] = {cmd_uid = uid,}
    res["data"].member = {}

    local tmp = {}

    --轮询好友信息， 筹集数据
    friendData:UserFriendsForEach(
        function(f_uid, f_info)
            if travelData:IsExistMembers(f_uid) == false and f_uid ~= uid then
                local f_friendData = FriendManager:GetFriendInfo(f_uid)
                if f_friendData == nil then
                    local tmp_data = {}
                    tmp_data.uid = f_uid
                    tmp_data.relation_ship = travelData:GetRelationShip(f_uid)
                    table.insert(tmp, tmp_data)
                end
            end
        end
    )

    if #tmp > 0 then
        cmd.data.cmd_uid = uid
        cmd.data.tmp = tmp
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
        return
    end

    --轮询好友信息， 筹集数据
    friendData:UserFriendsForEach(
        function(f_uid, f_info)
            if travelData:IsExistMembers(f_uid) == false and f_uid ~= uid then
                local f_friendData = FriendManager:GetFriendInfo(f_uid)
                if f_friendData ~= nil then
                    local f_travelData = f_friendData:GetUserTravel()
                    local tmp = {}
                    tmp.uid = f_uid
                    tmp.head = f_travelData:GetTravelHead()
                    tmp.name = f_friendData:GetName()
                    tmp.star =  f_friendData:GetStar()
                    tmp.sex =  f_friendData:GetSex()
                    tmp.signature =  f_friendData:GetSignature()
                    tmp.area =  f_friendData:GetArea()
                    tmp.horoscope =  f_friendData:GetHoroscope()
                    tmp.travel_level = f_travelData:GetLevel()
                    tmp.relation_ship = f_travelData:GetRelationShip(uid)
                    tmp.cur_employ_uid = f_travelData:GetEmployUid()
                    tmp.cur_employ_name = f_travelData:GetEmployName()
                    tmp.employ_cd = 0

                    --这说明你不久前雇佣过对方
                    if uid == f_travelData:GetLastEmployUid() then
                        tmp.employ_cd = f_travelData:GetEmployCdLeftTime()
                    else
                        tmp.employ_cd = 0
                    end
                    table.insert(res["data"].member, tmp)     
                end
            end
        end
    )

    return res
end

Lby.CmdGetTravelEmployFriend_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    local res = cmd

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"].member = res["data"].member or {}


    --轮询好友信息， 筹集数据
    friendData:UserFriendsForEach(
        function(f_uid, f_info)
            if travelData:IsExistMembers(f_uid) == false and f_uid ~= uid then
                local f_friendData = FriendManager:GetFriendInfo(f_uid)
                if f_friendData ~= nil then
                    local f_travelData = f_friendData:GetUserTravel()
                    local tmp = {}
                    tmp.uid = f_uid
                    tmp.head = f_travelData:GetTravelHead()
                    tmp.name = f_friendData:GetName()
                    tmp.star =  f_friendData:GetStar()
                    tmp.sex =  f_friendData:GetSex()
                    tmp.signature =  f_friendData:GetSignature()
                    tmp.area =  f_friendData:GetArea()
                    tmp.horoscope =  f_friendData:GetHoroscope()
                    tmp.travel_level = f_travelData:GetLevel()
                    tmp.relation_ship = f_travelData:GetRelationShip(uid)
                    tmp.cur_employ_uid = f_travelData:GetEmployUid()
                    tmp.cur_employ_name = f_travelData:GetEmployName()
                    tmp.employ_cd = 0

                    --这说明你不久前雇佣过对方
                    if uid == f_travelData:GetLastEmployUid() then
                        tmp.employ_cd = f_travelData:GetEmployCdLeftTime()
                    else
                        tmp.employ_cd = 0
                    end
                    table.insert(res["data"].member, tmp)     
                end
            end
        end
    )

    unilight.response(userInfo.laccount, res)
end

-- 打开推荐雇佣界面信息
Net.CmdGetTravelEmployRecommend_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.GetTravelEmployRecommend_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"] = {cmd_uid = uid,}
    res["data"].member = {}

    --首先放金牌客户
    if travelData:IsExistMembers(static_const.Static_Const_Friend_Travel_GOLD_GUEST_UID) == false then
        local tmp = {}
        tmp.uid = static_const.Static_Const_Friend_Travel_GOLD_GUEST_UID
        tmp.head = travelData:GetTravelHead()
        tmp.name = static_const.Static_Const_Friend_Travel_GOLD_GUEST_NAME
        tmp.star =  friendData:GetStar()
        tmp.sex =  friendData:GetSex()
        tmp.signature =  friendData:GetSignature()
        tmp.area =  friendData:GetArea()
        tmp.horoscope =  friendData:GetHoroscope()
        tmp.travel_level = 0
        tmp.relation_ship = 0
        tmp.cur_employ_uid = 0
        tmp.cur_employ_name = ""
        tmp.employ_cd = 0

        table.insert(res["data"].member, tmp)
    end

    --选择条件匹配的可推荐对象
    --玩家不能已经被雇佣
    --玩家不能是好友
    --玩家等级必须小
    --不能再CD时间里
    local count = 0
    for f_uid, f_friendData in pairs(FriendManager.userFriend.map) do
        if travelData:IsExistMembers(f_uid) == false and friendData:GetUserFriend(f_uid) == nil and f_uid ~= uid then
            local f_travelData = f_friendData:GetUserTravel()
            if travelData:GetLevel() >= f_travelData:GetLevel() and f_travelData:GetEmployUid() == 0 then
                --这说明你不久前雇佣过对方, 不在CD时间里
                if uid ~= f_travelData:GetLastEmployUid() or f_travelData:GetEmployCdLeftTime() == 0 then
                    local tmp = {}
                    tmp.uid = f_uid
                    tmp.head = f_travelData:GetTravelHead()
                    tmp.name = f_friendData:GetName()
                    tmp.star =  f_friendData:GetStar()
                    tmp.sex =  f_friendData:GetSex()
                    tmp.signature =  f_friendData:GetSignature()
                    tmp.area =  f_friendData:GetArea()
                    tmp.horoscope =  f_friendData:GetHoroscope()
                    tmp.travel_level = f_travelData:GetLevel()
                    tmp.relation_ship = f_travelData:GetRelationShip(uid)
                    tmp.cur_employ_uid = f_travelData:GetEmployUid()
                    tmp.cur_employ_name = f_travelData:GetEmployName()
                    tmp.employ_cd = 0

                    table.insert(res["data"].member, tmp)

                    if #res["data"].member >= static_const.Static_Const_TRAVEL_MAX_RECOMMEND_COUNT then
                        return res
                    end
                end
            end
        end
    end

    return res
end

-- 雇佣或抓捕玩家
Net.CmdEmployFriendToTravel_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.EmployFriendToTravel_S"

    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local userInfo = UserInfo.GetUserInfoById(uid)
    if userInfo == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    if travelData:GetMemberCount() >= travelData:GetUnlockSlotCount() then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.TRAVEL_NO_POS,
            desc = "没有更多位置了，需解锁新位置"
        }
        return res
    end

    local employ_uid = cmd["data"].uid

    if employ_uid == uid then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    if travelData:IsExistMembers(employ_uid) == true then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.TRAVEL_CANNOT_EMPLY_TWICE,
            desc = "已经被你雇佣过了"
        }
        return res
    end

    --金牌客户处理
    if employ_uid == static_const.Static_Const_Friend_Travel_GOLD_GUEST_UID then
        --设置抓捕信息
        travelData:AddMember(employ_uid)
        travelData:StartEmployEndTimer(employ_uid)

        --从新计算一次旅行团加成
        travelData:CalcAddontion()

        userInfo.achieveTask:addProgress(TaskConditionEnum.EmployFriendEvent, 1)
        userInfo.dailyTask:addProgress(TaskConditionEnum.EmployFriendEvent, 1)
        userInfo.mainTask:addProgress(TaskConditionEnum.EmployFriendEvent, 1)

        res["data"] = {
            cmd_uid = uid,
            resultCode = 0,
            desc = "雇佣成功",
            capture_times = travelData:GetCaptureTimes(),
            today_buy_capture_times = travelData:GetTodayBuyCaptureTimes(),
            member = {
                uid = employ_uid,
                head = travelData:GetTravelHead(),
                name = static_const.Static_Const_Friend_Travel_GOLD_GUEST_NAME,
                star =  friendData:GetStar(),
                sex =  friendData:GetSex(),
                signature =  friendData:GetSignature(),
                area =  friendData:GetArea(),
                horoscope =  friendData:GetHoroscope(),
                travel_level = 0,
                relation_ship = 0,
                level_time = static_const.Static_Const_TRAVEL_Employ_MAX_TIME,
            },
            type = 0,
            isCapture = false,
        }

        return res
    end

    cmd.data.cmd_uid = uid
    cmd.data.isfriend = false
    if friendData:GetUserFriend(employ_uid) ~= nil then
        cmd.data.isfriend = true
    end
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdNotifyTravelMemberBeCapture_C = function(cmd, lobbyClientTask)
    local uid = cmd.data.uid
    local last_uid = cmd.data.cmd_uid
    local employ_uid = cmd.data.employ_uid

    local req = {}
    req["do"] = "Cmd.NotifyTravelMemberBeCapture_S"
    req["data"] = {
        cmd_uid = uid,
        employ_uid = employ_uid,
        last_uid = last_uid,
    }

    local last_friendData = FriendManager:GetOrNewFriendInfo(last_uid);
    if last_friendData ~= nil then
        local last_travelData = last_friendData:GetUserTravel()

        if last_travelData:GetShieldCount() > 0 then
            last_travelData:SubShieldCount()

            last_friendData:Give(last_friendData, MsgTypeEnum.FriendRobbedWithFailure)

            do
                local last_userInfo = UserInfo.GetOfflineUserInfo(last_uid)
                if last_userInfo ~= nil then
                    last_userInfo.dailyTask:addProgress(TaskConditionEnum.StopCaptureEvent, 1)
                    last_userInfo.mainTask:addProgress(TaskConditionEnum.StopCaptureEvent, 1)
                end
            end

            if last_friendData:GetOnline() == true then
                --在线的话通知对方
                local notify_req = {}
                notify_req["do"] = "Cmd.NotifyUserBuyShieldCount_S"
                notify_req["data"] = {
                    cmd_uid = last_uid,
                    shield_count = last_travelData:GetShieldCount(),
                }

                UserInfo.SendInfoByUid(last_uid, notify_req)
            end

            req["data"].has_shield = true
            unilobby.SendCmdToLobby(req["do"], req["data"])
            return
        end

        req["data"].has_shield = false

        last_travelData:DelMember(employ_uid)
        --先干掉定时器
        last_travelData:StopEmployEndTimer(employ_uid)

        --从新计算被抢了旅行团成员的旅行团加成
        last_travelData:CalcAddontion()

        --抓捕了对方好友，通知下对方
        --last_friendData:Give(friendData, MsgTypeEnum.FriendRobbed)

        do
            local last_userInfo = UserInfo.GetOfflineUserInfo(last_uid)
            if last_userInfo ~= nil then
                last_userInfo.dailyTask:addProgress(TaskConditionEnum.CaptureFriendEvent, 1)
                last_userInfo.mainTask:addProgress(TaskConditionEnum.CaptureFriendEvent, 1)
            end
        end

        if last_friendData:GetOnline() == true then
            --在线的话通知对方
            local req = { }
            req["do"] = "Cmd.NotifyUserTravelCapture_S"
            req["data"] = {
                cmd_uid = last_uid, 
                uid = employ_uid,
            }
            UserInfo.SendInfoByUid(last_uid, req)
        end

        unilobby.SendCmdToLobby(req["do"], req["data"])
        return
    end
end

Lby.CmdEmployFriendToTravel_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local employ_uid = cmd.data.employ_uid
    
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    cmd.data.capture_times = travelData:GetCaptureTimes()
    cmd.data.today_buy_capture_times = travelData:GetTodayBuyCaptureTimes()

    unilight.response(userInfo.laccount, cmd)
end

Lby.CmdNotifyCaptureFriend = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local employ_uid = cmd.data.employ_uid
    local last_uid = cmd.data.last_uid

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    --查看玩家当前抓捕次数
    if travelData:GetCaptureTimes() <= 0 then
        --扣钱和比较钱的操作
        if UserInfo.CheckUserMoney(userInfo, static_const.Static_ItemType_Diamond, travelData:GetTodayBuyCaptureTimes_NeedCost()) == false then
            local res = {}
            res["do"] = "Cmd.EmployFriendToTravel_S"
            res["data"] = {
                cmd_uid = uid,
                resultCode = ERROR_CODE.DIAMOND_NOT_ENOUGH,
                desc = "你的砖石不够",
                capture_times = travelData:GetCaptureTimes(),
                today_buy_capture_times = travelData:GetTodayBuyCaptureTimes(),
            }
            unilight.response(userInfo.laccount, res)
        end

        UserInfo.SubUserMoney(userInfo, static_const.Static_ItemType_Diamond, travelData:GetTodayBuyCaptureTimes_NeedCost())
        travelData:AddTodayBuyCaptureTimes()

        userInfo.dailyTask:addProgress(TaskConditionEnum.CaptureFriendEvent, 1)
        userInfo.mainTask:addProgress(TaskConditionEnum.CaptureFriendEvent, 1)
    end

    --减少一次抓捕次数
    travelData:DecCaptureTimes()

    do
        local req = {}
        req["do"] = "Cmd.NotifyTravelMemberBeCapture_C"
        req["data"] = {
            cmd_uid = last_uid,
            employ_uid = employ_uid,
            uid = uid,
        }
        unilobby.SendCmdToLobby(req["do"], req["data"])
    end
end

Lby.CmdNotifyBeEmployedSuccess = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local name = cmd.data.name
    local employ_uid = cmd.data.employ_uid
    
    local employ_friendData = FriendManager:GetOrNewFriendInfo(employ_uid);
    local employ_travelData = employ_friendData:GetUserTravel()

    --设置抓捕信息
    employ_travelData:SetEmployUid(uid)
    employ_travelData:SetEmployName(name)
end 

Lby.CmdNotifyEmployFriendSuccess = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local employ_uid = cmd.data.employ_uid

    local res = {}
    res["do"] = "Cmd.EmployFriendToTravel_S"
    res["data"] = cmd.data

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    if cmd.data.resultCode == 0 then
        travelData:AddMember(employ_uid)
        travelData:StartEmployEndTimer(employ_uid)

        --从新计算一次旅行团加成
        travelData:CalcAddontion()

        --任务系统，任务完成情况
        if cmd.data.isCapture == false then
            userInfo.achieveTask:addProgress(TaskConditionEnum.EmployFriendEvent, 1)
            userInfo.dailyTask:addProgress(TaskConditionEnum.EmployFriendEvent, 1)
            userInfo.mainTask:addProgress(TaskConditionEnum.EmployFriendEvent, 1)
        end

        if cmd.data.member ~= nil then
            cmd.data.member.relation_ship = travelData:GetRelationShip(employ_uid)
        end
    end

    res.data.capture_times = travelData:GetCaptureTimes()
    res.data.today_buy_capture_times = travelData:GetTodayBuyCaptureTimes()

    unilight.response(userInfo.laccount, res)
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

    local res = { }
    res["do"] = "Cmd.ClearEmployFriendCD_S"

    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local userInfo = UserInfo.GetUserInfoById(uid)
    if userInfo == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    
    local employ_uid = cmd["data"].uid

    --判断是否已经被你雇佣
    if travelData:IsExistMembers(employ_uid) == true then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.TRAVEL_CANNOT_EMPLY_TWICE,
            desc = "已经被你雇佣过了"
        }
        return res
    end

    if employ_uid == static_const.Static_Const_Friend_Travel_GOLD_GUEST_UID then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 0,
            desc = "清理成功",
            uid = employ_uid,
        }
        return res
    end

    --扣钱和比较钱的操作
    if UserInfo.CheckUserMoney(userInfo, static_const.Static_MoneyType_Diamond, GlobalConst.Travel_CD_Diamond) == false then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.DIAMOND_NOT_ENOUGH,
            desc = "你的砖石不够"
        }
        return res
    end

    --判断对方是否存在
    local employ_friendData = FriendManager:GetFriendInfo(employ_uid)
    if employ_friendData == nil then
        cmd.data.cmd_uid = uid
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
        return
    end

    local employ_travelData = employ_friendData:GetUserTravel()

    --判断当前等级
    if employ_travelData:GetLevel() > travelData:GetLevel() then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.TRAVEL_LEVEL_LIMIT,
            desc = "该好友等级高过你，不能雇佣（抓捕）"
        }
        return res
    end

    --如果需要清理的话
    if employ_travelData:GetLastEmployUid() == uid then
        if employ_travelData:GetEmployCdLeftTime() > 0 then
            UserInfo.SubUserMoney(userInfo, static_const.Static_MoneyType_Diamond, GlobalConst.Travel_CD_Diamond)
            --
            employ_travelData:SetLastEmployUid(0)
            employ_travelData:ClearEmployCd()

            do
                local req = {}
                req["do"] = "Cmd.NofiyClearEmployCd"
                req["data"] = {
                    cmd_uid = employ_uid,
                }
                unilobby.SendCmdToLobby(req["do"], req["data"])
            end
        end
    end

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "清理成功",
        uid = employ_uid,
    }
    return res
end

Lby.CmdClearEmployFriendCD_f_S = function(cmd, lobbyClientTask)
    local employ_uid = cmd.data.cmd_uid
    local res = {}
    res["do"] = "Cmd.ClearEmployFriendCD_S"
    
    --判断对方是否存在
    local employ_friendData = FriendManager:GetOrNewFriendInfo(employ_uid)
    
    local employ_travelData = employ_friendData:GetUserTravel()
    
    --如果需要清理的话
    if employ_travelData:GetLastEmployUid() == cmd.data.uid then
        if employ_travelData:GetEmployCdLeftTime() > 0 then
            employ_travelData:SetLastEmployUid(0)
            employ_travelData:ClearEmployCd()

            do
                local req = {}
                req["do"] = "Cmd.NofiyClearEmployCd"
                req["data"] = {
                    cmd_uid = employ_uid,
                }
                unilobby.SendCmdToLobby(req["do"], req["data"])
            end
        end
    end

    res["data"] = {
        cmd_uid = cmd.data.uid,
        resultCode = 0,
        desc = "清理成功",
        uid = employ_uid,
    }
    unilobby.SendCmdToLobby(res["do"], res["data"])
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

    local res = { }
    res["do"] = "Cmd.RescissionEmployFriendShip_S"

    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    local employ_uid = cmd["data"].uid

    --判断是否已经被你雇佣
    if travelData:IsExistMembers(employ_uid) == false then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    travelData:DelMember(employ_uid)

    --先干掉定时器
    travelData:StopEmployEndTimer(employ_uid)
    --从新计算一次旅行团加成
    travelData:CalcAddontion()

    if employ_uid == static_const.Static_Const_Friend_Travel_GOLD_GUEST_UID then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 0,
            desc = "",
            uid = employ_uid,
        }

        return res
    end

    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        uid = employ_uid,
    }

    return res
end

Lby.CmdRescissionEmployFriendShip_C = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local rescission_uid = cmd.data.rescission_uid
    
    --判断对方是否存在
    local employ_friendData = FriendManager:GetOrNewFriendInfo(uid)
    local employ_travelData = employ_friendData:GetUserTravel()

    employ_travelData:SetEmployUid(0)
    employ_travelData:SetEmployName("")
end

-- 团长升级
Net.CmdUserTravelLevelUp_C = function(cmd, laccount)
    local uid = laccount.Id

    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    local res = { }
    res["do"] = "Cmd.UserTravelLevelUp_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    local level = travelData:GetLevel()

    --升级自身需要的条件traveLevel
    local need = traveLevel.query(level+1)
    if need ~= nil then
        if friendData:GetStar() < need.star then
            res["data"] = {
                cmd_uid = uid,
                resultCode = ERROR_CODE.TRAVEL_STAR_NOT_ENOUGH,
                desc = "星级不够"
            }
            return res
        end

        local money_table = string.split(need.cost, "_")
        local money_type, money =  money_table[1], money_table[2]
        if money_type == nil or money == nil then
            unilight.error("配置表数据出错.........")
            res["data"] = {
                cmd_uid = uid,
                resultCode = 1,
                desc = "",
            }
            return res
        end

        if tonumber(money_type) == static_const.Static_MoneyType_Diamond then
            if UserInfo.CheckUserMoney(userInfo, static_const.Static_MoneyType_Diamond, tonumber(money)) == false then
                res["data"] = {
                    cmd_uid = uid,
                    resultCode = ERROR_CODE.DIAMOND_NOT_ENOUGH,
                    desc = "抱歉，你的钱不够"
                }
                return res
            end
        elseif tonumber(money_type) == static_const.Static_MoneyType_Gold then
            if UserInfo.CheckUserMoney(userInfo, static_const.Static_MoneyType_Gold, tonumber(money)) == false then
                res["data"] = {
                    cmd_uid = uid,
                    resultCode = ERROR_CODE.MONEY_NOT_ENOUGH,
                    desc = "抱歉，你的钱不够"
                }
                return res
            end
        end

        UserInfo.SubUserMoney(userInfo, tonumber(money_type), tonumber(money))
    end

    travelData:LevelUp()
    --从新计算一次旅行团加成
    travelData:CalcAddontion()

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "升级成功",
        unlock_count = 0,
    }

    --团员位置解锁的条件
    local cond = travelUnlock.query(travelData:GetUnlockSlotCount()+1)
    if cond ~= nil then
        if travelData:GetLevel() >= cond.level then
            travelData:AddUnlockSlotCount()
            res["data"].unlock_count = 1
        end
    end

    userInfo.achieveTask:addProgress(TaskConditionEnum.TravelLevelUpEvent, travelData:GetLevel())
    userInfo.mainTask:addProgress(TaskConditionEnum.TravelLevelUpEvent, 1)
    userInfo.mainTask:addProgress(TaskConditionEnum.TravelLevelValueEvent, travelData:GetLevel())

    do
        local req = {}
        req["do"] = "Cmd.NotifyTravelLevelup"
        req["data"] = {
            cmd_uid = uid,
            level = travelData:GetLevel(),
        }

        unilobby.SendCmdToLobby(req["do"], req["data"])
    end

    return res
end


--更改玩家的旅行团头像
Net.CmdChangeUserTravelHead_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.ChangeUserTravelHead_S"

    --检查客户端输入数据
    if cmd["data"] == nil or cmd["data"].head == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据错误",
        }
        return res
    end

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    local head = cmd["data"].head

    if head == travelData:GetTravelHead() then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 0,
            desc = "",
        }
        return res
    end

    if travelData:IsExistTravelHeadBackup(head) == false then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.TRAVEL_NEED_BUY_HEAD,
            desc = "这个头像需要先购买",
        }
        return res
    end

    travelData:SetTravelHead(head)

    do
        local req = {}
        req["do"] = "Cmd.NotifyTravelHeadChange"
        req["data"] = {
            cmd_uid = uid,
            head = head,
        }

        unilobby.SendCmdToLobby(req["do"], req["data"])
    end

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        head = head,
    }
    return res
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

    local res = { }
    res["do"] = "Cmd.ReleaseTravelAnger_S"

    --检查客户端数据输入
    if cmd["data"] == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local travelData = friendData:GetUserTravel()
    --玩家可能不在线,设置离线数据被该标志
    friendData:SetOfflineChange()

    if travelData:IsAngerFull() == false then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.TRAVEL_ANGER_NOT_FULL,
            desc = ""
        }
        return res
    end

    local money = friendData:GetProduct() * 30 * 1000/(800+travelData:GetAnger());

    travelData:ClearAnger()
    travelData:AddAngerClickCount()
    travelData:SetAngerClickMoney(money)

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        anger = travelData:GetAnger(),
        anger_click_count = travelData:GetAngerClickCount(),
    }

    if travelData:GetAngerClickCount() <= GlobalConst.Doublerage_Time then
        res["data"].money = money * 100
    else
        res["data"].money = money
        UserInfo.AddUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)
    end

    return res
end


--怒气值满了，点击后看视屏回调
Net.CmdReleaseAngerSeeSceen_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.ReleaseAngerSeeSceen_S"

    --检查客户端数据输入
    if cmd["data"] == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    local see_screen = cmd["data"].see_screen;
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local travelData = friendData:GetUserTravel()

    --玩家可能不在线,设置离线数据被该标志
    friendData:SetOfflineChange()

    local money = travelData:GetAngerClickMoney()

    if see_screen == true and travelData:GetAngerClickCount() <= GlobalConst.Doublerage_Time then
        money = money * 100
        UserInfo.AddUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)
    else
        UserInfo.AddUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)
    end

    
    travelData:SetAngerClickMoney(0)

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        money = money,
    }

    return res
end

