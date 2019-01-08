

-- 邀请游戏，发送邀请你玩着游戏的玩家UID
Net.CmdBeAskedPlayGame_C = function(cmd, laccount)
    local uid = laccount.Id

    local res = { }
    res["do"] = "Cmd.BeAskedPlayGame_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    --检查客户端数据输入
    if cmd["data"] == nil or cmd["data"].uid == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local ask_uid = cmd["data"].uid
    if ask_uid == uid then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "不能邀请自己"
        }
        return res
    end

    local isFriend = (friendData:GetUserFriend(uid) ~= nil)
    friendData:SetAskMePlayGameUid(ask_uid)

    --任务系统，任务完成情况
	local userInfo = UserInfo.GetUserInfoById(laccount.Id)
	if userInfo ~= nil then
		userInfo.achieveTask:addProgress(TaskConditionEnum.AskFriendEvent, 1)
		userInfo.dailyTask:addProgress(TaskConditionEnum.AskFriendEvent, 1)
		userInfo.mainTask:addProgress(TaskConditionEnum.AskFriendEvent, 1)
	end

    local askFriendData = FriendManager:GetFriendInfo(ask_uid);

    if askFriendData == nil then
        if cmd.data == nil then
            cmd.data = {}
        end
        cmd.data.cmd_uid = uid
        cmd.data.isFriend = isFriend
        cmd.data.isFirstLogin = friendData.isFirstLogin
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
        return
    end

    askFriendData:AddMeAskPlayerUids(uid)

    if isFriend == false then
        friendData:AddUserFriend(askFriendData:GetUid(), askFriendData:GetHead(), askFriendData:GetName(), askFriendData:GetAppId())
        askFriendData:AddUserFriend(friendData:GetUid(), friendData:GetHead(), friendData:GetName(), friendData:GetAppId())
    end

    if friendData.isFirstLogin == true then
        askFriendData:AddMeAskPlayerUidsAndFirstLogin(uid)
    end

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = ""
    }
    return res
end

Lby.CmdBeAskedPlayGame_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
    
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

Lby.CmdAddFriendFromCenterServer_CS = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    if cmd.data.friend_data ~= nil then
        local friendData = FriendManager:GetOrNewFriendInfo(uid)
        if friendData ~= nil then
            friendData:AddUserFriend(cmd.data.friend_data.uid, cmd.data.friend_data.head, cmd.data.friend_data.name, cmd.data.friend_data.appId, cmd.data.friend_data.isQQFriend)
        end
    end
end

Lby.CmdAddMeAskPlayerUidsAndFirstLogin_CS = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    if cmd.data.friend_data ~= nil then
        local friendData = FriendManager:GetOrNewFriendInfo(uid)
        if friendData ~= nil then
            friendData:AddMeAskPlayerUidsAndFirstLogin(cmd.data.uid)
        end
    end
end

    
-- 客户端建号时发送QQ好友消息
Net.CmdSendUserQQFriendDataCmd_C = function(cmd, laccount)
    local uid = laccount.Id
    local data = cmd["data"]
    local res = { }
    res["do"] = "Cmd.SendUserQQFriendDataCmd_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    --检查客户端数据输入

    if data == nil or data.self_data == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    if type(data.self_data.head) ~= "string" or type(data.self_data.name) ~= "string"
    or type(data.self_data.app_id) ~= "string" or type(data.self_data.sex) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    --先将自己的QQ数据保存下来
    friendData:SetBaseInfo(uid, data.self_data.head, data.self_data.name, data.self_data.app_id, data.self_data.sex)
    UserInfo.UpdateQqData(uid, data.self_data.head, data.self_data.name, data.self_data.sex)

    if cmd.data == nil then
        cmd.data = {}
    end
    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
    }
    return res
end

