
-- 客户端获得旅行团信息
Zone.CmdGetUserTravelInfo_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.GetUserTravelInfo_S"
    res["data"] = {
        cmd_uid = cmd.data.cmd_uid
    }

    local uid = cmd.data.cmd_uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"].member = {}

    for i, tmp_data in ipairs(cmd.data.tmp) do
        local member = { }
        member.uid = tmp_data.m_uid
        
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
            member.relation_ship = tmp_data.relation_ship
            member.travel_level = member_travelData:GetLevel()
            member.level_time = tmp_data.level_time
            table.insert(res["data"].member, member)
        end
    end

    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end

-- 打开好友雇佣界面信息
Zone.CmdGetTravelEmployFriend_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.GetTravelEmployFriend_S"

    local uid = cmd.data.cmd_uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"] = {cmd_uid = uid,}
    res["data"].member = {}

    for i, tmp_data in ipairs(cmd.data.tmp) do
        local f_friendData = FriendManager:GetFriendInfo(tmp_data.uid)
        if f_friendData ~= nil then
            local f_travelData = f_friendData:GetUserTravel()
            local tmp = {}
            tmp.uid = tmp_data.uid
            tmp.head = f_travelData:GetTravelHead()
            tmp.name = f_friendData:GetName()
            tmp.star =  f_friendData:GetStar()
            tmp.sex =  f_friendData:GetSex()
            tmp.signature =  f_friendData:GetSignature()
            tmp.area =  f_friendData:GetArea()
            tmp.horoscope =  f_friendData:GetHoroscope()
            tmp.travel_level = f_travelData:GetLevel()
            tmp.relation_ship = tmp_data.relation_ship
            tmp.cur_employ_uid = f_travelData:GetEmployUid()
            tmp.cur_employ_name = f_travelData:GetEmployName()
            tmp.employ_cd = 0
            table.insert(res["data"].member, tmp)     
        end
    end

    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end

-- 雇佣或抓捕玩家
Zone.CmdEmployFriendToTravel_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.EmployFriendToTravel_S"
    local uid = cmd.data.cmd_uid
    local isfriend = cmd.data.isfriend

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    local employ_uid = cmd["data"].uid

    local employ_friendData = FriendManager:GetFriendInfo(employ_uid)
    if employ_friendData == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "对方不存在"
        }
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
        return
    end

    local employ_travelData = employ_friendData:GetUserTravel()

    if employ_travelData:GetLastEmployUid() == uid then
        if employ_travelData:GetEmployCdLeftTime() > 0 then
            res["data"] = {
                cmd_uid = uid,
                resultCode = ERROR_CODE.TRAVEL_IN_EMPLOY_CD,
                desc = "雇佣CD时间，暂时不能被你雇佣",
            }
            ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
            return
        end
    end

    if employ_travelData:GetLevel() > travelData:GetLevel() then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.TRAVEL_LEVEL_LIMIT,
            desc = "",
        }
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
        return
    end

    --玩家正在别雇佣，抓捕他们 删除原有的雇佣信息，并通知对方
    if employ_travelData:GetEmployUid() ~= 0 then

        --抓捕逻辑,抓捕次数有限制
        if isfriend == false then
            res["data"] = {
                cmd_uid = uid,
                resultCode = ERROR_CODE.TRAVEL_HAS_EMPLOYED_BY_OTHERS,
                desc = "被别的玩家雇佣",
            }
            ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
            return
        end

        do
            local req = {}
            req["do"] = "Cmd.NotifyCaptureFriend"
            req["data"] = {
                cmd_uid = uid,
                employ_uid = employ_uid,
                last_uid = employ_travelData:GetEmployUid(),
            }
            ZoneInfo.SendCmdToMe(req["do"], req["data"], zonetask)
            return
        end
    end

    --设置抓捕信息
    employ_travelData:SetEmployUid(uid)
    employ_travelData:SetEmployName(friendData:GetName())

    travelData:AddMember(employ_uid)

    do
        local req = {}
        req["do"] = "Cmd.NotifyBeEmployedSuccess"
        req["data"] = {
            cmd_uid = uid,
            name = friendData:GetName(),
            employ_uid = employ_uid,
        }
        FriendManager.SendCmdByFriend(req["do"], req["data"], employ_friendData, zonetask)
    end

    do
        local req = {}
        req["do"] = "Cmd.NotifyEmployFriendSuccess"
        req["data"] = {
            cmd_uid = uid,
            employ_uid = employ_uid,
            resultCode = 0,
            desc = "雇佣成功",
            member = {
                uid = employ_uid,
                head = employ_travelData:GetTravelHead(),
                name = employ_friendData:GetName(),
                star =  employ_friendData:GetStar(),
                sex =  employ_friendData:GetSex(),
                signature =  employ_friendData:GetSignature(),
                area =  employ_friendData:GetArea(),
                horoscope =  employ_friendData:GetHoroscope(),
                travel_level = employ_travelData:GetLevel(),
                level_time = static_const.Static_Const_TRAVEL_Employ_MAX_TIME,
            },
            type = 0,
            isCapture = false,
        }

        ZoneInfo.SendCmdToMe(req["do"], req["data"], zonetask)
    end
end

