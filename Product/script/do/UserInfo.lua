module('UserInfo', package.seeall) -- 用户信息

GlobalUserInfoMap = {} -- 玩家在线信息全局管理

--获取玩家的在线信息
function GetUserInfoById(uid)
	return GlobalUserInfoMap[uid]
end

function UpdateQqData(uid, data)
	local userInfo = GetUserInfoById(uid)

	if data == nil then
		unilight.warn("QQ data is nil")
		return
	end

	userInfo.qq = data
end

function CreateTempUserInfo(uid)
	unilight.debug("Create new user info")

	local userInfo = {
		uid			= uid,
		nickName	= "测试员" .. uid,
		money		= 1000,
		diamond		= 1000,
		star		= 0,
		settings	= {},
	}

	local world = World:new()
	world:init(userInfo)
	world:create()
	userInfo["world"] = world

	local taskMgr = DailyTaskMgr:new()
	userInfo["taskMgr"] = taskMgr
--[[
	--玩家商品
	local items = UserItems::new()
	items:init(userInfo)	
	userInfo["UserItems"] = items
	--玩家属性
	local props = UserProps::new()
	props:init(userInfo)
	userInfo["UserProps"] = props
]]--

	local msgMgr = MsgMgr:new()
	msgMgr:init(userInfo)
	msgMgr:createTemp()
	userInfo["msgMgr"] = msgMgr

	local mailMgr = MailMgr:new()
	mailMgr:init(userInfo)
	userInfo["mailMgr"] = mailMgr

	return userInfo
end

function CreateUserByDb(uid, dbUser)
	unilight.debug("Get user info from DB")

	userInfo = {
		uid = uid,
		nickName = dbUser.nickName,
		money = dbUser.money,
		diamond = dbUser.diamond,
		star = dbUser.star or 0,
	}

	userInfo["settings"] = dbUser.settings or {}

	local world = World:new()
	world:init(userInfo)

	if dbUser["world"] == nil then
		unilight.warn("Load user info from DB, but there is no world data")
		world:create()
	elseif world:loadFromDb(dbUser.world) ~= true then
		unilight.warn("Can load world data from DB")
	end

	userInfo["world"] = world

	--[Msg
	-- Temporarilly use userinfo db
	local msgMgr = MsgMgr:new()
	msgMgr:init(userInfo)

	if dbUser["msg"] == nil then
		msgMgr:createTemp()
	else
		msgMgr:loadFromDb(dbUser.msg)
		msgMgr:clean()
	end

	userInfo["msgMgr"] = msgMgr
	--]
	
	--Not to load data here
	local mailMgr = MailMgr:new()
	mailMgr:init(userInfo)
	userInfo["mailMgr"] = mailMgr

	local taskMgr = DailyTaskMgr:new()
	taskMgr:init(userInfo)
	userInfo["taskMgr"] = taskMgr

	if userInfo.star == 0 then
		userInfo.star = world:recalcStar()
	end

	unilight.debug("Can not obtain QQ data, simulate a QQ data")
	userInfo.qq = {
		head = "no head",
		name = "假的QQ名",
		sex = true,
		app_id = "23432412",
	}

	return userInfo
end

function Update()
	for k,userInfo in pairs(GlobalUserInfoMap) do
		userInfo.world:update()
	end
end

function GetClientData(userInfo)
	local userInfoData = {
		uid = userInfo.uid,
		nickName = userInfo.nickName,
		money = userInfo.money,
		diamond = userInfo.diamond,
		star = userInfo.star,
		world = userInfo.world:sn(),
	}

	return userInfoData
end

function GetServerData(userInfo)
	local userInfoData = {
		uid = userInfo.uid,
		nickName = userInfo.nickName,
		money = userInfo.money,
		diamond = userInfo.diamond,
		star = userInfo.star,
		world = userInfo.world:sn(),
		settings = userInfo.settings,
		msg = userInfo.msgMgr:sn(),
	}

	return userInfoData
end

function Connected(uid)
	FriendManager:UserLoginFriend(uid)
end

