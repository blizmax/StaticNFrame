module('timerManager', package.seeall)


function timerManager.Init(currTime, timerType)
	g_markTime.curr = TimeUtils.GetTableTime()
end


function timerManager:execute(currTime, timerType)
	--每分钟更新一次服务器的数据	
	g_markTime.curr = TimeUtils.GetTableTime()
	ServerCheck.ServerLoop()
	g_markTime.last = g_markTime.curr
end

function timerManager:createOnceTimer(strIndex)
	return _G[strIndex]
end