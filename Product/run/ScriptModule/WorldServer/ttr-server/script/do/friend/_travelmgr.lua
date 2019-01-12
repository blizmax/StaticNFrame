

CreateClass("UserTravel")   --单个玩家所有好友数据结构体

TravelShieldStatusEnum = {
    TravelShieldStatus_Close = 0,
    TravelShieldStatus_Open = 1,
}

function UserTravel:Init(uid)
    self.uid = uid
    --初始化旅行团等级
    self.level = 0

    --旅行团界面专用头像数据, 数字，来自配置表
    self.travelHead = 1 -- 1是配置表数据

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
end

function UserTravel:GetTravelHead()
    return self.travelHead
end

function UserTravel:SetTravelHead(head)
    self.travelHead = head
end

function UserTravel:GetLevel()
    return self.level
end

function UserTravel:SetLevel(level)
    self.level = level
end


function UserTravel:SetDBTable(data)
    if data == nil then return end

    self.uid = data.uid or self.uid
    self.level = data.level or self.level
    self.travelHead = data.travelHead or self.travelHead

    self.employUid = data.employUid or self.employUid
    self.employName = data.employName or self.employName
    self.employCd = data.employCd or self.employCd
    self.lastEmployUid = data.lastEmployUid or self.lastEmployUid

    if data.members ~= nil then
        for k,v in pairs(data.members) do
            self.members:Insert(k,v)
        end
    end
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
    self.employCd = os.time() + static_const.Static_Const_TRAVEL_Employ_CD_Time
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