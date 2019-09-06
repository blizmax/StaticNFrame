
SqlServer = {}
SqlServer.redis_index = "redis_sqlserver"
function SqlServer.rpush(sqlCase)
	
	--mysqlItem:execute(sqlCase)
	if sqlCase == nil then
		return nil
	end
	--luaPrint(sqlCase)
	--redisItem:rpush("sqlservice", sqlCase, SqlServer.redis_index)
	LogModel.GameSqlServer(sqlCase)
end

function SqlServer.Execute()
	local len = redisItem:llen("sqlservice", SqlServer.redis_index)
	for i = 1, len do
		local strGet = redisItem:lpop("sqlservice", SqlServer.redis_index)
		
		if strGet ~= nil then
			mysqlItem:execute(strGet)
		end
	end
	
	--SqlServer.RaceLogExectute()
	--SqlServer.ChatLogExectute()
end