Lby.CmdSendUserQQFriendDataCmd_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

    
-- 获取玩家游戏好友信息
Net.CmdGetUserFriendDataCmd_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.GetUserFriendDataCmd_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local travelData = friendData:GetUserTravel()
    local friendvisitData = friendData:GetFriendVisit()

    res["data"] = {
        cmd_uid = uid,
        self_data = {
            uid = uid,
            name = friendData:GetName(),
            head = friendData:GetHead(),
            sex = friendData:GetSex(),
            star = friendData:GetStar(),
            signature =  friendData:GetSignature(), 
            area =  friendData:GetArea(), 
            horoscope =  friendData:GetHoroscope(),
            friend_ship = 0,
            money = friendData:GetMoney(),
            product = friendData:GetProduct(),
            click = friendData:GetClick(),
        },

        friend_data = {},
        today_mischief_number = friendvisitData:GetMischiefNumber(),
        today_inspire_number = friendvisitData:GetInspireNumber(),
    }

    --获取QQ好友数据
    friendData:UserFriendsForEach(
        function(k,v)
            local f_friendData = FriendManager:GetFriendInfo(k)
            if f_friendData ~= nil then
                local data = {
                    uid = k,
                    name = f_friendData:GetName(), 
                    head =  f_friendData:GetHead(), 
                    star =  f_friendData:GetStar(), 
                    sex =  f_friendData:GetSex(), 
                    signature =  f_friendData:GetSignature(), 
                    area =  f_friendData:GetArea(), 
                    horoscope =  f_friendData:GetHoroscope(),
                    friend_ship = travelData:GetRelationShip(k),
                    money = f_friendData:GetMoney(),
                    product = f_friendData:GetProduct(),
                    click = f_friendData:GetClick()
                }
                table.insert(res["data"].friend_data, data)
            end
        end
    )

    return res
--[[
    if cmd.data == nil then
        cmd.data = {}
    end
    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
--]]
end

Lby.CmdGetUserFriendDataCmd_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

--获得玩家被邀请为好友的列表
Net.CmdGetUserAskedAddFriends_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.GetUserAskedAddFriends_S"
    res["data"] = {}

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"].friend_data = { }
    --玩家不在线时， 被邀请为好友处理
    friendData:AskAddFriendsForEach(
        function(k,v)
            local ask_uid = k
            local ask_friend_data = FriendManager:GetFriendInfo(ask_uid)
            if ask_friend_data ~= nil then
                local req = { 
                    uid = ask_uid,
                    name = ask_friend_data:GetName(), 
                    head =  ask_friend_data:GetHead(), 
                    star =  ask_friend_data:GetStar(), 
                    sex =  ask_friend_data:GetSex(), 
                    signature =  ask_friend_data:GetSignature(), 
                    area =  ask_friend_data:GetArea(), 
                    horoscope =  ask_friend_data:GetHoroscope(),
                    friend_ship = travelData:GetRelationShip(ask_uid),
                }
                table.insert( res["data"].friend_data, req)
            end
        end
    )

    return res
--[[   if cmd.data == nil then
        cmd.data = {}
    end
    cmd.data.cmd_uid = uid
    unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
--]]
end

Lby.CmdGetUserAskedAddFriends_S  = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

