
CreateClass("FriendData")   --单个好友简单数据


--玩家的单个好友数据初始化
function FriendData:Init(uid, head, name, app_id, isQQFriend)
    self.uid = uid or 0
    self.head = head or ""
    self.name = name or ""
    self.app_id = app_id or ""
    self.isQQFriend = isQQFriend or false
    self.sex = 1
    self.star = 1
    self.signature = ""
    self.area = ""
    self.horoscope = ""
    self.money = 0
    self.product = 0
    self.click = 0
end

function FriendData:GetDBTable()
    local tmp = {}
    tmp.uid = self.uid
    tmp.head = self.head
    tmp.name = self.name
    tmp.app_id = self.app_id
    tmp.isQQFriend = self.isQQFriend
    tmp.sex = self.sex
    tmp.star = self.star
    tmp.signature = self.signature
    tmp.area = self.area
    tmp.horoscope = self.horoscope
    tmp.money = self.money
    tmp.product = self.product
    tmp.click = self.click
    return tmp
end

function FriendData:SetDBTable(data)
    self.uid = data.uid or 0
    self.head = data.head or ""
    self.name = data.name or ""
    self.app_id = data.app_id or ""
    self.isQQFriend = data.isQQFriend or false
    self.sex = data.sex or self.sex
    self.star = data.star or self.star
    self.signature = data.signature or self.signature
    self.area = data.area or self.area
    self.horoscope = data.horoscope or self.horoscope
    self.money = data.money or self.money
    self.product = data.product or self.product
    self.click = data.click or self.click
end

function FriendData:GetUid()
    return self.uid
end

function FriendData:GetHead()
    return self.head
end

function FriendData:GetName()
    return self.name
end

function FriendData:GetAppId()
    return self.app_id
end

function FriendData:GetIsQQFriend()
    return self.isQQFriend
end

