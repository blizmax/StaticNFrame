-- 客户端获得旅行团信息
Net.CmdGetUserTravelInfo_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.GetUserTravelInfo_S"

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"] = {}
    res["data"].level = travelData:GetLevel()
    res["data"].capture_times = travelData:GetCaptureTimes()
    res["data"].unlock_slot_count = travelData:GetUnlockSlotCount()
    res["data"].member = {}

    --清理已经到期的旅行团成员
    travelData:ClearOutTimeMember()

    --轮询收集每个旅行团成员的数据
    travelData:MembersForEach(
        function(m_uid, m_time)
            if m_time + static_const.Static_Const_TRAVEL_Employ_MAX_TIME > os.time() then
                local member = { }
                member.uid = m_uid
                local member_friendData = FriendManager:GetFriendInfo(m_uid);  
                if member_friendData ~= nil then
                    local member_travelData = member_friendData:GetUserTravel()
                    member.head = member_friendData:GetHead()
                    member.name = member_friendData:GetName()
                    member.star =  member_friendData:GetStar() 
                    member.sex =  member_friendData:GetSex() 
                    member.signature =  member_friendData:GetSignature()
                    member.area =  member_friendData:GetArea()
                    member.horoscope =  member_friendData:GetHoroscope()
                    member.friend_ship = travelData:GetRelationShip(m_uid)
                    member.travel_level = member_travelData:GetLevel()
                    member.level_time = os.time - (static_const.Static_Const_TRAVEL_Employ_MAX_TIME+m_time)
                    table.insert(res["data"].member, member)
                end
            end
        end
    )

    travelData:PrintUserTravel()
    return res
end

-- 打开好友雇佣界面信息
Net.CmdGetTravelEmployFriend_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.GetTravelEmployFriend_S"

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"] = {}
    res["data"].member = {}

    --清理已经到期的旅行团成员
    travelData:ClearOutTimeMember()

    --轮询好友信息， 筹集数据
    friendData:UserFriendsForEach(
        function(f_uid, f_info)
            if travelData:IsExistMembers(f_uid) == false then
                local f_friendData = FriendManager:GetFriendInfo(f_uid)
                if f_friendData ~= nil then
                    local f_travelData = f_friendData:GetUserTravel()
                    --推荐的玩家等级必须必好友等级大
                    if travelData:GetLevel() >= f_travelData:GetLevel() then
                        local tmp = {}
                        tmp.uid = f_uid
                        tmp.head = f_friendData:GetHead()
                        tmp.name = f_friendData:GetName()
                        tmp.star =  f_friendData:GetStar()
                        tmp.sex =  f_friendData:GetSex()
                        tmp.signature =  f_friendData:GetSignature()
                        tmp.area =  f_friendData:GetArea()
                        tmp.horoscope =  f_friendData:GetHoroscope()
                        tmp.travel_level = f_travelData:GetLevel()
                        tmp.relation_ship = f_travelData:GetRelationShip(uid)
                        tmp.cur_employ_uid = 0
                        tmp.cur_employ_name = ""
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
        end
    )

    return res
end

-- 打开推荐雇佣界面信息
Net.CmdGetTravelEmployRecommend_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.GetTravelEmployRecommend_S"

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"] = {}
    res["data"].member = {}

    --清理已经到期的旅行团成员
    travelData:ClearOutTimeMember()

    --选择条件匹配的可推荐对象
    --玩家不能已经被雇佣
    --玩家不能是好友
    --玩家等级必须小
    --不能再CD时间里
    local count = 0
    for f_uid, f_friendData in pairs(FriendManager.userFriend.map) do
        if travelData:IsExistMembers(f_uid) == false and friendData:GetUserFriend(f_uid) == nil then
            local f_travelData = f_friendData:GetUserTravel()
            if travelData:GetLevel() >= f_travelData:GetLevel() and f_travelData:GetEmployUid() == 0 then
                --这说明你不久前雇佣过对方, 不在CD时间里
                if uid ~= f_travelData:GetLastEmployUid() or f_travelData:GetEmployCdLeftTime() == 0 then
                    local tmp = {}
                    tmp.uid = f_uid
                    tmp.head = f_friendData:GetHead()
                    tmp.name = f_friendData:GetName()
                    tmp.star =  f_friendData:GetStar()
                    tmp.sex =  f_friendData:GetSex()
                    tmp.signature =  f_friendData:GetSignature()
                    tmp.area =  f_friendData:GetArea()
                    tmp.horoscope =  f_friendData:GetHoroscope()
                    tmp.travel_level = f_travelData:GetLevel()
                    tmp.relation_ship = f_travelData:GetRelationShip(uid)
                    tmp.cur_employ_uid = 0
                    tmp.cur_employ_name = ""
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
    local res = { }
    res["do"] = "Cmd.EmployFriendToTravel_S"

    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    if travelData:GetMemberCount() >= travelData:GetUnlockSlotCount() then
        res["data"] = {
            resultCode = 1,
            desc = "没有更多位置了，需解锁新位置"
        }
        return res
    end

    local employ_uid = cmd["data"].uid

    local employ_friendData = FriendManager:GetFriendInfo(employ_uid)
    if employ_friendData == nil then
        res["data"] = {
            resultCode = 1,
            desc = "对方不存在"
        }
        return res
    end

    local employ_travelData = employ_friendData:GetUserTravel()

    if travelData:IsExistMembers(employ_uid) == true then
        res["data"] = {
            resultCode = 1,
            desc = "已经被你雇佣过了"
        }
        return res
    end

    if employ_travelData:GetLastEmployUid() == uid then
        if employ_travelData:GetEmployCdLeftTime() > 0 then
            res["data"] = {
                resultCode = 1,
                desc = "雇佣CD时间，暂时不能被你雇佣"
            }
            return res
        end
    end

    if employ_travelData:GetLevel() > travelData:GetLevel() then
        res["data"] = {
            resultCode = 1,
            desc = "该好友等级高过你，不能雇佣（抓捕）"
        }
        return res
    end

    --玩家正在别雇佣，抓捕他们 删除原有的雇佣信息，并通知对方
    if employ_friendData:GetEmployUid() ~= 0 then
        --抓捕逻辑,抓捕次数有限制
        if friendData:GetUserFriend(employ_uid) ~= nil then
            res["data"] = {
                resultCode = 1,
                desc = "不是你的好友，不能抓捕"
            }
            return res
        end

        --清理一下抓捕次数
        travelData:ClearCaptureInfo()

        --查看玩家当前抓捕次数
        if travelData:GetCaptureTimes() <= 0 then
            res["data"] = {
                resultCode = 1,
                desc = "抓捕次数不够了，不能抓捕"
            }
            return res
        end
        --减少一次抓捕次数
        travelData:DecCaptureTimes()

       local last_uid = employ_friendData:GetEmployUid()
       local last_friendData = FriendManager:GetFriendInfo(last_uid)
       if last_friendData ~= nil then
            local last_travelData = last_friendData:GetUserTravel()
            last_travelData:DelMember(employ_uid)
       end
    end

    --设置抓捕信息
    employ_travelData:SetEmployUid(uid)
    employ_travelData:SetEmployName(friendData:GetName())
    travelData:AddMember(employ_uid)

    res["data"] = {
        resultCode = 0,
        desc = "雇佣成功"
    }
    return res
