Building = {
	owner = nil,
	state = nil,
	id = 0,
	lv = 1,
	buildLv = 1,
	lost = 0,
	produce = 0,
	isFootReward = false, --是否领取构造最高奖励

	product_time = os.time(),
	product_value = 0,
}

function Building:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function Building:init(owner, state, id, lv, buildLv, isFootReward)
	self.owner = owner
	self.state = state
	self.id = id
	self.lv = lv
	self.buildLv = buildLv
	self.product_time = os.time()
	self.product_value = 0
	self.produce = 0
	self.isFootReward = isFootReward or self.isFootReward

	if self.buildLv >= 100 then
		self.buildLv = 99
	end

	state.buildings[self.id] = self
end

function Building:recalcEverySec()
	self:recalc()

	self.product_value = 0
	if self.product_time + self:getValue("ProduceTime") < os.time() then
		self.product_value = self.produce * self:getValue("ProduceTime")
		self.product_time = os.time()
	end
end

function Building:recalc()
	--local words = "recalc building: {id: %d, lv: %d, buildLv: %d}"
	--unilight.debug(words.format(words, self.id, self.lv, self.buildLv))
--	self.produce = self:getValue("ProduceMoney") * self:getRebuildValue("Times")
	--local words = "recalc building: {id: %d, lv: %d, buildLv: %d, produce:%d}"
	--unilight.debug(words.format(words, self.id, self.lv, self.buildLv, self.produce))

	local userinfo = self.owner
	if userinfo == nil then
		return
	end

	--建筑效率 = 	1 - 建筑效率提升% / (建筑效率提升% + 1)	
	local aaa = (1 - UserProps:getUserProp(userinfo,"pBuildingProduceRate")/(UserProps:getUserProp(userinfo,"pBuildingProduceRate") + 1)) * GlobalConst.Takt_Time
	--建筑产量 / 建筑效率 * 建筑改造加成 * (1 + 抓捕好友加成 * (1 + 好友亲密度加成) * (1 + 旅行团加成)) * 世界加成
	self.produce = (self:getValue("ProduceMoney") / self:getValue("ProduceTime") )* self:getRebuildValue("Times")/aaa * (userinfo.friendAddontion) *(userinfo.UserProps:getUserProp(userinfo,"pWorldGoldAddRatio") + 1)

end

-- Get value from Levelup table
function Building:getValue(key, isNext)
	if isNext == nil or isNext == false then
		if key == "ProductMoney" then
			return tonumber(TableLevelup.query(self.id, self.lv)[key])
		else
			return TableLevelup.query(self.id, self.lv)[key]
		end
	else
		if key == "ProductMoney" then
			return tonumber(TableLevelup.query(self.id, self.lv + 1)[key])
		else
			return TableLevelup.query(self.id, self.lv + 1)[key]
		end
	end
end

function Building:getRebuildValue(key, isNext)
	if isNext == nil or isNext == false then
		if key == "CostMoney" or key == "ProductMoney" then
			return tonumber(TableRebuild.query(self.id, self.buildLv)[key])
		else
			return TableRebuild.query(self.id, self.buildLv)[key]
		end
	else
		if key == "CostMoney" or key == "ProductMoney" then
			return tonumber(TableRebuild.query(self.id, self.buildLv + 1)[key])
		else
			return TableRebuild.query(self.id, self.buildLv + 1)[key]
		end
	end
end

function Building:sn()
	local data = {
		id = self.id,
		lv = self.lv,
		buildLv = self.buildLv,
		isFootReward = self.isFootReward,
	}
	return data
end

function Building:earn()
	return self.product_value
end

function Building:levelupTen()
	if self.lv ~= 1 then
		return ERROR_CODE.ARGUMENT_ERROR
	end

	local num = 9
	self.lv = self.lv + num
	self:recalc()
	--self.owner.star = self.owner.star + num
	UserInfo.AddUserStar(self.owner, num)

	self.owner.achieveTask:addProgress(TaskConditionEnum.BuildingLevelUpEvent, self.owner.star)
	self.owner.dailyTask:addProgress(TaskConditionEnum.BuildingLevelUpEvent, self.owner.star)
	self.owner.mainTask:addProgress(TaskConditionEnum.BuildingLevelUpEvent, num)
	self.owner.mainTask:addProgress(TaskConditionEnum.SpecifyBuildingLevelUpEvent, num, self.id)
	self.owner.mainTask:addProgress(TaskConditionEnum.SpecifyBuildingStar, self.lv, self.id)

	--任务系统，任务完成情况
	--self.owner.achieveTask:addProgress(TaskConditionEnum.BuildingLevelUpEvent, self.owner.star)
	--self.owner.dailyTask:addProgress(TaskConditionEnum.BuildingLevelUpEvent, self.owner.star)

	local friendInfo = FriendManager:GetFriendInfo(self.owner.uid)
	if friendInfo ~= nil then
		friendInfo.simpleData.star = self.owner.star

		local friendvisitData = friendInfo:GetFriendVisit()
		if friendvisitData:GetCurMapId() == self.state.id then
			friendvisitData:SetLevel(self.id, self.lv)
		end
	end

	--同步中心服务器
	local data = {}
	data.cmd_uid = self.owner.uid
	data.userInfo = {
		star = self.owner.star,
		mapid = self.state.id,
		buildid = self.id,
		lv = self.lv,
	}
	unilobby.SendCmdToLobby("Cmd.BuildingLevelup_C", data)

	--for i = 1, num do
	--	self:AutoRebuild()
	--end

	return ERROR_CODE.SUCCESS
