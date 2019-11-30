module('timerManager', package.seeall)


function timerManager.Init()
		
	g_markTime.curr = TimeUtils.GetTableTime()

	LocationService.Init()

	g_markTime.last = g_markTime.curr
	print(" init end")
end


function timerManager:createOnceTimer(strIndex)
	return _G[strIndex]
end

function timerManager.UpdateSec()
	if g_markTime.last == nil then
		g_markTime.last = TimeUtils.GetTableTime()
		return
	end
	--每分钟更新一次服务器的数据	
	g_markTime.curr = TimeUtils.GetTableTime()

	LocationService.ServerLoop()

	g_markTime.last = g_markTime.curr
	--LogFile("info",   "timerManager.UpdateSec")
end

function timerManager.UpdateMin()
	LogFile("info",   "timerManager.UpdateMin")
end

function timerManager.Update5Min()
	LogFile("info",   "timerManager.Update5Min")
end

function timerManager.Update10Min()
	LogFile("info",   "timerManager.Update10Min")
end

function timerManager.Update30Min()
	LogFile("info",   "timerManager.Update30Min")
end

function timerManager.UpdateHour()
	LogFile("info",   "timerManager.UpdateHour")
end

function timerManager.UpdateDay()
	LogFile("info",   "timerManager.UpdateDay")
end

function timerManager.UpdateWeek()
	LogFile("info",   "timerManager.UpdateWeek")
end

function timerManager.UpdateMonth()
	LogFile("info",   "timerManager.UpdateMonth")
end

