require "script/gxlua/class"
require "script/gxlua/unilight"
require "script/do/common/staticconst"

CreateClass("UserTravel")   --单个玩家所有好友数据结构体

function UserTravel:Init(uid)
    self.uid = uid
    --初始化旅行团等级
    self.level = 1

    -- 旅行团功能 当前雇佣他的旅行团UID 如果为0表示空闲
    self.employUid = 0
    -- 当前雇佣他的旅行团名字
    self.employName = ""

    -- 雇佣CD时间时间到期前，需要知道上一次雇佣他的人是谁
    self.lastEmployUid = 0

    -- 旅行团功能 雇佣CD时间 cd时间到了，上一次雇佣他的对象可以重新雇佣他
    self.employCd = 0
    
    --旅行团成员映射，uid--加入旅行团时间
    self.members = Map:New()
    self.members:Init()

    --旅行团亲密度, 玩家UID--亲密值
    self.relationships = Map:New()
    self.relationships:Init()

    --今天剩余抓捕次数
    self.captureTimes = static_const.Static_Const_TRAVEL_INIT_MAX_CAPTURE_TIMES

    --上一次抓捕时间，用于隔天清理
    self.lastCaptureTime = 0

    --已经解锁的位置数目, 默认已经有3个
    self.unlockSlotCount = static_const.Static_Const_TRAVEL_Init_UNLOCK_SLOT_COUNT
end

function UserTravel:SetDBTable(data)
    if data == nil then return end

    self.uid = data.uid
    self.level = data.level
    self.employUid = data.employUid
    self.employName = data.employName
    self.employCd = data.employCd
    self.lastEmployUid = data.lastEmployUid
    
    for k,v in pairs(data.members) do
        self.members.Insert(k,v)
    end

    for k,v in pairs(data.relationships) do
        self.relationships.Insert(k,v)
    end

    self.captureTimes = data.captureTimes
    self.lastCaptureTime = data.lastCaptureTime
    self.unlockSlotCount = data.unlockSlotCount
end

function UserTravel:GetDBTable()
    local data = {}
    data.uid = self.uid
    data.level = self.level
    data.employUid = self.employUid
    data.employName = self.employName
    data.lastEmployUid = self.lastEmployUid
    data.employCd = self.employCd

    data.members = {}
    self.members:ForEach(
        function(k,v)
            data.members[k] = v
        end
    )

    data.relationships = {}
    self.relationships:ForEach(
        function(k,v)
            data.relationships[k] = v
        end
    )

    data.captureTimes = self.captureTimes

    data.lastCaptureTime = self.lastCaptureTime

    data.unlockSlotCount = self.unlockSlotCount

    return data
end

function UserTravel:GetEmployEndLeftTime(uid)
    local tt = self:GetMemberTime(uid)
    if tt + static_const.Static_Const_TRAVEL_Employ_MAX_TIME > os.time then
        return tt + static_const.Static_Const_TRAVEL_Employ_MAX_TIME - os.time
    end
    return 0
end

function UserTravel:GetUnlockSlotCount()
    return self.unlockSlotCount
end

function UserTravel:AddUnlockSlotCount()
    self.unlockSlotCount = self.unlockSlotCount + 1
end

function UserTravel:GetCaptureTimes()
    return self.captureTimes
end

function UserTravel:DecCaptureTimes()
    if self.captureTimes > 0 then
        self.captureTimes = self.captureTimes - 1
    else
        self.captureTimes = 0
    end
end

function UserTravel:AddCaptureTimes(add)
    if add > 0 then
        self.captureTimes = self.captureTimes + add
    end
end

function UserTravel:GetLastCaptureTime()
    return self.lastCaptureTime
end

function UserTravel:SetLastCaptureTime()
    self.lastCaptureTime = os.time()
end

function UserTravel:ClearCaptureInfo()
    if common.IsSameDay(self.lastCaptureTime, os.time()) == false then
        self.lastCaptureTime = os.time()
        self.captureTimes = static_const.Static_Const_TRAVEL_INIT_MAX_CAPTURE_TIMES
    end
end