Lby.CmdSendReqBeAddFriendCmd_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 发送好友请求
Net.CmdSendReqAddFriendCmd_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.SendReqAddFriendCmd_S"

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    --检查客户端输入
    if cmd["data"] == nil or type(cmd["data"].friend_uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    --好友上线，如果已经达到上线的话
    if friendData:GetFriendsCount() >= static_const.Static_Const_Friend_MAX_Friend_Count then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.FRIENDS_MAX_LIMIT,
            desc = "好友数量已达上限"
        }
        return res
    end

    --如果已经是好友的话
    local ask_uid = cmd["data"].friend_uid;

    if ask_uid == uid then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    if friendData:GetUserFriend(ask_uid) ~= nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.FRIENDS_IS_YOUR_FRIEND,
            desc = "对方已是你的好友"
        }
        return res
    end

    if ask_uid == uid  then
        res["data"] = {
            cmd_uid = uid,
            resultCode = ERROR_CODE.FRIENDS_CAN_ADD_SELF,
            desc = "不能加自己为好友"
        }
        return res
    end

    --检查自己今天的邀请信息
    if friendData:IsExistTodayAskedFriends(ask_uid) == true then
        res["data"] = {
            cmd_uid = uid,
           resultCode = ERROR_CODE.FRIENDS_APPLY_TOO_MUCH,
           desc = "已经申请"
        }
        return res
    end

    --任务系统，任务完成情况
	local userInfo = UserInfo.GetUserInfoById(laccount.Id)
	if userInfo ~= nil then
		userInfo.achieveTask:addProgress(TaskConditionEnum.AskFriendEvent, 1)
		userInfo.dailyTask:addProgress(TaskConditionEnum.AskFriendEvent, 1)
		userInfo.mainTask:addProgress(TaskConditionEnum.AskFriendEvent, 1)
	end

    local askFriendData = FriendManager:GetFriendInfo(ask_uid);

    if askFriendData == nil then
        if cmd.data == nil then
            cmd.data = {}
        end
        cmd.data.cmd_uid = uid
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
        return
    end

    --记录今天邀请过的好友
    friendData:AddTodayAskedFriends(ask_uid)
    askFriendData:AddAskAddFriends(uid)

	--给被邀请的人，一条消息记录
    askFriendData:Give(friendData, MsgTypeEnum.FriendApply)

    --需要知道对方是否在线,如果不在线的话，先把请求存起来
    if askFriendData:GetOnline() == false then
        return res
    else
        --在线的话发送好友请求给对方
        local req = { }
        req["do"] = "Cmd.SendReqBeAddFriendCmd_S"
        req["data"] = {
            cmd_uid = ask_uid,
            uid = uid,
            name = friendData:GetName(), 
            head =  friendData:GetHead(), 
            star =  friendData:GetStar(), 
            sex =  friendData:GetSex(), 
            signature =  friendData:GetSignature(), 
            area =  friendData:GetArea(), 
            horoscope =  friendData:GetHoroscope(),
            friend_ship = travelData:GetRelationShip(ask_uid),
        }

        UserInfo.SendInfo(ask_uid, req)

       return res
    end
end

Lby.CmdAddTodayAskedFriends_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    if cmd.data.friend_data ~= nil then
        local friendData = FriendManager:GetOrNewFriendInfo(uid)
        if friendData ~= nil then
            friendData:AddTodayAskedFriends(cmd.data.uid)
        end
    end
end

Lby.CmdAskAddFriends_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    if cmd.data.friend_data ~= nil then
        local friendData = FriendManager:GetOrNewFriendInfo(uid)
        if friendData ~= nil then
            friendData:AddAskAddFriends(cmd.data.uid)
        end
    end
end

Lby.CmdSendReqAddFriendCmd_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

Lby.CmdSendReqBeAddFriendCmd_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 同意或拒绝好友请求
Net.CmdSendReqAgreeAddFriendCmd_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.SendReqAgreeAddFriendCmd_S"

    --客户端数据参数检查
    if cmd["data"] == nil or type(cmd["data"].agree) ~= "boolean" or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local agree = cmd["data"].agree
    local ask_uid = cmd["data"].uid

    if uid == ask_uid then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "不能添加自己为好友"
        }
        return res
    end

    local friendData = FriendManager:GetOrNewFriendInfo(uid)

    friendData:DelAskAddFriends(ask_uid)

    if agree == true then
        --删除QQ好像删除信息
        friendData:DelDeleteQQFriend(ask_uid)
        if friendData:GetFriendsCount() >= static_const.Static_Const_Friend_MAX_Friend_Count then
            res["data"] = {
                cmd_uid = uid,
                resultCode = ERROR_CODE.FRIENDS_MAX_LIMIT,
                desc = "好友数目已经达到上限，不能添加"
            }
            return res
        end
    end

    local ask_friendData = FriendManager:GetFriendInfo(ask_uid);
    if ask_friendData == nil then
        local isQQFriend = false
        if friendData:IsExistDeleteQQFriend(ask_uid) == true then
            isQQFriend = true
        end
        if cmd.data == nil then
            cmd.data = {}
        end
        cmd.data.cmd_uid = uid
        cmd.data.isQQFriend = isQQFriend
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
        return
    end

    --删除今天申请记录
    ask_friendData:DelTodayAskedFriends(uid)

    --如果同意的话，就相互加为好友
    if agree == true then
        local isQQFriend = false
        if friendData:IsExistDeleteQQFriend(ask_uid) == true then
            isQQFriend = true
        end
        ask_friendData:DelDeleteQQFriend(uid)

        if ask_friendData:GetFriendsCount() >= static_const.Static_Const_Friend_MAX_Friend_Count then
            res["data"] = {
                cmd_uid = uid,
                resultCode = ERROR_CODE.FRIENDS_MAX_LIMIT,
                desc = "对方好友数目已经达到上限，不能添加"
            }
            return res
        end

        friendData:AddUserFriend(ask_friendData:GetUid(), ask_friendData:GetHead(), ask_friendData:GetName(), ask_friendData:GetAppId(), isQQFriend)
        ask_friendData:AddUserFriend(friendData:GetUid(), friendData:GetHead(), friendData:GetName(), friendData:GetAppId(), isQQFriend)

        --任务系统，任务完成情况
        local userInfo = UserInfo.GetOfflineUserInfo(ask_friendData.uid)
        if userInfo ~= nil then
            userInfo.mainTask:addProgress(TaskConditionEnum.AddFriendEvent, 1)
        end

        if ask_friendData:GetOnline() == true then
            res["data"] = {
                cmd_uid = uid,
                resultCode = 0,
                desc = "添加好友成功"
            }

            local req = {}
            req["do"] = "Cmd.SendReqAgreeAddFriendCmd_S"
            req["data"] = {
                cmd_uid = ask_uid,
                resultCode = 0,
                desc = "添加好友成功"
            }

            UserInfo.SendInfo(ask_uid, req)

            return res
        end
    end