end

function Building:levelup()
	local cost = self:getValue("CostMoney", true)

	if (cost == nil) then
		return ERROR_CODE.BUILDING_LEVEL_MAX
	end

	local money_table = string.split(cost, "_")
	local money_type, money =  money_table[1], money_table[2]
	if money_type == nil or money == nil then
		unilight.warn("Table[Levelup]'s CostMoney is error")
		return ERROR_CODE.TABLE_ERROR
	end

	if UserInfo.CheckUserMoney(self.owner, money_type, money) ~= true then
		if tonumber(money_type) == static_const.Static_MoneyType_Gold then
			return ERROR_CODE.MONEY_NOT_ENOUGH
		end
		if tonumber(money_type) == static_const.Static_MoneyType_Diamond then
			return ERROR_CODE.DIAMOND_NOT_ENOUGH
		end
	end

	UserInfo.SubUserMoney(self.owner, money_type, money)

	self.lv = self.lv + 1
	self:recalc()
	--self.owner.star = self.owner.star + 1
	UserInfo.AddUserStar(self.owner, 1)

	--任务系统，任务完成情况
	self.owner.achieveTask:addProgress(TaskConditionEnum.BuildingLevelUpEvent, self.owner.star)
	self.owner.dailyTask:addProgress(TaskConditionEnum.BuildingLevelUpEvent, self.owner.star)
	self.owner.mainTask:addProgress(TaskConditionEnum.BuildingLevelUpEvent, 1)
	self.owner.mainTask:addProgress(TaskConditionEnum.SpecifyBuildingLevelUpEvent, 1, self.id)
	self.owner.mainTask:addProgress(TaskConditionEnum.SpecifyBuildingStar, self.lv, self.id)

	local friendInfo = FriendManager:GetFriendInfo(self.owner.uid)
	if friendInfo ~= nil then
		friendInfo.simpleData.star = self.owner.star

		local friendvisitData = friendInfo:GetFriendVisit()
		if friendvisitData:GetCurMapId() == self.state.id then
			friendvisitData:SetLevel(self.id, self.lv)
		end
	end

	local data = {}
	data.cmd_uid = self.owner.uid
	data.userInfo = {
		star = self.owner.star,
		mapid = self.state.id,
		buildid = self.id,
		lv = self.lv,
	}
	unilobby.SendCmdToLobby("Cmd.BuildingLevelup_C", data)

	--self:AutoRebuild()

	return ERROR_CODE.SUCCESS
end

function Building:AutoRebuild()
	local row = TableRebuild.query(self.id, self.buildLv + 1)
	if row == nil then
		return
	end

	local needLv = row["NeedLv"]

	if type(needLv) ~= "number" then
		unilight.error("Table[Rebuild]'s needLv is error")
		return
	end

	if self.lv >= needLv then
		self.buildLv = self.buildLv + 1
		self:recalc()

		--任务系统，任务完成情况
		self.owner.achieveTask:addProgress(TaskConditionEnum.BuildingChangeEvent, 1)
		self.owner.dailyTask:addProgress(TaskConditionEnum.BuildingChangeEvent, 1)

		local friendInfo = FriendManager:GetFriendInfo(self.owner.uid)
		if friendInfo ~= nil then
			local friendvisitData = friendInfo:GetFriendVisit()
			if friendvisitData:GetCurMapId() == self.state.id then
				friendvisitData:SetBuildLevel(self.id, self.buildLv)
			end
		end

		local data = {}
		data.cmd_uid = self.owner.uid
		data.mapid = self.state.id
		data.buildid = self.id
		data.buildLv = self.buildLv
		unilobby.SendCmdToLobby("Cmd.BuildingReBuild_C", data)

		local reward = row["RewardMoney"]

		local money_table = string.split(reward, "_")
		local money_type, money =  money_table[1], money_table[2]
		if money_type ~= nil and money ~= nil then
			UserInfo.AddUserMoneyByUid(self.owner.uid, money_type, money)
		end

		local res = {}
		res["do"] = "Cmd.BuildingRebuildCmd_S"
		res["data"] = {
			stateId = self.state.id,
			buildingId = self.id,
			resultCode = 0,
		}

		unilight.response(self.owner.laccount, res)
	end

