
SqlServer = {}
SqlServer.redis_index = "redis_sqlserver"
function SqlServer.rpush(sqlCase)
	
	--mysqlItem:execute(sqlCase)
	if sqlCase == nil then
		return nil
	end
	redisItem:rpush("sqlservice", sqlCase, SqlServer.redis_index)
end

function SqlServer.Execute()
	local len = redisItem:llen("sqlservice", SqlServer.redis_index)
	for i = 1, len do
		local strGet = redisItem:lpop("sqlservice", SqlServer.redis_index)
		
		if strGet ~= nil then
			mysqlItem:execute(strGet)
		end
	end
	
	SqlServer.RaceLogExectute()
	SqlServer.ChatLogExectute()
end

function SqlServer.RaceLogRpush(logkey, logmsg)

	redisItem:rpush(logkey, logmsg, SqlServer.redis_index)
	
end

function SqlServer.RaceLogExectute()

end


function SqlServer.ChatRpush(msg)
	redisItem:rpush("sqlchatlog", msg, SqlServer.redis_index)
end

function SqlServer.ChatLogExectute()
	local chatList = {}
	local len = redisItem:llen("sqlchatlog", SqlServer.redis_index)
	for i = 1, len do
		local strGet = redisItem:lpop("sqlchatlog", SqlServer.redis_index)
		local cInfo = proplayer_pb.chatinfo()
		cInfo:ParseFromString(strGet)
		local key = cInfo.senderid < cInfo.receiverid and cInfo.senderid.."v"..cInfo.receiverid or cInfo.receiverid.."v"..cInfo.senderid
		if chatList[key] == nil then
			chatList[key] = {}
		end
		table.insert(chatList[key], cInfo)
	end

	for k1,v1 in pairs(chatList) do
		local fp = io.open("./chat/"..k1..".log", "a")
		if fp ~= nil then
			
			for k2,v2 in ipairs(v1) do
				local content = v2.chatdate..":["..v2.sender.." TO "..v2.receiver.."]"..v2.msg.."\n"
				fp:write(content)
			end
			fp:close()
		end
	end
end

