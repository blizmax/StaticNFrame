ServerCheck = {}
ServerDiconnectCheck = {}

function ServerCheck.work(buffer)
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

		local buffer = {}
		buffer["serverid"] = serverid
		buffer["serverip"] = serverip
		buffer["tcpport"] = tcpport
		processWork( "CheckServerIpPort", luajson.encode(buffer))
	end
end

function ServerDiconnectCheck.work(buffer)
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
		
		local buffer = {}
		buffer["serverid"] = serverid
		buffer["serverip"] = serverip
		buffer["tcpport"] = tcpport
		processWork( "CheckDisconnect", luajson.encode(buffer))
	end
end

function ServerCheck.ServerLoop()
	local tm = TimeUtils.GetTableTime()
	
	if tm.sec % 10 == 0 then
		processWork( "ServerCheck", "1" )
    end

	if tm.sec % 10 == 0 then
		processWork( "ServerDiconnectCheck",  "2")
	end	
end

CheckServerIpPort = {}

function CheckServerIpPort.work(buffer)
	local serverinfo = luajson.decode(buffer)
	local serverip = serverinfo["serverip"]
	local tcpport = serverinfo["tcpport"]
	local serverid = serverinfo["serverid"]
	local socket = require("socket")
	local sock = socket.tcp()
	sock:settimeout(1)
	local client = sock:connect(serverip, tcpport)
	if client ~= nil then
		sock:close()
		--LogFile("error", serverip.." connect success")
	else
		LogFile("error", serverip.." connect timeout, may be attacted.......")
		local count = 0
		for i=1,5 do
			local sock = socket.tcp()
			sock:settimeout(1)
			local client = sock:connect(serverip, tcpport)
			if client == nil then
				count = count + 1
				LogFile("error", serverip.." connect timeout, count:"..count.." may be attacted.......")
			else
				sock:close()
			end
		end

		if count >= 5 then
			LogFile("error", serverip.." connect timeout, must be attacted.......")
			HttpGet("http://kxqp2006.top/index.php/dwcgame/client/serverAttattedPhone?phonenum=18927427460&serverip="..serverip)

			local sqlCase = "update du_server set state = 0 where serverid = "..serverid
			mysqlItem:execute(sqlCase)
		end
	end
end

CheckDisconnect = {}
function CheckDisconnect.work(buffer)
	local serverinfo = luajson.decode(buffer)
	local serverip = serverinfo["serverip"]
	local tcpport = serverinfo["tcpport"]
	local serverid = serverinfo["serverid"]
	local socket = require("socket")

	local sock = socket.tcp()
	sock:settimeout(1)
	local client, errorStr = sock:connect(serverip, tcpport)
	if client ~= nil then
		sock:close()
		LogFile("error", serverip.." state = 0, but connect success")
		local count = 0
		for i=1,5 do
			local sock = socket.tcp()
			sock:settimeout(1)
			local client, errorStr = sock:connect(serverip, tcpport)
			if client ~= nil then
				count = count + 1
				sock:close()
				LogFile("error", serverip.." state = 0, count:"..count.." but connect success")
			end
		end

		if count >= 5 then
			LogFile("error", serverip.." set state = 1")
			HttpGet("http://kxqp2006.top/index.php/dwcgame/client/serverReConnectPhone?phonenum=18927427460&serverip="..serverip)
			local sqlCase = "update du_server set state = 1 where serverid = "..serverid
			mysqlItem:execute(sqlCase)
		end
	else
		--LogFile("error", "serverid:"..serverid.." serverip:"..serverip.." disconnect")
	end
end

