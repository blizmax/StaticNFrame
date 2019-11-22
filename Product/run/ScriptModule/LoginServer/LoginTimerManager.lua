module('timerManager', package.seeall)


function timerManager.Init(currTime, timerType)
		
	g_markTime.curr = TimeUtils.GetTableTime()

	g_markTime.last = g_markTime.curr
	print(timerType.." init end")
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

	g_markTime.last = g_markTime.curr
	--LogFile("info",   "LuaNFrame.UpdateSec")
end

function timerManager.UpdateMin()
	LogFile("info",   "LuaNFrame.UpdateMin")
end

function timerManager.Update5Min()
	LogFile("info",   "LuaNFrame.Update5Min")
end

function timerManager.Update10Min()
	LogFile("info",   "LuaNFrame.Update10Min")
end

function timerManager.Update30Min()
	LogFile("info",   "LuaNFrame.Update30Min")
end

function timerManager.UpdateHour()
	LogFile("info",   "LuaNFrame.UpdateHour")
end

function timerManager.UpdateDay()
	LogFile("info",   "LuaNFrame.UpdateDay")
end

function timerManager.UpdateWeek()
	LogFile("info",   "LuaNFrame.UpdateWeek")
end

function timerManager.UpdateMonth()
	LogFile("info",   "LuaNFrame.UpdateMonth")
end


