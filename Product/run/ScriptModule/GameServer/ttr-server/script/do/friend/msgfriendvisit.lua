
-- 获得好友互访数据
Net.CmdGetFriendVisitInfo_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.GetFriendVisitInfo_S"

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    if cmd.data == nil then
        cmd.data = {}
    end
    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
end

Lby.CmdGetFriendVisitInfo_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

--鼓舞好友,单词用错先保留
Net.CmdMischiefFriend_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.MischiefFriend_S"

    --检查客户端数据输入
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
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    local f_uid = cmd["data"].uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)

    local friendvisitData = friendData:GetFriendVisit()
    local travelData = friendData:GetUserTravel()

    if friendData:GetUserFriend(f_uid) == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.FRIEND_VISIT_NOT_FRIEND,
            desc = "",
        }
        return res
    end

    if friendvisitData:MischiefNumberIsLimit() then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.FRIEND_VISIT_TO_VISIT_LIMIT,
            desc = "",
        }
        return res
    end

    if friendvisitData:IsTodayMischief(f_uid) then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.FRIEND_VISIT_TODAY_MISCHIEF,
            desc = "",
        }
        return res
    end

    local f_friendData = FriendManager:GetFriendInfo(f_uid)
    if f_friendData == nil then
        if cmd.data == nil then
            cmd.data = {}
        end
        cmd.data.cmd_uid = uid
        cmd.data.product = friendData:GetProduct()
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
        return
    end

    local f_friendvisitData = f_friendData:GetFriendVisit()
    local f_travelData = f_friendData:GetUserTravel()

    if f_friendvisitData:EncouragedNumberIsLimit() then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.FRIEND_VISIT_ENCOURAGED_LIMIT,
            desc = "",
        }
        return res
    end

    local targetmoney = 0
    local targetproduct = 0

    targetmoney = f_friendData:GetMoney()
    targetproduct = f_friendData:GetProduct()

    local money = math.min(friendData:GetProduct(), targetproduct)*120 + f_friendData:GetStar() * 100
    money = math.ceil(money)
    if money <= 0 then
        money = 1
    end

    UserInfo.AddUserMoneyByUid(uid, static_const.Static_MoneyType_Gold, money)
    UserInfo.AddUserMoneyByUid(f_uid, static_const.Static_MoneyType_Gold, money)

    travelData:AddRelationShip(f_uid)
    f_travelData:AddRelationShip(uid)

    friendvisitData:SetLastMischiefTime(os.time())
    friendvisitData.last_mischief_money = money

    friendvisitData:AddMischiefNumber()
    friendvisitData:RecordMischiefFriend(f_uid)

    f_friendvisitData:AddEncouragedNumber()

    --鼓舞或捣蛋好友，通知下对方
    f_friendData:Give(friendData, MsgTypeEnum.FriendVisitMischiefFriend, {tostring(money),})

    userInfo.mainTask:addProgress(TaskConditionEnum.InspireFriendEvent, 1)

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        count = friendvisitData:GetMischiefNumber(),
        money = money,
    }
    return res
end

Lby.MischiefFriend_f_C = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local mischief_uid = cmd.data.mischief_uid
    local product = cmd.data.product

    local res = {}
    res["do"] = "Cmd.MischiefFriend_S"
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    local f_friendData = FriendManager:GetOrNewFriendInfo(uid)
    local f_friendvisitData = f_friendData:GetFriendVisit()
    local f_travelData = f_friendData:GetUserTravel()

    if f_friendvisitData:EncouragedNumberIsLimit() then
        res["data"] = {
            cmd_uid = mischief_uid,
            resultCode = ERROR_CODE.FRIEND_VISIT_ENCOURAGED_LIMIT,
            desc = "",
        }
        unilobby.SendCmdToLobby(res["do"], res["data"])
        return
    end

    local targetmoney = 0
    local targetproduct = 0

    targetmoney = f_friendData:GetMoney()
    targetproduct = f_friendData:GetProduct()

    local money = math.min(product, targetproduct)*120 + f_friendData:GetStar() * 100
    money = math.ceil(money)
    if money <= 0 then
        money = 1
    end

    UserInfo.AddUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)

    f_travelData:AddRelationShip(mischief_uid)

    f_friendvisitData:AddEncouragedNumber()

    --鼓舞或捣蛋好友，通知下对方
    --f_friendData:Give(friendData, MsgTypeEnum.FriendVisitMischiefFriend, {tostring(money),})

    res["data"] = {
        cmd_uid = mischief_uid,
        uid = uid,
        resultCode = 0,
        desc = "",
        money = money,
    }
    unilobby.SendCmdToLobby(res["do"], res["data"])
    return
