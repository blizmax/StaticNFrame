

-- 邀请游戏，发送邀请你玩着游戏的玩家UID
Zone.CmdBeAskedPlayGame_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.BeAskedPlayGame_S"

    local uid = cmd.data.cmd_uid
    local ask_uid = cmd.data.uid
    local isFriend = cmd.data.isFriend
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local askFriendData = FriendManager:GetFriendInfo(ask_uid);

    if askFriendData == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "错误，对方不存在"
        }
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
        return
    end

    if isFriend == false then
        ---可能的发放奖励代码，以后加上
        local req = { }
        req["do"] = "Cmd.AddFriendFromCenterServer_CS"
        req["data"] = {
            cmd_uid = uid,
            friend_data = {
                uid = askFriendData:GetUid(),
                head = askFriendData:GetHead(),
                name = askFriendData:GetName(),
                appId = askFriendData:GetAppId(),
            },
        }
        ZoneInfo.SendCmdToMe(req["do"], req["data"], zonetask)

        req["data"] = {
            cmd_uid = ask_uid,
            friend_data = {
                uid = friendData:GetUid(),
                head = friendData:GetHead(),
                name = friendData:GetName(),
                appId = friendData:GetAppId(),
            },
        }
        FriendManager.SendCmdByFriend(req["do"], req["data"], askFriendData, zonetask)
    end

    if friendData.isFirstLogin == true then
        local req = { }
        req["do"] = "Cmd.AddMeAskPlayerUidsAndFirstLogin_CS"
        req["data"] = {
            cmd_uid = ask_uid,
            uid = uid,
        }
        FriendManager.SendCmdByFriend(req["do"], req["data"], askFriendData, zonetask)
    end

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = ""
    }
    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end

-- 客户端建号时发送QQ好友消息
Zone.CmdSendUserQQFriendDataCmd_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.SendUserQQFriendDataCmd_S"

    local uid = cmd.data.cmd_uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local data = cmd["data"]

    --先将自己的QQ数据保存下来
    friendData:SetBaseInfo(uid, data.self_data.head, data.self_data.name, data.self_data.app_id, data.self_data.sex)

    if type(data.self_data.app_id) == "string" and data.self_data.app_id ~= "" then
        FriendManager:SaveUserQQInfo(data.self_data.app_id, uid)
    end

    local tmp_uids = {}
    --获取QQ好友信息，并且查看QQ好友是否有在玩本游戏，有的话，添加为好友
    for i, v in ipairs(data.friend_data) do
        --检查客户端数据输入
        if type(v.head) ~= "string" or type(v.name) ~= "string" or type(v.app_id) ~= "string" then
            res["data"] = {
                cmd_uid = uid,
                resultCode = 1,
                desc = "数据出错"
            }
            ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
            return
        end

        --判断是否双方是QQ好友，如果是就相互添加为好友
        local qqData_uid = FriendManager:GetUserQQInfo(v.app_id)

        if qqData_uid ~= nil then
            local qq_friend_data = FriendManager:GetFriendInfo(qqData_uid)
            if qq_friend_data ~= nil then
                local req = { }
                req["do"] = "Cmd.AddFriendFromCenterServer_CS"
                req["data"] = {
                    cmd_uid = uid,
                    friend_data = {
                        uid = qq_friend_data:GetUid(),
                        head = qq_friend_data:GetHead(),
                        name = qq_friend_data:GetName(),
                        appId = qq_friend_data:GetAppId(),
                    },
                }
                ZoneInfo.SendCmdToMe(req["do"], req["data"], zonetask)

                req["data"] = {
                    cmd_uid = qqData_uid,
                    friend_data = {
                        uid = friendData:GetUid(),
                        head = friendData:GetHead(),
                        name = friendData:GetName(),
                        appId = friendData:GetAppId(),
                    },
                }
                FriendManager.SendCmdByFriend(req["do"], req["data"], qq_friend_data, zonetask)
            end
        end
    end

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
    }
    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end

