

CreateClass("FriendManager")    --好友数据结构管理

TaskConditionEnum = 
{
	LoginEvent = 0,						--登录
	BuildingLevelUpEvent = 1,			--升级建筑（的次数）
	BuildingChangeEvent = 2,			--建筑改造（的次数）
	TravelLevelUpEvent = 3,				--旅行团等级提升
	OpenMapEvent = 4,					--开启地图
	ClickEvent = 5,						--点击次数事件(点击旅行团)
	EmployFriendEvent = 6,				--雇佣好友事件
	CaptureFriendEvent = 7,				--抓捕好友事件
	StopCaptureEvent = 8,				--防御抓捕
	CostDiamondEvent  = 9,				--累积消耗砖石
	ApplyFriendEvent = 10,				--申请好友
	AskFriendEvent = 11,				--邀请好友玩游戏
	SharedGameEvent = 12,				--分享好友
	AllBuildingStarEvent = 13,			--建筑要达到的的总星级
	SpecifyBuildingLevelUpEvent = 14,	--升级指定建筑
	SpecifyBuildingStar = 15,			--指定建筑要达到的星级
	TravelLevelValueEvent = 16,			--旅行团要达到的等级
	AddFriendEvent = 17,				--添加好友
	VisitFriendEvent = 18,				--访问好友
	InspireFriendEvent = 19,			--鼓舞好友
	MischiefFriendEvent = 20,			--捣蛋好友
	OpenSpecifyMapEvent = 21,			--开启指定地图
}

--初始化，分配好友数据
function FriendManager:Init()
    --data 映射 data为一个UID映射好友数据
   self.userFriend = Map:New()
   self.userFriend:Init()
   --用来做随机推荐
   self.friendUidList = {}

   -- QQ唯一标识，存取app_id对应的头像，名字，以及在游戏中的uid
   self.userQQInfo = Map:New()
   self.userQQInfo:Init()

   self:LoadQQInfoFromDB()
   self:LoadAllUserFriendData()
end

