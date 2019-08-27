--lua5.3 没有math.mod函数
function math.mod(a, b)
	return math.fmod(a,b)
end

g_platfrom = 'win32'           --g_platfrom init for luaself

function md5(md5Str)
	return LuaNFrame.GetMD5(md5Str)
end

g_randomsee = 0
function math.myrandom(...)
	local arg = {...}
	if g_randomsee == 0 then
		math.newrandomseed()
		g_randomsee = 1
	end
	for i = 1, #arg do
		arg[i] = math.floor(arg[i])
	end

	return math.random(table.unpack(arg))
end

function LogFile(file, msg)
	local cStackInfo = debug.getinfo(2, "Sl")
	if cStackInfo then
		CPPNFrame:Info(NFLogId.NF_LOG_SYSTEMLOG, 0, "["..tostring(cStackInfo.short_src)..":"..tostring(cStackInfo.currentline).."] | "..tostring(msg))
	else
		CPPNFrame:Info(NFLogId.NF_LOG_SYSTEMLOG, 0, tostring(msg))
	end
end

--入参是两个参数，第一个是索引，第一个是参数信息
function processWork(luaFunc, dataStr)
	LuaNFrame.ProcessWork(luaFunc, dataStr)
end

function processTimer(timeSec, indexStr, dataStr)
	LuaNFrame.ProcessTimer(timeSec, indexStr, dataStr)
end

function processLoopTimer(timeSec, indexStr, dataStr)
	--该函数设置的定时器，是在主线程serverloop的线程中执行
	LuaNFrame.ProcessLoopTimer(timeSec, indexStr, dataStr)
end

function TcpSessionClose(playerID)
	if tonumber(playerID) == 0 then
		return
	end
	luaPrint(playerID.." Exit")
	OnlineModel.PlayerExit(playerID)
end

function TcpSessionReport(userID, IP)
	--上报玩家的IP，目前主要是IP	
	PlayerModel.PlayerReport(userID, IP)
end

function SendMessage(useridList, retMsgID, buffLen, retString)

	if useridList == nil or buffLen == 0  then
		return 
	end

	local userIDStr = ""
	if type(useridList) == "table" then
		if #useridList == 0 then
			return
		end
		
		for key,values in pairs(useridList) do
			LuaNFrame.SendMsgToPlayer(0,  tonumber(values), retMsgID, buffLen, retString)	
		end
	else
		LuaNFrame.SendMsgToPlayer(0, tonumber(useridList), retMsgID, buffLen, retString)
	end
end