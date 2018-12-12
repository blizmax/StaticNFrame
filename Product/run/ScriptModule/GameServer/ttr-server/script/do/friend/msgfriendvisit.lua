
-- 获得好友互访数据
Net.CmdGetFriendVisitInfo_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.GetFriendVisitInfo_S"

    local uid = laccount.Id

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res;
    end

    local f_uid = cmd["data"].uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()

    local f_friendData = FriendManager:GetFriendInfo(f_uid)
    if f_friendData == nil then
        res["data"] = {
            resultCode = ERROR_CODE.ID_NOT_FOUND,
            desc = "该玩家不存在",
        }
        return res
    end

    local f_travelData = f_friendData:GetUserTravel()
    local f_friendvisitData = f_friendData:GetFriendVisit()

    local visit_friend = {
        uid = f_uid,
        head = f_friendData:GetHead(),
        name = f_friendData:GetName(),
        star = f_friendData:GetStar(),
        sex = f_friendData:GetSex(),
        money = f_friendData:GetMoney(),
        product = f_friendData:GetProduct(),
        travel_level = f_travelData:GetLevel(),
        cur_mapid = f_friendvisitData:GetCurMapId(),
        today_encouraged_number = f_friendvisitData:GetEncouragedNumber(),
        today_beteased_number = f_friendvisitData:GetBeteasedNumber(),
        is_today_mischief = friendvisitData:IsTodayMischief(f_uid),
        is_today_inspire = friendvisitData:IsTodayInspire(f_uid),
        builds = {},
        travel_member = {},
    }

    local f_friendvisitData_builds = f_friendvisitData:GetBuilds()
    for id, build in pairs(f_friendvisitData_builds) do
        local tmp = {
            id = build.id,
            lv = build.lv,
            buildlv = build.buildlv,
        }
        table.insert(visit_friend.builds, tmp)
    end

    local self_travel = {
        uid = f_friendData:GetUid(),
        head = f_travelData:GetTravelHead(),
        name = f_friendData:GetName(),
        star = f_friendData:GetStar(),
        sex = f_friendData:GetSex(),
        travel_level = f_travelData:GetLevel(),
    }

    table.insert(visit_friend.travel_member, self_travel)

    f_travelData:MembersForEach(
        function(m_uid, m_t)
            local m_friendData = FriendManager:GetFriendInfo(m_uid)
            if m_friendData ~= nil then
                local m_travelData = m_friendData:GetUserTravel()
                local tmp = {
                    uid = m_friendData:GetUid(),
                    head = m_travelData:GetTravelHead(),
                    name = m_friendData:GetName(),
                    star = m_friendData:GetStar(),
                    sex = m_friendData:GetSex(),
                    travel_level = m_travelData:GetLevel(),
                }
                table.insert(visit_friend.travel_member, tmp)
            end

        end
    )

    local userInfo = UserInfo.GetUserInfoById(uid)
    userInfo.mainTask:addProgress(TaskConditionEnum.VisitFriendEvent, 1)

    res["data"] = {
        resultCode = 0,
        desc = "",
        friend = visit_friend,
    }

    return res
end