function Disconnected(uid)
	unilight.info("account_disconnect:" .. uid)

	FriendManager:UserLogoutFriend(uid)

	local userInfo = GetUserInfoById(uid)

	if userInfo == nil then
		unilight.warn("User is nil")
		return
	end

	unilight.savedata("userinfo", GetServerData(userInfo))

	userInfo.mailMgr:saveToDb()

	GlobalUserInfoMap[uid] = nil
end

if UserInfoClass == nil then
	CreateClass("UserInfoClass")
end
UserInfoClass:SetClassName("User")

function UserInfoClass:GetId()
	return self.data.uid
end

function UserInfoClass:Online()
	unilight.debug("Player online")
end

function UserInfoClass:Offline()
	unilight.debug("Player offline")
end

-- db: 注册玩家
function UserRegister(uid, lobbyId)
	local userdata, isSave = CreateUserData(uid, 0, lobbyId)
	return userdata, isSave
end

-- db: 构造玩家具体信息
function CreateUserData(uid, parent, lobbyId, parentIsAgent, parentList)
	local userdata = chessuserinfodb.CreateUserData(uid, 0) -- 以前chips 现在默认为0(金币 不在这里初始化 金币统一使用类似 钻石／房卡的初始化方式)

	-- 针对麻将大厅的初始化
	userdata.mahjong = {
		card 	= nil, 			                                   -- 玩家房卡数量
        play    = {},                                              -- 所有参与过的组局 用于汇总战绩 里面只存全局唯一的 globalRoomId
        playgame= {},                                              -- 记录玩家是否玩过某个游戏
        lastcreate = nil,                                          -- 最新一次创建记录
        isRecover = nil,                                           -- 单纯用于标记 龙岩 是否已恢复老区砖石数据
        gZGive    = nil,                                           -- 用于标记贵州是否给老用户赠送过
        playnum   = 0,                                             -- 游戏局数 marketing中记录的是存在上级parent的情况下才会记录的 这里是固定会记录的
    }

    SaveUserData(userdata)

	return userdata
end

-- db: 得到一个玩家信息
function GetUserDataById(uid)
	if not uid then
		unilight.debug("GetUserDataById uid为nil")
		return nil
	end
	local userdata = nil
    userdata = chessuserinfodb.GetUserDataById(uid)
	return userdata
end

-- db: 保存玩家的信息
function SaveUserData(userdata)
    if userdata == nil then
        unilight.warn("SaveUserData userdata 为空")
        return
    end
	if userdata.mahjong then
		if userdata.mahjong.diamond ~= nil then
			userdata.mahjong.diamond = tonumber(string.format("%0.3f", userdata.mahjong.diamond))
		end
		if userdata.mahjong.card ~= nil then
			userdata.mahjong.card = tonumber(string.format("%0.3f", userdata.mahjong.card))
		end
	end
	if userdata.property.chips ~= nil then
		userdata.property.chips = tonumber(string.format("%0.3f", userdata.property.chips))
	end
	chessuserinfodb.SaveUserData(userdata)
end

-- db: 按字段保存玩家信息
function SaveUserDataByField(uid, fieldpath, data)
    unilight.savefield("userinfo", uid, fieldpath, data)
end

-- info: 获取玩家基础数据 isMe=1或者nil 表示为自己
function GetUserDataBaseInfo(data, isMe)
	local userBaseInfo = {
		uid       = data.uid,
		headUrl   = data.base.headurl,
		nickName  = data.base.nickname,
		gender    = data.base.gender,
		platId    = data.base.platid,
		subPlatId = data.base.subplatid,
		ip        = data.base.loginip or "127.0.0.1",
		parent    = 0,
		flower    = data.base.flower,
		signature = data.base.signature,
		playNum   = 0,
        bankMoney = data.bank.chips,
        bankDiamond = data.bank.diamond,
        remainder = data.property.chips,
	}

    -- 钻石赋值出去 默认为0
    userBaseInfo.diamond = data.mahjong.diamond or 0

    -- 房卡始终赋值出去
	userBaseInfo.card = data.mahjong.card

    -- 金币始终赋值出去  -- 在这里将bankerchips里面的chips也加上
    if data.property.chips ~= nil then
        userBaseInfo.chips = data.property.chips + (data.property.bankerchips or 0)
    end

	return userBaseInfo
end
