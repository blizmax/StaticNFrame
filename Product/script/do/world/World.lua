World =
{
	owner = nil,
	states = nil,
}

WORLD_CLICK_FACTOR = 0.04

function World:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function World:init(owner)
	self.owner = owner
	self.states = {}
end

function World:create()
	--At the beginning, there is only the first state with activated
	local state = State:new()
	state:init(self.owner, self, 101)
end

function World:createTemp()
	unilight.debug("Create a temp world")
	local state1 = State:new()
	state1:init(self.owner, self, 101)

	local building1 = Building:new()
	building1:init(self.owner, state1, 1, 2, 2)

	local building2 = Building:new()
	building2:init(self.owner, state1, 2, 2, 1)
	
	local state2 = State:new()
	state2:init(self.owner, self, 102)

	local building3 = Building:new()
	building3:init(self.owner, state2, 7, 1, 1)
end

function World:update()
	self.owner.money = self.owner.money + self:earn()
	--unilight.debug("UID:" .. self.owner.uid .."Money:" .. self.owner.money)
end

--In normal case, it is unused
function World:recalcStar()
	local star = 0

	for i, state in pairs(self.states) do
		star = star + state:recalcStar()
	end

	unilight.debug("Recalc star:" .. star)
	return star
end

function World:sn()
	local data = {
		states = {},
	}

	local c = 0

	for i, state in pairs(self.states) do
		c = c + 1
	end

	local last = 101 + c - 1

	for i = 101, last do
		table.insert(data.states, self.states[i]:sn())
	end

	return data
end

function World:loadFromDb(data)
	if data["states"] == nil then
		unilight.warn("No states")
		return false
	end

	for i, db_state in pairs(data.states) do
		local state = State:new()
		state:init(self.owner, self, db_state.id)
		
		if state:loadFromDb(db_state) == false then
			return false
		end
	end

	return true
end

	
function World:levelup(stateId, buildingId)
	if self.states[stateId] == nil then
		return ERROR_CODE.BUILDING_STATE_NOT_OPEN
	end

	return self.states[stateId]:levelup(buildingId)
end

function World:buy(stateId, buildingId)
	if self.states[stateId] == nil then
		return ERROR_CODE.BUILDING_STATE_NOT_OPEN
	end

	return self.states[stateId]:buy(buildingId)
end

function World:openState(id) -- state ID
	if self.states[id] ~= nil then
		return ERROR_CODE.OPEATE_AGAIN
	end

	--The first building of this state
	local buildingId = (id % 100) * 4 + 1
	local costString = TableBuilding[buildingId]["OpenCost"]
	local cost = tonumber(string.match(string.match(costString, ",%d+"), "%d+"))

	if self.owner.money < cost then
		return ERROR_CODE.MONEY_NOT_ENOUGH
	end

	local state = State:new()
	state:init(self.owner, self, id)

	return ERROR_CODE.SUCCESS
end

function World:rebuild(stateId, buildingId)
	if self.states[stateId] == nil then
		return ERROR_CODE.BUILDING_STATE_NOT_OPEN
	end

	return self.states[stateId]:rebuild(buildingId)
end

function World:earn()
	local money = 0

	for k,state in pairs(self.states) do
		money = money + state:calcEarning()
	end

	return money
end

function World:click(stateId, critical)
	if self.states[stateId] == nil then
		return ERROR_CODE.BUILDING_STATE_NOT_OPEN
	end

	local earning = self.states[stateId]:calcEarning()

	if type(critical) == "number" and critical ~= 0 then
		earning = earning * 10
	end

	earning = earning * 0.04

	self.owner.money = self.owner.money + earning

	return 0
end
