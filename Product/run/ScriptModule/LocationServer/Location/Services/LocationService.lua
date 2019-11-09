
--定位服务器

LocationService = {}


function LocationService.Init()
	--定位服务器的初始化。
	
end


function LocationService.ServerLoop()
	ServerCheck.ServerLoop()
	if false == ThreadManager.IsLock(LocationModel.location_lock) and LocationModel.GetListLen() > 0 then
		processWork( "CheckIPLocation", "location" )
	end	
end

ServerCheck = {}

function ServerCheck.ServerLoop()
	local tm = TimeUtils.GetTableTime()
	
	if tm.sec == 0 then
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
			LogFile("error", serverip.." connect success")
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
			local count = 0
			for i=1,5 do
				local sock2 = socket.connect(serverip, tcpport)
				if sock2 ~= nil then
					count = count + 1
					sock2:settimeout(0)
					sock2:close()
				end
			end

			if count >= 5 then
				table.insert(serversuccess, serverid)
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