end

Lby.CmdMischiefFriend_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local f_uid = cmd.data.uid
    local money = cmd.data.money
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    if cmd.data.resultCode ~= 0 then
        unilight.response(userInfo.laccount, cmd)
        return
    end

    local friendData = FriendManager:GetOrNewFriendInfo(uid)

    local friendvisitData = friendData:GetFriendVisit()
    local travelData = friendData:GetUserTravel()

    UserInfo.AddUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)

    travelData:AddRelationShip(f_uid)

    friendvisitData:SetLastMischiefTime(os.time())
    friendvisitData.last_mischief_money = money

    friendvisitData:AddMischiefNumber()
    friendvisitData:RecordMischiefFriend(f_uid)

    userInfo.mainTask:addProgress(TaskConditionEnum.InspireFriendEvent, 1)

    unilight.response(userInfo.laccount, cmd)
end

--捣蛋好友, 单词用错先保留
Net.CmdInspireFriend_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.InspireFriend_S"

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" or type(cmd["data"].buildid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local f_uid = cmd["data"].uid
    local buildid = cmd["data"].buildid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()
    local travelData = friendData:GetUserTravel()

    if friendData:GetUserFriend(f_uid) == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.FRIEND_VISIT_NOT_FRIEND,
            desc = "不是好友不能访问",
        }
        return res
    end

    if friendvisitData:IsTodayInspire(f_uid) then
        res["data"] = {
            cmd_uid = uid,
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
                cmd_uid = uid,
                resultCode = ERROR_CODE.FRIEND_VISIT_TO_VISIT_LIMIT,
                desc = "",
            }
            return res
        end
    end

    local f_friendData = FriendManager:GetFriendInfo(f_uid)
    if f_friendData == nil then
        if cmd.data == nil then
            cmd.data = {}
        end
        cmd.data.cmd_uid = uid
        cmd.data.product = friendData:GetProduct()
        cmd.data.see_sceen = see_sceen
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
        return
    end

    local f_travelData = f_friendData:GetUserTravel()
    local f_friendvisitData = f_friendData:GetFriendVisit()

    if f_friendvisitData:BeteasedNumberIsLimit() then
        res["data"] = {
            cmd_uid = uid,
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

    local money = 0
    
    if guest == true then 
        money = math.min(friendData:GetProduct(), f_friendData:GetProduct())*360 + f_friendData:GetStar() * 100
    else
        money = math.min(friendData:GetProduct(), f_friendData:GetProduct())*60 + f_friendData:GetStar() * 100
    end
    money = math.ceil(money)
    if money <= 0 then
        money = 1
    end

    UserInfo.AddUserMoneyByUid(uid, static_const.Static_MoneyType_Gold, money)
    UserInfo.SubUserMoneyByUid(f_uid, static_const.Static_MoneyType_Gold, money)

    friendvisitData:SetLastInspireTime(os.time())

    if see_sceen == true then
        friendvisitData:AddInspireNumber()
        f_friendvisitData:AddBeteasedNumber()
    end

    friendvisitData:RecordInspireFriend(f_uid)

    --鼓舞或捣蛋好友，通知下对方
    f_friendData:Give(friendData, MsgTypeEnum.FriendVisitInspireFriend, {tostring(money),})

    do
        local userInfo = UserInfo.GetUserInfoById(uid)
        if userInfo ~= nil then
            userInfo.mainTask:addProgress(TaskConditionEnum.MischiefFriendEvent, 1)
        end
    end

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        count = friendvisitData:GetInspireNumber(),
        money = money,
        see_sceen = see_sceen,
        reward_buildid = reward_buildid,
        guest = guest,
    }
    return res
end