-- 获取玩家游戏好友信息
Zone.CmdGetUserFriendDataCmd_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.GetUserFriendDataCmd_S"
    res["data"] = { 
        cmd_uid = cmd.data.cmd_uid,
        friend_data = {},
    }

    local uid = cmd.data.cmd_uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    
    --获取QQ好友数据
    for i, data in ipairs(cmd.data.tmp) do
        local f_friendData = FriendManager:GetFriendInfo(data.uid)
        if f_friendData ~= nil then
            local data = {
                uid = data.uid,
                name = f_friendData:GetName(), 
                head =  f_friendData:GetHead(), 
                star =  f_friendData:GetStar(), 
                sex =  f_friendData:GetSex(), 
                signature =  f_friendData:GetSignature(), 
                area =  f_friendData:GetArea(), 
                horoscope =  f_friendData:GetHoroscope(),
                friend_ship = data.friend_ship,
                money = f_friendData:GetMoney(),
                product = f_friendData:GetProduct(),
                click = f_friendData:GetClick()
            }
            table.insert(res["data"].friend_data, data)
        end
    end

    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end

--获得玩家被邀请为好友的列表
Zone.CmdGetUserAskedAddFriends_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.GetUserAskedAddFriends_S"

    local uid = cmd.data.cmd_uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    res["data"] = { cmd_uid = uid,}
    res["data"].friend_data = {}

    for i, data in ipairs(cmd.data.tmp) do
        local ask_uid = data.uid
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
                friend_ship = data.friend_ship,
            }
            table.insert(res["data"].friend_data, req)
        end
    end

    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end

-- 发送好友请求
Zone.CmdSendReqAddFriendCmd_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.SendReqAddFriendCmd_S"

    local uid = cmd.data.cmd_uid
    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local travelData = friendData:GetUserTravel()

    --如果已经是好友的话
    local ask_uid = cmd["data"].friend_uid;

    --如果对方不在线， 查询保留的申请信息，如果有你，说明你多次申请了
    local askFriendData = FriendManager:GetFriendInfo(ask_uid);

    if askFriendData == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "错误，对方不存在"
        }
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
        return
    end

     --记录今天邀请过的好友
    do
        local req = {}
        req["do"] = "Cmd.AddTodayAskedFriends_S"
        req["data"] = {
            cmd_uid = uid,
            uid = ask_uid,
        }
        ZoneInfo.SendCmdToMe(req["do"], req["data"], zonetask)
    end

    do
        local req = {}
        req["do"] = "Cmd.AskAddFriends_S"
        req["data"] = {
            cmd_uid = ask_uid,
            uid = uid,
        }
        FriendManager:SendCmdByFriend(req["do"], req["data"], askFriendData, zonetask)
    end

	--给被邀请的人，一条消息记录
    askFriendData:Give(friendData, MsgTypeEnum.FriendApply)

    res["data"] = {
        cmd_uid = uid,
        resultCode = 0,
        desc = ""
    }

    --需要知道对方是否在线,如果不在线的话，先把请求存起来
    if askFriendData:GetOnline() == false then
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
        return
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
            friend_ship = 0,
        }

        ZoneInfo.SendCmdToMeById(req["do"], req["data"], askFriendData.gameid, askFriendData.zoneid)
        
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
    end
end

