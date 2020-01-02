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

function timerManager.UpdateSec()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.UpdateSec")
end

function timerManager.UpdateMin()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.UpdateMin")
end

function timerManager.Update5Min()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.Update5Min")
end

function timerManager.Update10Min()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.Update10Min")
end

function timerManager.Update30Min()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.Update30Min")
end

function timerManager.UpdateHour()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.UpdateHour")
end

function timerManager.UpdateDay()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.UpdateDay")
end

function timerManager.UpdateWeek()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.UpdateWeek")
end

function timerManager.UpdateMonth()
	LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0,  "LuaNFrame.UpdateMonth")
end