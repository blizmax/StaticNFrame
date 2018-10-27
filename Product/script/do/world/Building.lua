Building = {
	owner = nil,
	state = nil,
	id = 0,
	lv = 1,
	buildLv = 1,
	lost = 0,
	produce = 0,
}

function Building:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function Building:init(owner, state, id, lv, buildLv)
	self.owner = owner
	self.state = state
	self.id = id
	self.lv = lv
	self.buildLv = buildLv

	if self.buildLv >= 100 then
		self.buildLv = 99
	end

	self:recalc()

	table.insert(state.buildings, self.id, self)
end

function Building:recalc()
	--local words = "recalc building: {id: %d, lv: %d, buildLv: %d}"
	--unilight.debug(words.format(words, self.id, self.lv, self.buildLv))
	self.produce = self:getValue("ProduceMoney") * self:getRebuildValue("Times")
	--local words = "recalc building: {id: %d, lv: %d, buildLv: %d, produce:%d}"
	--unilight.debug(words.format(words, self.id, self.lv, self.buildLv, self.produce))
end

-- Get value from Levelup table
function Building:getValue(key, isNext)
	if isNext == nil or isNext == false then
		return TableLevelup.query(self.id, self.lv)[key]
	else
		return TableLevelup.query(self.id, self.lv + 1)[key]
	end
end

function Building:getRebuildValue(key, isNext)
	if isNext == nil or isNext == false then
		return TableRebuild.query(self.id, self.buildLv)[key]
	else
		return TableRebuild.query(self.id, self.buildLv + 1)[key]
	end
end

function Building:sn()
	local data = {
		id = self.id,
		lv = self.lv,
		buildLv = self.buildLv,
	}
	return data
end

function Building:earn()
	return self.produce
end

function Building:levelup()
	local cost = self:getValue("CostMoney", true)

	if (cost == nil) then
		return ERROR_CODE.BUILDING_LEVEL_MAX
	end

	if type(cost) ~= "number" then
		unilight.warn("Table[Levelup]'s CostMoney is error")
		return ERROR_CODE.TABLE_ERROR
	end

	if self.owner.money < cost then
		return ERROR_CODE.MONEY_NOT_ENOUGH
	end

	self.owner.money = self.owner.money - cost
	self.lv = self.lv + 1
	self:recalc()
	self.owner.star = self.owner.star + 1

	self.owner.mailMgr:addNew(2)

	return ERROR_CODE.SUCCESS
end

function Building:rebuild()
	local row = TableRebuild.query(self.id, self.buildLv + 1)

	if row == nil then
		return ERROR_CODE.BUILDING_REBUILD_MAX
	end

	local needLv = row["NeedLv"]

	if type(needLv) ~= "number" then
		unilight.warn("Table[Rebuild]'s needLv is error")
		return ERROR_CODE.TABLE_ERROR
	end

	if self.lv < needLv then
		return ERROR_CODE.BUILDING_LEVEL_NOT_ENOUGH
	end

	local cost = row["CostMoney"]

	if type(cost) ~= "number" then
		unilight.warn("Table[Rebuild]'s CostMoney is error")
		return ERROR_CODE.TABLE_ERROR
	end

	if (cost ~= 0) then
		if self.owner.money < cost  then
			return ERROR_CODE.MONEY_NOT_ENOUGH
		end
	
		self.owner.money = self.owner.money - cost
	end

	local diamond = row["CostDiamond"]

	if type(diamond) ~= "number" then
		unilight.warn("Table[Rebuild]'s CostDiamond is error")
		return ERROR_CODE.TABLE_ERROR
	end

	if diamond ~= 0 then
		if self.owner.diamond < cost  then
			return ERROR_CODE.DIAMOND_NOT_ENOUGH
		end
	
		self.owner.diamond = self.owner.diamond - diamond
	end

	self.buildLv = self.buildLv + 1
	self:recalc()

	self.owner.mailMgr:addNew(3)

	return ERROR_CODE.SUCCESS
end