Zone.CmdNotifyTravelMemberBeCapture_S = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.EmployFriendToTravel_S"
    local uid = cmd.data.cmd_uid
    local employ_uid = cmd.data.employ_uid
    local last_uid = cmd.data.last_uid
    local has_shield = cmd.data.has_shield

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    local employ_friendData = FriendManager:GetFriendInfo(employ_uid)
    if employ_friendData == nil then
        return
    end

    local last_friendData = FriendManager:GetFriendInfo(last_uid)
    if last_friendData ~= nil then
        local last_travelData = last_friendData:GetUserTravel()
        last_travelData:DelMember(employ_uid)
    end

    local employ_travelData = employ_friendData:GetUserTravel()

    --玩家正在别雇佣，抓捕他们 删除原有的雇佣信息，并通知对方
    if has_shield == true then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 0,
            desc = "",
            type = 1,
            isCapture = true,
            member = {
                uid = employ_uid,
                head = employ_travelData:GetTravelHead(),
                name = employ_friendData:GetName(),
                star =  employ_friendData:GetStar(),
                sex =  employ_friendData:GetSex(),
                signature =  employ_friendData:GetSignature(),
                area =  employ_friendData:GetArea(),
                horoscope =  employ_friendData:GetHoroscope(),
                travel_level = employ_travelData:GetLevel(),
                relation_ship = cmd.data.relation_ship,
                level_time = 0,
            },
            visit = FriendManager.GetCaptureFriendVisitFriend(employ_friendData)
        }
        FriendManager.SendCmdByFriend(res["do"], res["data"], friendData)
        return
    end

    --设置抓捕信息
    employ_travelData:SetEmployUid(uid)
    employ_travelData:SetEmployName(friendData:GetName())

    travelData:AddMember(employ_uid)

    do
        local req = {}
        req["do"] = "Cmd.NotifyBeEmployedSuccess"
        req["data"] = {
            cmd_uid = uid,
            name = friendData:GetName(),
            employ_uid = employ_uid,
        }
        FriendManager.SendCmdByFriend(req["do"], req["data"], employ_friendData, zonetask)
    end

    do
        local req = {}
        req["do"] = "Cmd.NotifyEmployFriendSuccess"
        req["data"] = {
            cmd_uid = uid,
            employ_uid = employ_uid,
            resultCode = 0,
            desc = "雇佣成功",
            member = {
                uid = employ_uid,
                head = employ_travelData:GetTravelHead(),
                name = employ_friendData:GetName(),
                star =  employ_friendData:GetStar(),
                sex =  employ_friendData:GetSex(),
                signature =  employ_friendData:GetSignature(),
                area =  employ_friendData:GetArea(),
                horoscope =  employ_friendData:GetHoroscope(),
                travel_level = employ_travelData:GetLevel(),
                relation_ship = cmd.data.relation_ship,
                level_time = static_const.Static_Const_TRAVEL_Employ_MAX_TIME,
            },
            type = 0,
            isCapture = true,
            visit = FriendManager.GetCaptureFriendVisitFriend(employ_friendData),
        }

        FriendManager.SendCmdByFriend(req["do"], req["data"], friendData)
    end
end

Zone.CmdNotifyTravelMemberBeCapture_C = function(cmd,zonetask)
    local last_uid = cmd.data.cmd_uid
    local last_friendData = FriendManager:GetOrNewFriendInfo(last_uid)
     do
         local req = {}
         req["do"] = "Cmd.NotifyTravelMemberBeCapture_C"
         req["data"] = {
             cmd_uid = last_uid,
             employ_uid = cmd.data.employ_uid,
             uid = cmd.data.uid,
         }
         FriendManager.SendCmdByFriend(req["do"], req["data"], last_friendData, zonetask)
     end
end

-- 清楚雇佣CD时间
Zone.CmdClearEmployFriendCD_C = function(cmd,zonetask)
    local res = {}
    res["do"] = "Cmd.ClearEmployFriendCD_f_S"
    res["data"] = {
        cmd_uid = cmd.data.uid,
        uid = cmd.data.cmd_uid,
    }
    local friendData = FriendManager:GetOrNewFriendInfo(cmd.data.uid)

    FriendManager.SendCmdByFriend(res["do"], res["data"], friendData, zonetask)
end

Zone.CmdNofiyClearEmployCd = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local employ_friendData = FriendManager:GetOrNewFriendInfo(uid)
    local employ_travelData = employ_friendData:GetUserTravel()
    employ_travelData:SetLastEmployUid(0)
    employ_travelData:ClearEmployCd()
end

Zone.CmdClearEmployFriendCD_S = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)

    FriendManager.SendCmdByFriend(res["do"], res["data"], friendData)
end

Zone.CmdNotifyDelMember = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local employ_uid = cmd.data.employ_uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local travelData = friendData:GetUserTravel()
    travelData:DelMember(employ_uid)
end

-- 解除雇佣关系
Zone.CmdRescissionEmployFriendShip_C = function(cmd,zonetask)
    local res = {}
    res["do"] = "Cmd.RescissionEmployFriendShip_C"
    res["data"] = {
        cmd_uid = cmd.data.uid,
        rescission_uid = cmd.data.cmd_uid,
    }

    local friendData = FriendManager:GetOrNewFriendInfo(cmd.data.cmd_uid);
    local travelData = friendData:GetUserTravel()

    travelData:DelMember(cmd.data.uid)

    local employ_friendData = FriendManager:GetOrNewFriendInfo(cmd.data.uid)

    local employ_travelData = employ_friendData:GetUserTravel()

    employ_travelData:SetEmployUid(0)
    employ_travelData:SetEmployName("")

    FriendManager.SendCmdByFriend(res["do"], res["data"], employ_friendData, zonetask)
end

Zone.CmdNotifyTravelLevelup = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local level = cmd.data.level
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    travelData:SetLevel(level)
end

Zone.CmdNotifyTravelHeadChange = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid
    local head = cmd.data.head
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    travelData:SetTravelHead(head)
end