module('timerManager', package.seeall)


function timerManager.Init(currTime, timerType)
		
	g_markTime.curr = TimeUtils.GetTableTime()
	if timerType == "logtimer" then
	elseif timerType == "pokertimer" then
		--把这个线程空余出来，作为事件的触发器。
	elseif timerType == "gametimer" then
		UnlimitedAgentService.Init()
	elseif timerType == "utilstimer" then
		
	end
	g_markTime.last = g_markTime.curr
	print(timerType.." init end")
end


function timerManager:execute(currTime, timerType)
	
	if g_markTime.last == nil then
		g_markTime.last = TimeUtils.GetTableTime()
		print("================"..timerType)
		return
	end
	--每分钟更新一次服务器的数据	
	g_markTime.curr = TimeUtils.GetTableTime()
	if timerType == "logtimer" then

	elseif timerType == "gametimer" then
		--其他模块循环的线程，包括小马快跑，快乐牛牛，消息通知
		AgentService.ServiceLoop()
	elseif timerType == "utilstimer" then
		AgentService.UtilsLoop()
	end
	g_markTime.last = g_markTime.curr
end

function timerManager:createOnceTimer(strIndex)
	return _G[strIndex]
end


