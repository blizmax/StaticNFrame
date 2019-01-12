
-- 获得好友互访数据
Zone.CmdGetFriendVisitInfo_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.GetFriendVisitInfo_S"

    local uid = cmd.data.cmd_uid

    local f_uid = cmd["data"].uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()

    local f_friendData = FriendManager:GetFriendInfo(f_uid)
    if f_friendData == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.ID_NOT_FOUND,
            desc = "该玩家不存在",
        }
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
        return
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

    FriendManager.NotifyMainTaskAddProgress(zonetask, friendData, TaskConditionEnum.VisitFriendEvent, 1)

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        friend = visit_friend,
    }

    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
    return
end

--鼓舞好友,单词用错先保留
Zone.CmdMischiefFriend_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.MischiefFriend_f_C"
    res["data"] = {
        cmd_uid = cmd.data.uid,
        mischief_uid = cmd.data.cmd_uid,
        product = cmd.data.product,
    }

    local friendData = FriendManager:GetFriendInfo(cmd.data.uid)
    if friendData == nil then
        return
    end

    FriendManager.SendCmdByFriend(res["do"], res["data"], friendData, zonetask)
end

--捣蛋好友, 单词用错先保留
Zone.CmdInspireFriend_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.InspireFriend_f_C"
    res["data"] = {
        cmd_uid = cmd.data.uid,
        inspire_uid = cmd.data.cmd_uid,
        buildid = cmd.data.buildid,
        product = cmd.data.product,
        see_sceen = cmd.data.see_sceen,
    }

    local friendData = FriendManager:GetFriendInfo(cmd.data.uid)
    if friendData == nil then
        return
    end

    FriendManager.SendCmdByFriend(res["do"], res["data"], friendData, zonetask)
end

Zone.CmdMischiefFriend_S = function(cmd,zonetask)
    local friendData = FriendManager:GetFriendInfo(cmd.data.cmd_uid)
    if friendData == nil then
        return
    end

    FriendManager.SendCmdByFriend(res["do"], res["data"], friendData, zonetask)
end

Zone.CmdInspireFriend_S = function(cmd,zonetask)
    local friendData = FriendManager:GetFriendInfo(cmd.data.cmd_uid)
    if friendData == nil then
        return
    end

    FriendManager.SendCmdByFriend(cmd["do"], cmd["data"], friendData)
end

--鼓舞看视频完回调
Zone.CmdMischiefFriend_Screen_C = function(cmd,zonetask)
    local res = {}
    res["do"] = "Cmd.MischiefFriend_Screen_C"
    res["data"] = {
        cmd_uid = cmd.data.uid,
        money = cmd.data.money,
    }

    local f_friendData = FriendManager:GetFriendInfo(cmd.data.uid)
    if f_friendData == nil then
        return
    end

    FriendManager.SendCmdByFriend(res["do"], res["data"], f_friendData, zonetask)
end