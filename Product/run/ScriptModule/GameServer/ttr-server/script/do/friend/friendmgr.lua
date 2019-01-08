

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
end

--好友系统，0点定时清理数据
function FriendManager:AllFriendZeroClear()
    unilight.debug("定时0点清理好友数据..........")
    self.userFriend:ForEach(
        function(uid, friendData)
            --玩家可能不在线,设置离线数据被该标志
            friendData:SetOfflineChange()
            friendData:ZeroClearData()
        end
    )
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

function FriendManager:GetAddontion(uid)
    local friendData = self:GetFriendInfo(uid)
    if friendData ~= nil then
        return friendData:GetAddontion()
    end
    return 1.0
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

        local ret = self:LoadDataFromDb(friendData)
        if ret == nil then
            self:SaveDataToDb(friendData)
        end
        if friendData:GetOnline() == false then
            friendData:SetLastLogoutTime()
        end
        self.userFriend:Insert(uid, friendData)
        table.insert(self.friendUidList, uid)
    end
    return friendData
end

function FriendManager:SaveDataToDb(friendData)
    if friendData == nil then return end

    local t = friendData:GetDBTable()
    unilight.savedata("friendinfo", t, true, true)
end

function FriendManager:LoadDataFromDb(friendData)
    if friendData == nil then return nil end
    
    local data = unilight.getdata("friendinfo", friendData.uid)
    if data == nil then
        return nil
    end
    friendData:SetDBTable(data)
    return friendData
end

function FriendManager:LoadDataFromDbByUid(uid)
    local data = unilight.getdata("friendinfo", uid)
    if data == nil then
        return nil
    end
    local friendData = UserFriend:New()
    friendData:Init(uid)
    friendData:SetDBTable(data)
    return friendData
end

-- 获取玩家好友信息如果不存在的话，从DB里拉取数据
function FriendManager:GetFriendInfo(uid)
    return self.userFriend:Find(uid)
end

--考虑到性能，一些情况下只考虑内存中存在的数据
function FriendManager:GetFriendInfoFromMemory(uid)
    return self.userFriend:Find(uid)
end

function FriendManager:FriendInfoForEach(fun, ...)
    self.userFriend:ForEach(fun, ...)
end

function FriendManager:DelFriendInfoFromMemory(uid)
    self.userFriend:Remove(uid)
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

-- 系统自动推荐好友
function FriendManager.SystemAutoRecommendFriend(uid, timer)
    local friendData = FriendManager:GetOrNewFriendInfo(uid)
    local travelData = friendData:GetUserTravel()
    
    local res = { }
    res["do"] = "Cmd.SystemAutoRecommendFriendCmd_S"
    for i=0,100,1  do
        local data = FriendManager:GetRandomFriendInfo()
        if data ~= nil and data:GetUid() ~= uid and data:GetOnline() == true and friendData:GetUserFriend(data:GetUid()) == nil then
            res["data"] = {
                cmd_uid = uid,
                uid = data:GetUid(),
                head = data:GetHead(),
                name = data:GetName(),
                star =  data:GetStar(), 
                sex =  data:GetSex(), 
                signature =  data:GetSignature(), 
                area =  data:GetArea(), 
                horoscope =  data:GetHoroscope(),
                friend_ship = travelData:GetRelationShip(data:GetUid()),
            }
            if friendData.gameid ~= 0 and friendData.zoneid ~= 0 then
                ZoneInfo.SendCmdToMeById(res["do"], res["data"], friendData.gameid, friendData.zoneid)
            end
            return
        end
    end
end

function FriendManager.RefreshUserFriendToDB()
    FriendManager:FriendInfoForEach(
        function(uid,info)
            info:ClearOfflineChange()
            FriendManager:SaveDataToDb(info)
        end
    )
end

-- 玩家上线
function FriendManager:UserLoginFriend(uid)
    local friendData = self:GetOrNewFriendInfo(uid)
    friendData:SetOnline()
    friendData:ClearLastLogoutTime()

    if friendData.autorecommendtimer == nil then
        friendData.autorecommendtimer = unilight.addtimer("FriendManager.SystemAutoRecommendFriend", static_const.Static_Const_Friend_System_Auto_Recommend_Time, uid)
    end

    return friendData
end

-- 玩家下线
function FriendManager:UserLogoutFriend(uid)
    local friendData = self:GetOrNewFriendInfo(uid)
    friendData:SetOffline()
    friendData:SetLastLogoutTime()

    friendData:ClearOfflineChange()
    FriendManager:SaveDataToDb(friendData)

    if friendData.autorecommendtimer ~= nil then
        unilight.stoptimer(friendData.autorecommendtimer)
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