-- 同意或拒绝好友请求
Zone.CmdSendReqAgreeAddFriendCmd_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.SendReqAgreeAddFriendCmd_S"

    local uid = cmd.data.cmd_uid
    local agree = cmd.data.agree
    local ask_uid = cmd.data.uid

    local friendData = FriendManager:GetOrNewFriendInfo(uid);
    local ask_friendData = FriendManager:GetFriendInfo(ask_uid);
    if ask_friendData == nil then
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "对方不存在"
        }
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
        return
    end

    --删除今天申请记录
    --ask_friendData:DelTodayAskedFriends(uid)

    --如果同意的话，就相互加为好友
    if agree == true then
        do
            local req = { }
            req["do"] = "Cmd.AddFriendFromCenterServer_CS"
            req["data"] = {
                cmd_uid = uid,
                friend_data = {
                    uid = ask_friendData:GetUid(),
                    head = ask_friendData:GetHead(),
                    name = ask_friendData:GetName(),
                    appId = ask_friendData:GetAppId(),
                    isQQFriend = cmd.data.isQQFriend,
                },
            }
            ZoneInfo.SendCmdToMe(req["do"], req["data"], zonetask)
        end

        do
            local req = { }
            req["do"] = "Cmd.AddFriendFromCenterServer_CS"
            req["data"] = {
                cmd_uid = ask_uid,
                friend_data = {
                    uid = friendData:GetUid(),
                    head = friendData:GetHead(),
                    name = friendData:GetName(),
                    appId = friendData:GetAppId(),
                    isQQFriend = cmd.data.isQQFriend,
                },
            }
            FriendManager.SendCmdByFriend(req["do"], req["data"], ask_friendData, zonetask)
        end

        --ask_uid添加uid为朋友，这个是接口是uid接受或拒绝ask_uid的请求
        FriendManager.NotifyMainTaskAddProgress(zonetask, ask_friendData, TaskConditionEnum.AddFriendEvent, 1)

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

            ZoneInfo.SendCmdToMeById(req["do"], req["data"], ask_friendData.gameid, ask_friendData.zoneid)
        end
    end

    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end

-- 删除好友
Zone.CmdSendReqDeleteFriendCmd_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.SendReqDeleteFriendCmd_S"
    local uid = cmd.data.cmd_uid

    local del_uid = cmd["data"].uid

    local delFriendData = FriendManager:GetFriendInfo(del_uid)

    if delFriendData == nil then
        return
    end

    local res = {}
    res["do"] = "Cmd.SendReqDeleteFriendCmd_C"
    res["data"] = {
        cmd_uid = del_uid,
        uid = uid,
    }

    FriendManager.SendCmdByFriend(res["do"], res["data"], delFriendData, zonetask)
end

--推荐好友
Zone.CmdSendReqRecommendFriendCmd_C = function(cmd,zonetask)
    local uid = cmd.data.cmd_uid

    local res = { }
    res["do"] = "Cmd.SendReqRecommendFriendCmd_S"
    res["data"] = {cmd_uid = uid,}
    res["data"].friends = {}

    local friendData = FriendManager:GetOrNewFriendInfo(uid)

    local tmp = {}
    local maxNum = FriendManager:GetFriendCount()
    maxNum = (maxNum < 100) and maxNum or 100

    for i = 0, maxNum, 1 do
        local friendInfo = FriendManager:GetRandomFriendInfo()
        --推荐好友，满足条件 1不是自己 2不是好友 3今天没有被推荐过 4今天没有被邀请过
        if friendInfo ~= nil and friendInfo:GetUid() ~= uid and tmp[friendInfo:GetUid()] == nil  and
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
                friend_ship = 0,
            }
            table.insert(res["data"].friends, info)
            tmp[friendInfo:GetUid()] = true
        end

        --如果人数多于20直接发送，如果没有从以前推荐过的列表中插入
        if #res["data"].friends >= 20 then
            return res
        end
    end

    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end

--通过UID获得对方信息
Zone.CmdGetUserInfoByUid_C = function(cmd,zonetask)
    local res = { }
    res["do"] = "Cmd.GetUserInfoByUid_S"

    local uid = cmd.data.cmd_uid
    local find_uid = cmd.data.uid
    local friendData = FriendManager:GetFriendInfo(find_uid)

    if friendData ~= nil then
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
            friend_ship = 0,
        }
    else
        res["data"] = {
            cmd_uid = uid,
            resultCode = 1,
            desc = "玩家不存在",
        }
    end
    ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
end
