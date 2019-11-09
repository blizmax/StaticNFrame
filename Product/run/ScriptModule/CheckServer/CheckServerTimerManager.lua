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

ServerCheck = {}

function ServerCheck.ServerLoop()
	local tm = TimeUtils.GetTableTime()
	
	if math.mod(tm.sec, 10) == 0 then
		ServerCheck.CheckServerIpPort()
	end
end

function ServerCheck.CheckServerIpPort()
	local socket = require("socket")
	local servertimeout = {}
	local sqlCase = "select serverid,servername,serverip,tcpport from du_server where state = 1"
	mysqlItem:executeQuery(sqlCase)
	while true do
		
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end 
		local serverid = tonumber(sqlData[1]) == nil and 0 or tonumber(sqlData[1])
		local servername = tostring(sqlData[2]) == nil and "" or tostring(sqlData[2])
		local serverip = tostring(sqlData[3]) == nil and "" or tostring(sqlData[3])
		local tcpport = tonumber(sqlData[4]) == nil and 0 or tonumber(sqlData[4])

		
		local sock = socket.connect(serverip, tcpport)
		if sock ~= nil then
			sock:settimeout(0)
			sock:close()
			--LogFile("error", serverip.." connect success")
		else
			LogFile("error", serverip.." connect timeout, may be attacted.......")
			local count = 0
			for i=1,5 do
				local sock2 = socket.connect(serverip, tcpport)
				if sock2 == nil then
					count = count + 1
					LogFile("error", serverip.." connect timeout, count:"..count.." may be attacted.......")
				else
					sock2:settimeout(0)
					sock2:close()
				end
			end

			if count >= 5 then
				LogFile("error", serverip.." connect timeout, must be attacted.......")
				table.insert(servertimeout, serverid)
			end
		end
	end

	local serversuccess = {}
	local sqlCase = "select serverid,servername,serverip,tcpport from du_server where state = 0"
	mysqlItem:executeQuery(sqlCase)
	while true do
		
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end 
		local serverid = tonumber(sqlData[1]) == nil and 0 or tonumber(sqlData[1])
		local servername = tostring(sqlData[2]) == nil and "" or tostring(sqlData[2])
		local serverip = tostring(sqlData[3]) == nil and "" or tostring(sqlData[3])
		local tcpport = tonumber(sqlData[4]) == nil and 0 or tonumber(sqlData[4])

		local sock = socket.connect(serverip, tcpport)
		if sock ~= nil then
			sock:settimeout(0)
			sock:close()
			LogFile("error", serverip.." state = 0, but connect success")
			local count = 0
			for i=1,5 do
				local sock2 = socket.connect(serverip, tcpport)
				if sock2 ~= nil then
					count = count + 1
					sock2:settimeout(0)
					sock2:close()
					LogFile("error", serverip.." state = 0, count:"..count.." but connect success")
				end
			end

			if count >= 5 then
				table.insert(serversuccess, serverid)
				LogFile("error", serverip.." set state = 1")
			end
		end
	end

	for k, serverid in ipairs(servertimeout) do
		local sqlCase = "update du_server set state = 0 where serverid = "..serverid
		mysqlItem:execute(sqlCase)
	end

	for k, serverid in ipairs(serversuccess) do
		local sqlCase = "update du_server set state = 1 where serverid = "..serverid
		mysqlItem:execute(sqlCase)
	end
end