end

-- 清楚雇佣CD时间
Net.CmdClearEmployFriendCD_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.ClearEmployFriendCD_S"

    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()
    
    local employ_uid = cmd["data"].uid

    --判断对方是否存在
    local employ_friendData = FriendManager:GetFriendInfo(employ_uid)
    if employ_friendData == nil then
        res["data"] = {
            resultCode = 1,
            desc = "对方不存在"
        }
        return res
    end

    local employ_travelData = employ_friendData:GetUserTravel()

    --判断是否已经被你雇佣
    if travelData:IsExistMembers(employ_uid) == true then
        res["data"] = {
            resultCode = 1,
            desc = "已经被你雇佣过了"
        }
        return res
    end

    --判断当前等级
    if employ_travelData:GetLevel() > travelData:GetLevel() then
        res["data"] = {
            resultCode = 1,
            desc = "该好友等级高过你，不能雇佣（抓捕）"
        }
        return res
    end

    --如果需要清理的话
    if employ_travelData:GetLastEmployUid() == uid then
        if employ_travelData:GetEmployCdLeftTime() > 0 then
            --扣钱和比较钱的操作

            --
            employ_travelData:SetLastEmployUid(0)
            employ_travelData:ClearEmployCd()
        end
    end

    res["data"] = {
        resultCode = 0,
        desc = "清理成功"
    }
    return res
end

-- 购买抓捕次数
Net.CmdBuyCaptureFriendTime_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.BuyCaptureFriendTime_S"

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    --比较钱的操作


    --增加抓捕次数
    travelData:AddCaptureTimes(1)
    res["data"] = {
        resultCode = 0,
        desc = "购买成功"
    }
    return res
end

-- 解除雇佣关系
Net.CmdRescissionEmployFriendShip_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.RescissionEmployFriendShip_S"

    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    local employ_uid = cmd["data"].uid

    --判断对方是否存在
    local employ_friendData = FriendManager:GetFriendInfo(employ_uid)
    if employ_friendData == nil then
        res["data"] = {
            resultCode = 1,
            desc = "对方不存在"
        }
        return res
    end

    local employ_travelData = employ_friendData:GetUserTravel()

    --判断是否已经被你雇佣
    if travelData:IsExistMembers(employ_uid) == true then
        travelData:DelMember(employ_uid)
        employ_travelData:SetEmployUid(0)
        employ_travelData:SetEmployName("")
    end    

    return res
end

-- 团长升级
Net.CmdUserTravelLevelUp_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.UserTravelLevelUp_S"

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    --升级自身需要的条件


    travelData:LevelUp()
    res["data"] = {
        resultCode = 0,
        desc = "升级成功"
    }
    return res
end

-- 团员位置解锁
Net.CmdUserTravelUnlockSlot_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.UserTravelUnlockSlot_S"

    local uid = laccount.Id
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    if travelData:GetUnlockSlotCount() >= static_const.Static_Const_TRAVEL_MAX_EMPLOY_USER_COUNT then
        res["data"] = {
            resultCode = 0,
            desc = "已经达到最大值，解锁失败"
        }
        return res
    end

    --团员位置解锁的条件


    travelData:AddUnlockSlotCount()
    res["data"] = {
        resultCode = 0,
        desc = "位置解锁成功"
    }
    return res
end