end

Lby.CmdSendReqAgreeAddFriendCmd_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

-- 删除好友
Net.CmdSendReqDeleteFriendCmd_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.SendReqDeleteFriendCmd_S"
    local uid = cmd.data.cmd_uid

    if cmd["data"] == nil or type(cmd["data"].uid) ~= "number" then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "数据出错"
        }
        return res
    end

    local del_uid = cmd["data"].uid

    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    --如果是QQ好友的话，记录一下，免得下次登入又被加上
    if friendData:GetUserFriend(del_uid) ~= nil then
        local tmp = friendData:GetUserFriend(del_uid)
        if tmp:GetIsQQFriend() == true then
            --unilight.debug("是QQ好友，删除QQ好友，记录一下")
            friendData:AddDeleteQQFriend(del_uid)
        end
        friendData:DelUserFriend(del_uid)
    end

    --删除今天邀请和推荐
    friendData:DelTodayAskedFriends(del_uid)

    local delFriendData = FriendManager:GetFriendInfo(del_uid)
    if delFriendData == nil then
        if cmd.data == nil then
            cmd.data = {}
        end
        cmd.data.cmd_uid = uid
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])

        res["data"] = {
            cmd_uid = uid,
            resultCode = 0,
            desc = "删除好友成功"
        }
        return res
    end

    --如果是QQ好友的话，记录一下，免得下次登入又被加上
    if delFriendData:GetUserFriend(uid) ~= nil then
        local tmp = delFriendData:GetUserFriend(uid)
        if tmp:GetIsQQFriend() == true then
            --unilight.debug("是QQ好友，删除QQ好友，记录一下")
            delFriendData:AddDeleteQQFriend(uid)
        end
        delFriendData:DelUserFriend(uid)
    end

    delFriendData:DelTodayAskedFriends(uid)

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = "删除好友成功"
    }
    return res
end

Lby.CmdSendReqDeleteFriendCmd_C = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    local del_uid = cmd.data.uid

    local delFriendData = FriendManager:GetOrNewFriendInfo(uid)
    if delFriendData == nil then
        return
    end

    --如果是QQ好友的话，记录一下，免得下次登入又被加上
    if delFriendData:GetUserFriend(del_uid) ~= nil then
        local tmp = delFriendData:GetUserFriend(del_uid)
        if tmp:GetIsQQFriend() == true then
            --unilight.debug("是QQ好友，删除QQ好友，记录一下")
            delFriendData:AddDeleteQQFriend(del_uid)
        end
        delFriendData:DelUserFriend(del_uid)
    end

    delFriendData:DelTodayAskedFriends(del_uid)
end

Lby.CmdSendReqDeleteFriendCmd_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end

