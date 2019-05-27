
SysCountService = {}
--该类是统计整个经济系统的



--向系统发放钻石
function SysCountService.IssueMoney(amount)
	
	local sqlCase = "update dy_syscount set issue=issue+"..amount.." where id=101"
	SqlServer.rpush(sqlCase)
end

--系统回收钻石，也就是玩家用钻石买东西
function SysCountService.RecycleMoney(amount)
	local sqlCase = "update dy_syscount set recycle=recycle+"..amount.." where id=101"
	SqlServer.rpush(sqlCase)
end

--向系统发放筹码，包括玩家购买，每日赠送
function SysCountService.IssueJetton(amount)
	local sqlCase = "update dy_syscount set issue=issue+"..amount.." where id=102"
	SqlServer.rpush(sqlCase)
end

--系统回收钻石，也就是玩家用钻石买东西
function SysCountService.RecycleJetton(amount)
	local sqlCase = "update dy_syscount set recycle=recycle+"..amount.." where id=102"
	SqlServer.rpush(sqlCase)
end

--玩家总数
function SysCountService.PlayerCount()
	local sqlCase = "update dy_syscount set count=count+1 where id=104"
	SqlServer.rpush(sqlCase)
end

--付费玩家总数
function SysCountService.PayPlayerCount()
	local sqlCase = "update dy_syscount set count=count+1 where id=105"
	SqlServer.rpush(sqlCase)
end

--付费次数
function SysCountService.PayTimeCount(amount)
	local sqlCase = "update dy_syscount set count=count+1 where id=106"
	SqlServer.rpush(sqlCase)
end

--付费总额
function SysCountService.PayRmbCount(amount)
	local sqlCase = "update dy_syscount set count=count+"..amount.." where id=107"
	SqlServer.rpush(sqlCase)
end

--掉起付费次数
function SysCountService.CallPayTimeCount()
	local sqlCase = "update dy_syscount set count=count+1 where id=109"
	SqlServer.rpush(sqlCase)
end

--调起付费总额
function SysCountService.CallPayRmbCount(amount)
	local sqlCase = "update dy_syscount set count=count+"..amount.." where id=110"
	SqlServer.rpush(sqlCase)
end

--登录总次数
function SysCountService.LoginTime()
	local sqlCase = "update dy_syscount set count=count+1 where id=111"
	SqlServer.rpush(sqlCase)
end