function FriendManager:LoadAllUserFriendData()
    local tmp = unilight.getAll("friendinfo")
    if type(tmp) == "table" then
        unilight.info("load friend data num:"..#tmp)
    end
    for i, info in ipairs(tmp) do
        friendData = UserFriend:New()
        friendData:Init(info.uid)
        friendData:SetDBTable(info)
        friendData:SetOffline()
        self.userFriend:Insert(info.uid, friendData)

        RankListMgr:UpdateRankNode(RankListMgr.rank_type_star, friendData:GetUid(), friendData:GetStar())
        RankListMgr:UpdateRankNode(RankListMgr.rank_type_money, friendData:GetUid(), friendData:GetMoney())
        RankListMgr:UpdateRankNode(RankListMgr.rank_type_product, friendData:GetUid(), friendData:GetProduct())
        RankListMgr:UpdateRankNode(RankListMgr.rank_type_click, friendData:GetUid(), friendData:GetClick())
        table.insert(self.friendUidList, info.uid)
    end
end

--存取QQ唯一标识数据
function FriendManager:SaveUserQQInfo(app_id, uid)
    if self.userQQInfo:Find(app_id) ~= nil then
        self.userQQInfo:Replace(app_id, uid)
        return
    end

   self.userQQInfo:Insert(app_id, uid)
   local tmp = {
       app_id = app_id,
       uid = uid,
   }
   unilight.savedata("userQQAppId", tmp, true, true)
end

function FriendManager:LoadQQInfoFromDB()
    local tmp = unilight.getAll("userQQAppId")
    for i, info in ipairs(tmp) do
        self.userQQInfo:Insert(info.app_id, info.uid)
    end
end

-- 获得QQ唯一标识数据
function FriendManager:GetUserQQInfo(app_id)
   return self.userQQInfo:Find(app_id)
end

-- 获取或创建玩家好友信息
function FriendManager:GetOrNewFriendInfo(uid)
    local friendData = self.userFriend:Find(uid)

    --如果为空 则新建
    if friendData == nil then
        friendData = UserFriend:New()
        friendData:Init(uid)

        self.userFriend:Insert(uid, friendData)
        table.insert(self.friendUidList, uid)
    end
    return friendData
end

-- 获取玩家好友信息如果不存在的话，从DB里拉取数据
function FriendManager:GetFriendInfo(uid)
    local friendData = self.userFriend:Find(uid)
    return friendData
end

--考虑到性能，一些情况下只考虑内存中存在的数据
function FriendManager:GetFriendInfoFromMemory(uid)
    return self.userFriend:Find(uid)
end

function FriendManager:FriendInfoForEach(fun, ...)
    self.userFriend:ForEach(fun, ...)
end

function FriendManager:GetFriendCount()
    return self.userFriend:Count()
end

function FriendManager:GetRandomFriendInfo()
    local index = math.random(1, #self.friendUidList)
    local uid = self.friendUidList[index]
    if uid ~= nil then
        return FriendManager:GetFriendInfo(uid)
    end
end

-- 玩家上线
function FriendManager:UserLoginFriend(uid, zonetask)
    local friendData = self:GetOrNewFriendInfo(uid)

    local flag = true
    if friendData.online == true then
        if friendData.gameid ~= 0 and friendData.zoneid ~= 0 then
            if friendData.gameid ~= zonetask.GetGameId() or friendData.zoneid ~= zonetask.GetZoneId() then
                local req = { }
                req["do"] = "Cmd.ForceUserOffline_C"
                req["data"] = {
                    cmd_uid = uid,
                }

                ZoneInfo.SendCmdToMeById(req["do"], req["data"], friendData.gameid, friendData.zoneid)
                flag = false
            end
        end
    else
        if friendData.last_gameid ~= 0 and friendData.last_zoneid ~= 0 then
            if friendData.last_gameid ~= zonetask.GetGameId() or friendData.last_zoneid ~= zonetask.GetZoneId() then
                local req = { }
                req["do"] = "Cmd.ForceUserOffline_C"
                req["data"] = {
                    cmd_uid = uid,
                }

                ZoneInfo.SendCmdToMeById(req["do"], req["data"], friendData.last_gameid, friendData.last_zoneid)
                flag = false
            end
        end
    end

    friendData:SetOnline()

    friendData.gameid = zonetask.GetGameId()
    friendData.zoneid = zonetask.GetZoneId()
    friendData.last_gameid = 0
    friendData.last_zoneid = 0

    if flag == true then
        local res = {}
        res["do"] = "Cmd.UserInfoLoginCenter_S"
        res["data"] = {
            cmd_uid = uid,
        }
        ZoneInfo.SendCmdToMe(res["do"], res["data"], zonetask)
    end

    return friendData
end

-- 玩家下线
function FriendManager:UserLogoutFriend(uid, zonetask)
    local friendData = self:GetOrNewFriendInfo(uid)
    friendData:SetOffline()

    friendData.gameid = 0
    friendData.zoneid = 0
    friendData.last_gameid = zonetask.GetGameId()
    friendData.last_zoneid = zonetask.GetZoneId()
end

function FriendManager.SendCmdByFriend(doinfo, data, friendData, zonetask)
    if friendData.gameid ~= 0 and friendData.zoneid ~= 0 then
        ZoneInfo.SendCmdToMeById(doinfo, data, friendData.gameid, friendData.zoneid)
        return
    elseif friendData.last_gameid ~= 0 and friendData.last_zoneid ~= 0 then
        ZoneInfo.SendCmdToMeById(doinfo, data, friendData.last_gameid, friendData.last_zoneid)
        return
    end

    if zonetask ~= nil then
        ZoneInfo.SendCmdToMe(doinfo, data, zonetask)
    end
end

--获得一个玩家的抓捕互访数据
function FriendManager.GetCaptureFriendVisitFriend(f_friendData)
    local f_travelData = f_friendData:GetUserTravel()
    local f_friendvisitData = f_friendData:GetFriendVisit()

    local visit_friend = {
        uid = f_friendData:GetUid(),
        cur_mapid = f_friendvisitData:GetCurMapId(),
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

    return visit_friend
end

function FriendManager.NotifyDailyTaskAddProgress(zonetask, friendinfo, event, times)
    --通知客户端 每日任务完成
    local res = { }
    res["do"] = "Cmd.NotifyDailyTaskAddProgress_S"
    res["data"] = {
        cmd_uid = friendinfo.uid, 
        event = event,
        times = times,
    }
    FriendManager.SendCmdByFriend(res["do"], res["data"], friendinfo, zonetask)
end

function FriendManager.NotifyAchieveTaskAddProgress(zonetask, friendinfo, event, times)
    --通知客户端 每日任务完成
    local res = { }
    res["do"] = "Cmd.NotifyAchieveTaskAddProgress_S"
    res["data"] = {
        cmd_uid = friendinfo.uid, 
        event = event,
        times = times,
    }
    FriendManager.SendCmdByFriend(res["do"], res["data"], friendinfo, zonetask)
end

function FriendManager.NotifyMainTaskAddProgress(zonetask, friendinfo, event, times)
    --通知客户端 每日任务完成
    local res = { }
    res["do"] = "Cmd.NotifyMainTaskAddProgress_S"
    res["data"] = {
        cmd_uid = friendinfo.uid, 
        event = event,
        times = times,
    }
    FriendManager.SendCmdByFriend(res["do"], res["data"], friendinfo, zonetask)
end

function FriendManager.AddUserMoney(zonetask, friendinfo, moneytype, moneynum)
    local res = { }
    res["do"] = "Cmd.NotifyAddUserMoney_S"
    res["data"] = {
        cmd_uid = friendinfo.uid, 
        moneytype = moneytype,
        moneynum = moneynum,
    }
    FriendManager.SendCmdByFriend(res["do"], res["data"], friendinfo, zonetask)
end

function FriendManager.SubUserMoney(zonetask, friendinfo, moneytype, moneynum)
    local res = { }
    res["do"] = "Cmd.NotifySubUserMoney_S"
    res["data"] = {
        cmd_uid = friendinfo.uid, 
        moneytype = moneytype,
        moneynum = moneynum,
    }
    FriendManager.SendCmdByFriend(res["do"], res["data"], friendinfo, zonetask)
end

function FriendManager.UseItem(zonetask, friendinfo, itemid, itemnum)
    local res = { }
    res["do"] = "Cmd.NotifyUseItem_S"
    res["data"] = {
        cmd_uid = friendinfo.uid, 
        itemid = itemid,
        itemnum = itemnum,
    }
    FriendManager.SendCmdByFriend(res["do"], res["data"], friendinfo, zonetask)
end