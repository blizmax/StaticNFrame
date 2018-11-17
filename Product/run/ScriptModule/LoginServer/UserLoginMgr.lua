
CreateClass("UserLoginMgr")

CreateClass("UserLoginData")

function UserLoginData:Init(account, uid)
    self.account = account
    self.uid = uid
    self.login_time = 0
    self.out_time = 60*1000
    self.sign = "0"
    self.gameid = 0
    self.zoneid = 0
    self.plat_key = ""
    self.plat_login = ""
end

function UserLoginData:GetPlatKey()
    return self.plat_key
end

function UserLoginData:GetPlatLogin()
    return self.plat_login
end

function UserLoginData:SetPlatKey(key)
    self.plat_key = key
end

function UserLoginData:SetPlatLogin(key)
    self.plat_login = key 
end

function UserLoginData:GetGameId()
    return self.gameid
end

function UserLoginData:SetGameId(id)
    self.gameid = id
end

function UserLoginData:GetZoneId()
    return self.zoneid
end

function UserLoginData:SetZoneId(id)
    self.zoneid = id
end

function UserLoginData:GetSign()
    return self.sign
end

function UserLoginData:SetSign(sign)
    self.sign = sign
end

function UserLoginData:GetAccount()
    return self.account
end

function UserLoginData:SetAccount(account)
    self.account = account
end

function UserLoginData:GetUid()
    return self.uid
end

function UserLoginData:SetUid(uid)
    self.uid = uid
end

function UserLoginData:GetLoginTime()
    return self.login_time
end

function UserLoginData:SetLoginTime(t)
    self.login_time = t
end

function UserLoginData:IsLoginTimeout()
    if self.login_time + self.out_time < unilight.GetNowTime() then
        return true
    end
end

function UserLoginMgr:Init()
    --uid -- data
    self.user_datas = {}
end

function UserLoginMgr:CreateUserLogin(account)
    local data = UserLoginData:New()
    data:Init(account, unilight.GetUUID())
    self.user_datas[data:GetUid()] = data
    return data
end

function UserLoginMgr:GetUserLogin(uid)
    return self.user_datas[uid]
end

function UserLoginMgr:GetUserLoginByAccount(account)
    local data = nil
    for k, data in pairs(self.user_datas) do
        if data.account == account then
            return data
        end
    end

    if data == nil then
        data = self:CreateUserLogin(account)
    end
    return data
end