--鼓舞好友,单词用错先保留
Net.CmdMischiefFriend_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.MischiefFriend_S"

    local uid = laccount.Id

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res;
    end

    local f_uid = cmd["data"].uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()
    local travelData = friendData:GetUserTravel()

    if friendData:GetUserFriend(f_uid) == nil then
        res["data"] = {
            resultCode = ERROR_CODE.FRIEND_VISIT_NOT_FRIEND,
            desc = "",
        }
        return res
    end

    if friendvisitData:MischiefNumberIsLimit() then
        res["data"] = {
            resultCode = ERROR_CODE.FRIEND_VISIT_TO_VISIT_LIMIT,
            desc = "",
        }
        return res
    end

    if friendvisitData:IsTodayMischief(f_uid) then
        res["data"] = {
            resultCode = ERROR_CODE.FRIEND_VISIT_TODAY_MISCHIEF,
            desc = "",
        }
        return res
    end

    local f_friendData = FriendManager:GetFriendInfo(f_uid)
    if f_friendData == nil then
        res["data"] = {
            resultCode = ERROR_CODE.ID_NOT_FOUND,
            desc = "",
        }
        return res
    end

    local f_friendvisitData = f_friendData:GetFriendVisit()
    local f_travelData = f_friendData:GetUserTravel()

    if f_friendvisitData:EncouragedNumberIsLimit() then
        res["data"] = {
            resultCode = ERROR_CODE.FRIEND_VISIT_ENCOURAGED_LIMIT,
            desc = "",
        }
        return res
    end

    local targetmoney = 0
    local targetproduct = 0

    local userInfo = UserInfo.GetUserInfoById(uid)
    if userInfo == nil then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res;
    end

    local f_userInfo = UserInfo.GetUserInfoById(f_uid)
    if f_userInfo ~= nil then
        targetmoney = f_userInfo.money
        targetproduct = f_userInfo.product
    else
        targetmoney = f_friendData:GetMoney()
        targetproduct = f_friendData:GetProduct()
    end

    local money = math.min(userInfo.product, targetproduct)*120 + f_friendData:GetStar() * 100
    money = math.ceil(money)
    if money <= 0 then
        money = 1
    end

    UserInfo.AddUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)
    if f_userInfo ~= nil then
        UserInfo.AddUserMoney(f_userInfo, static_const.Static_MoneyType_Gold, money)
    else
        f_friendvisitData:AddVisitMoney(money)
        f_friendData:SetMoney(f_friendData:GetMoney()+money)
        RankListMgr:UpdateRankNode(RankListMgr.rank_type_money, f_uid, f_friendData:GetMoney())
    end

    travelData:AddRelationShip(f_uid)
    f_travelData:AddRelationShip(uid)

    friendvisitData:SetLastMischiefTime(os.time())
    friendvisitData.last_mischief_money = money

    friendvisitData:AddMischiefNumber()
    friendvisitData:RecordMischiefFriend(f_uid)

    f_friendvisitData:AddEncouragedNumber()
    --玩家可能不在线,设置离线数据被该标志
    f_friendData:SetOfflineChange()

    --鼓舞或捣蛋好友，通知下对方
    if userInfo ~= nil then
        message.give(f_uid, userInfo, MsgTypeEnum.FriendVisitMischiefFriend, {tostring(money),})
    end

    userInfo.mainTask:addProgress(TaskConditionEnum.InspireFriendEvent, 1)

    res["data"] = {
        resultCode = 0,
        desc = "",
        count = friendvisitData:GetMischiefNumber(),
        money = money,
    }
    return res;
end

