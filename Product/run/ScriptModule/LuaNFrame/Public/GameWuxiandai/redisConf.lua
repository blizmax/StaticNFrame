
redisConf = {}


function redisConf:getRedisIndex(p_index)

	if g_redisIndex[p_index] == nil then
		return g_redisIndex["default"].index
	else
		return g_redisIndex[p_index].index
	end
end

function redisConf:getRedisKey(p_index, p_key)

	if g_redisIndex[p_index] ~= nil and g_redisIndex[p_index].link == 1 then
		local t_key = g_redisIndex[p_index].key
		return t_key..p_key
	else
		return p_key
	end
	
end

return redisConf
