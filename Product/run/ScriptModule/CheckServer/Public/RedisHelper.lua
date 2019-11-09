
RedisHelper = {}


function RedisHelper.CheckRedis(userid, operatorid)
	local key = userid.."_"..operatorid
	return redisItem:get("operatorlist"..key)
end

function RedisHelper.SetRedis(playerid, operatorid, msg)
	if playerid == nil or tonumber(playerid) == 0 then
		return nil
	end
	local key = playerid.."_"..operatorid
	redisItem:setex("operatorlist"..key, 6, msg)
end