end

function Building:rebuildSeeSceen()
	local row = TableRebuild.query(self.id, self.buildLv)

	if row == nil then
		return ERROR_CODE.BUILDING_REBUILD_MAX
	end

	local reward = row["RewardMoney"]

	local money_table = string.split(reward, "_")
	local money_type, money =  money_table[1], money_table[2]
	if money_type == nil or money == nil then
		unilight.error("配置表数据出错.........")
		return ERROR_CODE.TABLE_ERROR
	end

	UserInfo.AddUserMoneyByUid(self.owner.uid, money_type, money)

	return ERROR_CODE.SUCCESS
end

function Building:rebuild()
	local row = TableRebuild.query(self.id, self.buildLv + 1)

	if row == nil then
		return ERROR_CODE.BUILDING_REBUILD_MAX
	end

	local needLv = row["NeedLv"]

	if type(needLv) ~= "number" then
		unilight.info("Table[Rebuild]'s needLv is error")
		return ERROR_CODE.TABLE_ERROR
	end

	if self.lv < needLv then
		return ERROR_CODE.BUILDING_LEVEL_NOT_ENOUGH
	end

	local cost = self:getRebuildValue("CostMoney", true)
	if type(cost) ~= "number" then
		unilight.info("Table[Rebuild]'s CostMoney is error")
		return ERROR_CODE.TABLE_ERROR
	end

	UserInfo.SubUserMoneyByUid(self.owner.uid, static_const.Static_MoneyType_Gold, cost)

	local diamond = row["CostDiamond"]

	if type(diamond) ~= "number" then
		unilight.info("Table[Rebuild]'s CostDiamond is error")
		return ERROR_CODE.TABLE_ERROR
	end

	UserInfo.SubUserMoneyByUid(self.owner.uid, static_const.Static_MoneyType_Diamond, diamond)

	self.buildLv = self.buildLv + 1
	self:recalc()

	--任务系统，任务完成情况
	self.owner.achieveTask:addProgress(TaskConditionEnum.BuildingChangeEvent, 1)
	self.owner.dailyTask:addProgress(TaskConditionEnum.BuildingChangeEvent, 1)

	local friendInfo = FriendManager:GetFriendInfo(self.owner.uid)
	if friendInfo ~= nil then
		local friendvisitData = friendInfo:GetFriendVisit()
		if friendvisitData:GetCurMapId() == self.state.id then
			friendvisitData:SetBuildLevel(self.id, self.buildLv)
		end
	end

	local data = {}
	data.cmd_uid = self.owner.uid
	data.mapid = self.state.id
	data.buildid = self.id
	data.buildLv = self.buildLv
	unilobby.SendCmdToLobby("Cmd.BuildingReBuild_C", data)

--	local reward = row["RewardMoney"]

--	local money_table = string.split(reward, "_")
--	local money_type, money =  money_table[1], money_table[2]
--	if money_type == nil or money == nil then
--		unilight.error("配置表数据出错.........")
--		return ERROR_CODE.TABLE_ERROR
--	end

--	UserInfo.AddUserMoneyByUid(self.owner.uid, money_type, money)

	return ERROR_CODE.SUCCESS
end

function Building:GetBuildingRebuildReward()
	local row = FootPrint.query(self.id)

	if row == nil then
		return ERROR_CODE.TABLE_ERROR
	end

	local needLv = row["RebuildTimes"]

	if type(needLv) ~= "number" then
		unilight.info("Table[Rebuild]'s needLv is error")
		return ERROR_CODE.TABLE_ERROR
	end

	unilight.error("buildid:"..self.id.." buildlv:"..self.buildLv.." needLv:"..needLv)

	if self.buildLv ~= needLv then
		return ERROR_CODE.BUILDING_LEVEL_NOT_ENOUGH
	end

	local cost = row["Reward1"]

	local money_table = string.split(cost, "_")
	local money_type, money =  money_table[1], money_table[2]
	if money_type == nil or money == nil then
		unilight.error("配置表数据出错.........")
		return ERROR_CODE.TABLE_ERROR
	end

	if UserInfo.CheckUserMoneyByUid(self.owner.uid, money_type, money) == false then
		if money_type == static_const.Static_MoneyType_Diamond then
			return ERROR_CODE.MONEY_NOT_ENOUGH
		elseif money_type == static_const.Static_MoneyType_Gold then
			return ERROR_CODE.DIAMOND_NOT_ENOUGH
		end
	end

	UserInfo.SubUserMoneyByUid(self.owner.uid, money_type, money)

	self.isFootReward = true

	return ERROR_CODE.SUCCESS
end