Lby.InspireFriend_f_C = function(cmd, lobbyClientTask)
    local res = { }
    res["do"] = "Cmd.InspireFriend_S"

    local uid = cmd.data.cmd_uid
    local buildid = cmd.data.buildid
    local product = cmd.data.product
    local see_sceen = cmd.data.see_sceen

    local f_friendData = FriendManager:GetOrNewFriendInfo(uid)

    local f_travelData = f_friendData:GetUserTravel()
    local f_friendvisitData = f_friendData:GetFriendVisit()

    if f_friendvisitData:BeteasedNumberIsLimit() then
        res["data"] = {
            cmd_uid = cmd.data.inspire_uid,
            resultCode = ERROR_CODE.FRIEND_VISIT_BETEASED_LIMIT,
            desc = "",
        }
        unilobby.SendCmdToLobby(res["do"], res["data"])
        return
    end

    --查看使用猜中
    local guest = false
    local reward_buildid = f_friendvisitData:GetLastInpireBuildId()
    if buildid == reward_buildid then
        guest = true
        f_friendvisitData:GetRandBuilds()
    end

    local money = 0

    if guest == true then 
        money = math.min(product, f_friendData:GetProduct())*360 + f_friendData:GetStar() * 100
    else
        money = math.min(product, f_friendData:GetProduct())*60 + f_friendData:GetStar() * 100
    end

    money = math.ceil(money)
    if money <= 0 then
        money = 1
    end

    UserInfo.SubUserMoneyByUid(uid, static_const.Static_MoneyType_Gold, money)

    if see_sceen == true then
        f_friendvisitData:AddBeteasedNumber()
    end

    res["data"] = {
        cmd_uid = cmd.data.inspire_uid,
        uid = uid,
        resultCode = 0,
        desc = "",
        money = money,
        see_sceen = see_sceen,
        guest = guest,
        reward_buildid = reward_buildid,
    }

    --鼓舞或捣蛋好友，通知下对方
    --f_friendData:Give(friendData, MsgTypeEnum.FriendVisitInspireFriend, {tostring(money),})
    unilobby.SendCmdToLobby(res["do"], res["data"])
end

Lby.CmdInspireFriend_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local f_uid = cmd.data.uid
    local money = cmd.data.money
    local see_sceen = cmd.data.see_sceen

    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()
    local travelData = friendData:GetUserTravel()
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    if cmd.data.resultCode ~= 0 then
        unilight.response(userInfo.laccount, cmd)
        return
    end

    UserInfo.AddUserMoney(userInfo, static_const.Static_MoneyType_Gold, money)

    friendvisitData:SetLastInspireTime(os.time())

    if see_sceen == true then
        friendvisitData:AddInspireNumber()
    end

    friendvisitData:RecordInspireFriend(f_uid)

    userInfo.mainTask:addProgress(TaskConditionEnum.MischiefFriendEvent, 1)

    cmd.data.count = friendvisitData:GetInspireNumber()
    unilight.response(userInfo.laccount, cmd)
end

--鼓舞看视频完回调
Net.CmdMischiefFriend_Screen_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.MischiefFriend_Screen_S"

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local f_uid = cmd["data"].uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()
    local travelData = friendData:GetUserTravel()

    local money = friendvisitData.last_mischief_money
    travelData:AddRelationShip(f_uid)

    UserInfo.AddUserMoneyByUid(uid, static_const.Static_MoneyType_Gold, money)

    local f_friendData = FriendManager:GetFriendInfo(f_uid)
    if f_friendData == nil then
        if cmd.data == nil then
            cmd.data = {}
        end
        cmd.data.cmd_uid = uid
        cmd.data.money = money
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])

        res["data"] = {
            cmd_uid = uid,
            resultCode = 0,
            desc = "",
            count = friendvisitData:GetMischiefNumber(),
            money = money*2,
        }
        return res
    end

    local f_travelData = f_friendData:GetUserTravel()

    UserInfo.AddUserMoneyByUid(f_uid, static_const.Static_MoneyType_Gold, money)

    f_travelData:AddRelationShip(uid)

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        count = friendvisitData:GetMischiefNumber(),
        money = money*2,
    }
    return res
end

Lby.CmdMischiefFriend_Screen_C = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local money = cmd.data.money
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    UserInfo.AddUserMoneyByUid(userInfo, static_const.Static_MoneyType_Gold, money)
end

--//捣蛋看视频完回调
Net.CmdInspireFriend_Screen_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.InspireFriend_Screen_S"

    --检查客户端数据输入
    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local f_uid = cmd["data"].uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local friendvisitData = friendData:GetFriendVisit()

    friendvisitData:AgainInspireFriend(f_uid)

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "",
        count = friendvisitData:GetInspireNumber(),
        money = 0,
    }
    return res
end

