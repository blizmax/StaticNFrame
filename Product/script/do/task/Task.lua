TaskCondition = 
{
	"Login",							--登录
	"BuildingLevelup",					--升级建筑
	"ClickTourGroup",					--点击旅游团
	"Hire",								--雇佣好友
	"Catch",							--成功抓捕好友
	"AddFriend",						--主动申请添加其他玩家为好友
	"InviteFriend",						--成功向好友发送邀请
	"ShareGame",						--成功分享游戏
	"GroupLevelup",						--团长等级提升
}

TaskStatus =
{
	"Begin",							--所有的任务都是自动开启，也没有开启任务的条件
	"Finish",							--任务已完成，但未领取奖励
	"Receive",							--奖励已领取
}

TaskStatusEnum = enum.create(TaskStatus)
TaskConditionEnum = enum.create(TaskCondition)

TaskStatus = nil
TaskCondition = nil

Task = 
{
	id = 0,
	times = 0,
	status = TaskStatusEnum.Begin,
}

function Task:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function Task:init(id, times, status)
	self.id = id
	self.times = times
	self.status = status
end

function Task:addProgress(times)
	times = times or 1

	if times <= 0 then
		return
	end

	self.times = self.times + times
end
