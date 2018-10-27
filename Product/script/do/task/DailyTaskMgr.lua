DailyTaskMgr = 
{
	owner = nil,
	activity = 0,
	tasks = nil,
}

function DailyTaskMgr:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function DailyTaskMgr:init(owner)
	self.owner = owner
	self.tasks = {}
end

function DailyTaskMgr:createTemp()
	self.activity = 10

	local task1 = Task:new()
	task1.init(1001, 1, TaskStatusEnum.Receive)

	local task2 = Task:new()
	task2.init(1002, 1, TaskStatusEnum.Begin)
end

--When 0:00, call
function DailyTaskMgr:reset()
	self.activity = 0
	self.tasks = {}
end

function DailyTaskMgr:addProgress(cond, times)

end