--捣蛋好友, 单词用错先保留
Net.CmdInspireFriend_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.InspireFriend_S"

    local uid = laccount.Id

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" or type(cmd["data"].buildid) ~= "number" then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res;
    end

    local f_uid = cmd["data"].uid
    local buildid = cmd["data"].buildid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()
    local travelData = friendData:GetUserTravel()

    if friendData:GetUserFriend(f_uid) == nil then
        res["data"] = {
            resultCode = ERROR_CODE.FRIEND_VISIT_NOT_FRIEND,
            desc = "不是好友不能访问",
        }
        return res
    end

    if friendvisitData:IsTodayInspire(f_uid) then
        res["data"] = {
            resultCode = ERROR_CODE.FRIEND_VISIT_TODAY_INSPIRE,
            desc = "",
        }
        return res
    end

    --是否可以看视屏
    local see_sceen = friendvisitData:IsTodayInspireSeeSceen(f_uid)

    --主要考虑到看视屏赠送了一次
    if friendvisitData:InspireNumberIsLimit() then
        if see_sceen == true then
            res["data"] = {
                resultCode = ERROR_CODE.FRIEND_VISIT_TO_VISIT_LIMIT,
                desc = "",
            }
            return res
        end
    end

    local f_friendData = FriendManager:GetFriendInfo(f_uid)
    if f_friendData == nil then
        res["data"] = {
            resultCode = ERROR_CODE.ID_NOT_FOUND,
            desc = "该玩家不存在",
        }
        return res
    end

    local f_travelData = f_friendData:GetUserTravel()
    local f_friendvisitData = f_friendData:GetFriendVisit()
    local f_travelData = f_friendData:GetUserTravel()

    if f_friendvisitData:BeteasedNumberIsLimit() then
        res["data"] = {
            resultCode = ERROR_CODE.FRIEND_VISIT_BETEASED_LIMIT,
            desc = "",
        }
        return res
    end

    --查看使用猜中
    local guest = false
    local reward_buildid = f_friendvisitData:GetLastInpireBuildId()
    if buildid == reward_buildid then
        guest = true
        f_friendvisitData:GetRandBuilds()
    end

    local targetmoney = 0

    local userInfo = UserInfo.GetUserInfoById(uid)
    if userInfo == nil then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res;
    end

    local f_userInfo = UserInfo.GetUserInfoById(f_uid)
    if f_userInfo ~= nil then
        targetmoney = f_userInfo.product
    else
        targetmoney = f_friendData:GetProduct()
    end

    local money = 0
    
    if guest == true then 
       money = math.min(userInfo.product, targetmoney)*360 + f_friendData:GetStar() * 100
    else
        money = math.min(userInfo.product, targetmoney)*120 + f_friendData:GetStar() * 100
    end
    money = math.ceil(money)
    if money <= 0 then
        money = 1
    end

    UserInfo.SubUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)
    if f_userInfo ~= nil then
        UserInfo.SubUserMoney(f_userInfo, static_const.Static_MoneyType_Gold, money)
    else
        f_friendvisitData:SubVisitMoney(money)
        if f_friendData:GetMoney() > money then
            f_friendData:SetMoney(f_friendData:GetMoney()-money)
        else
            f_friendData:SetMoney(0)
        end

        RankListMgr:UpdateRankNode(RankListMgr.rank_type_money, f_uid, f_friendData:GetMoney())
    end

    friendvisitData:SetLastInspireTime(os.time())

    if see_sceen == true then
        friendvisitData:AddInspireNumber()
        f_friendvisitData:AddBeteasedNumber()
    end

    friendvisitData:RecordInspireFriend(f_uid)

    --玩家可能不在线,设置离线数据被该标志
    f_friendData:SetOfflineChange()

    --鼓舞或捣蛋好友，通知下对方
    if userInfo ~= nil then
        message.give(f_uid, userInfo, MsgTypeEnum.FriendVisitInspireFriend, {tostring(money),})
    end

    userInfo.mainTask:addProgress(TaskConditionEnum.MischiefFriendEvent, 1)

    res["data"] = {
        resultCode = 0,
        desc = "",
        count = friendvisitData:GetInspireNumber(),
        money = money*2,
        see_sceen = see_sceen,
        reward_buildid = reward_buildid,
        guest = guest,
    }
    return res;
end

--鼓舞看视频完回调
Net.CmdMischiefFriend_Screen_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.MischiefFriend_Screen_S"

    local uid = laccount.Id

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res;
    end

    local f_uid = cmd["data"].uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()
    local travelData = friendData:GetUserTravel()

    local f_friendData = FriendManager:GetFriendInfo(f_uid)
    if f_friendData == nil then
        res["data"] = {
            resultCode = ERROR_CODE.ID_NOT_FOUND,
            desc = "该玩家不存在",
        }
        return res
    end

    local f_travelData = f_friendData:GetUserTravel()
    local f_friendvisitData = f_friendData:GetFriendVisit()

    local userInfo = UserInfo.GetUserInfoById(uid)
    if userInfo == nil then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res;
    end

    local f_userInfo = UserInfo.GetUserInfoById(f_uid)

    local money = friendvisitData.last_mischief_money

    UserInfo.AddUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)
    if f_userInfo ~= nil then
        UserInfo.AddUserMoney(f_userInfo, static_const.Static_MoneyType_Gold, money)
    else
        f_friendvisitData:AddVisitMoney(money)
        f_friendData:SetMoney(f_friendData:GetMoney()+money)
        RankListMgr:UpdateRankNode(RankListMgr.rank_type_money, f_uid, f_friendData:GetMoney())
    end

    travelData:AddRelationShip(f_uid)
    f_travelData:AddRelationShip(uid)

    --玩家可能不在线,设置离线数据被该标志
    f_friendData:SetOfflineChange()

    res["data"] = {
        resultCode = 0,
        desc = "",
        count = friendvisitData:GetMischiefNumber(),
        money = money*2,
    }
    return res;
end

--//捣蛋看视频完回调
Net.CmdInspireFriend_Screen_C = function(cmd, laccount)
    local res = { }
    res["do"] = "Cmd.InspireFriend_Screen_S"

    local uid = laccount.Id

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            resultCode = 1,
            desc = "数据出错"
        }
        return res;
    end

    local f_uid = cmd["data"].uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()

    friendvisitData:AgainInspireFriend(f_uid)

    res["data"] = {
        resultCode = 0,
        desc = "",
        count = friendvisitData:GetInspireNumber(),
        money = 0,
    }
    return res;
end