--推荐好友
Net.CmdSendReqRecommendFriendCmd_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.SendReqRecommendFriendCmd_S"
    res["data"] = {cmd_uid = uid,}
    res["data"].friends = {}

    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local travelData = friendData:GetUserTravel()

    for k, v in pairs(friendData.deleteQQFriend.map) do
         --推荐好友，满足条件 1不是自己 2不是好友 3今天没有被推荐过 4今天没有被邀请过
         if k ~= uid and friendData:GetUserFriend(k) == nil and 
            friendData:IsExistTodayAskedFriends(uid) == false then
            local friendInfo = FriendManager:GetFriendInfo(k)
            if friendInfo ~= nil then
                local info = {
                    uid = k,
                    name = friendInfo:GetName(), 
                    head =  friendInfo:GetHead(), 
                    star =  friendInfo:GetStar(), 
                    sex =  friendInfo:GetSex(), 
                    signature =  friendInfo:GetSignature(), 
                    area =  friendInfo:GetArea(), 
                    horoscope =  friendInfo:GetHoroscope(),
                    friend_ship = travelData:GetRelationShip(k),
                }
                table.insert(res["data"].friends, info)

                --如果人数多于20直接发送，如果没有从以前推荐过的列表中插入
                if #res["data"].friends >= 20 then
                    return res
                end
            end
         end
    end

    local tmp = {}
    for i = 0, 100, 1 do
        local friendInfo = FriendManager:GetRandomFriendInfo()
        --推荐好友，满足条件 1不是自己 2不是好友 3今天没有被推荐过 4今天没有被邀请过
        if friendInfo ~= nil and friendInfo:GetUid() ~= uid and tmp[friendInfo:GetUid()] == nil and 
        friendData:GetUserFriend(friendInfo:GetUid()) == nil and 
        friendData:IsExistTodayAskedFriends(uid) == false and
        friendInfo:GetName() ~= "" and friendInfo:GetHead() ~= "" then
            local info = {
                uid = friendInfo:GetUid(),
                name = friendInfo:GetName(), 
                head =  friendInfo:GetHead(), 
                star =  friendInfo:GetStar(), 
                sex =  friendInfo:GetSex(), 
                signature =  friendInfo:GetSignature(), 
                area =  friendInfo:GetArea(), 
                horoscope =  friendInfo:GetHoroscope(),
                friend_ship = travelData:GetRelationShip(friendInfo:GetUid()),
            }
            table.insert(res["data"].friends, info)
            tmp[friendInfo:GetUid()] = true
        end

        --如果人数多于20直接发送，如果没有从以前推荐过的列表中插入
        if #res["data"].friends >= 20 then
            return res
        end
    end

    return res
end

Lby.CmdSendReqRecommendFriendCmd_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end
    
--通过UID获得对方信息
Net.CmdGetUserInfoByUid_C = function(cmd, laccount)
    local uid = laccount.Id
    local res = { }
    res["do"] = "Cmd.GetUserInfoByUid_S"

    local find_uid = cmd.data.uid
    local friendData = FriendManager:GetFriendInfo(find_uid)

    if friendData ~= nil then
        local travelData = friendData:GetUserTravel()
        res["data"] = {
            cmd_uid = uid,
            resultCode = 0,
            desc = "",
            uid = find_uid,
            name = friendData:GetName(), 
            head =  friendData:GetHead(), 
            star =  friendData:GetStar(), 
            sex =  friendData:GetSex(), 
            signature =  friendData:GetSignature(), 
            area =  friendData:GetArea(), 
            horoscope =  friendData:GetHoroscope(),
            friend_ship = travelData:GetRelationShip(uid),
        }
        return res
    else
        if cmd.data == nil then
            cmd.data = {}
        end
        cmd.data.cmd_uid = uid
        unilobby.SendCmdToLobby(cmd["do"], cmd["data"])
    end
end
    
Lby.CmdGetUserInfoByUid_S = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid
    
    local userInfo = UserInfo.GetUserInfoById(uid)
	if userInfo == nil then
		unilight.error("userinfo is not exist,uid:"..uid)
		return
    end

    unilight.response(userInfo.laccount, cmd)
end