function UserTravel:PrintUserTravel()
    unilight.debug("-------打印旅行团程序信息-------------")
    unilight.debug("\t\t\tuid:" .. self.uid)
    unilight.debug("\t\t\tlevel:" .. self.level)
    unilight.debug("\t\t\temployUid:" .. self.employUid)
    unilight.debug("\t\t\tlastEmployUid:" .. self.lastEmployUid)
    unilight.debug("\t\t\temployCd:" .. self.employCd)
    unilight.debug("\t\t\tcaptureTimes:" .. self.captureTimes)
    unilight.debug("\t\t\tself.unlockSlotCount: " .. self.unlockSlotCount)
    unilight.debug("------旅行团成员以及剩余时间------")
    self.members:ForEach(
        function(m_uid, m_time)
            unilight.debug("\tmember_uid:" .. m_uid)
            unilight.debug("\tleft_time:" .. (os.time()-m_time-600))
            unilight.debug("-----------------------")
        end
    )

    unilight.debug("------旅行团亲密度----------")
    self.members:ForEach(
        function(m_uid, m_r)
            unilight.debug("\tm_uid:" .. m_uid)
            unilight.debug("\treleation_ship:" .. m_r)
            unilight.debug("---------------------------")
        end
    )
end

--获取好友雇佣他的旅行团UID
function UserTravel:GetEmployUid()
    return self.employUid
end

function UserTravel:GetEmployName()
    return self.employName
end

function UserTravel:SetEmployName(name)
    self.employName = name
end

--设置好友雇佣他的旅行团UID
function UserTravel:SetEmployUid(uid)
    self.employUid = uid
end

-- 雇佣CD时间时间到期前，需要知道上一次雇佣他的人是谁
function UserTravel:GetLastEmployUid()
    return self.lastEmployUid
end

-- 雇佣CD时间时间到期前，需要知道上一次雇佣他的人是谁
function UserTravel:SetLastEmployUid(uid)
    self.lastEmployUid = uid
end

-- 旅行团功能 雇佣CD时间 cd时间到了，上一次雇佣他的对象可以重新雇佣他
function UserTravel:SetEmployCd()
    self.employCd = os.time() + static_const.Static_Const_Employ_CD_Time
end

--- 是否处于雇佣CD时段里
function UserTravel:GetEmployCdLeftTime()
    if self.employCd > os.time() then
        return self.employCd - os.time()
    else
        return 0
    end
end

function UserTravel:ClearEmployCd()
    self.employCd = 0
end

function UserTravel:GetLevel()
    return self.level
end

function UserTravel:LevelUp()
    self.level = self.level + 1
end

function UserTravel:MembersForEach(fun, ...)
    self.members:ForEach(fun, ...)
end

function UserTravel:IsExistMembers(uid)
    if self.members:Find(uid) == nil then
        return false
    else
        return true
    end
end

function UserTravel:GetMemberTime(uid)
    local t = self.members:Find(uid)
    if t == nil then
        t = 0
    end
    return t
end

function UserTravel:AddMember(uid)
    self.members:Insert(uid, os.time())
end

--删除旅行团成员
function UserTravel:DelMember(uid)
    self.members:Remove(uid)
end

function UserTravel:GetMemberCount()
    return self.members:Count()
end

function UserTravel:GetRelationShip(uid)
    local t = self.relationships:Find(uid)
    if t == nil then
        t = 0
    end
    return t
end

function UserTravel:AddRelationShip(uid)
    local t = self.relationships:Find(uid)
    if t == nil then
        self.relationships:Insert(uid,1)
    else
        t = t + 1
        self.relationships:Replace(uid, t)
    end
end

--清理超时的旅行团成员
function UserTravel:ClearOutTimeMember()
    local out = {}
    self.members:ForEach(
        function(m_uid, m_time)
            if m_time + static_const.Static_Const_TRAVEL_Employ_MAX_TIME < os.time then
                out[m_uid] = 0
            end
        end
    )

    for uid, v in pairs(out) do
        local friendData = FriendManager:GetFriendInfo(uid)
        if friendData ~= nil then
            local travelData = friendData:GetUserTravel()
            travelData:SetEmployUid(0)
            travelData:SetLastEmployUid(uid)
            travelData:SetEmployCd()
            travelData:AddRelationShip(self.uid)
        end
        self:DelMember(uid)
        self:AddRelationShip(uid)
    end
end

--定时查看数据
function UserTravel:CheckData(timer)
    self:ClearOutTimeMember()
end



