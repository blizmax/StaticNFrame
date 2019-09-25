
LocationModel = {}
  
LocationModel.redis_index = "redis_location"

LocationModel.location_lock = "location"


LocationModel.location_list = "location_list"   --存放需要查询的Location


function LocationModel.GetListLen()
	local len = redisItem:llen(LocationModel.location_list, LocationModel.redis_index)
	return len == nil and 0 or tonumber(len)
end


function LocationModel.PushList(itemInfo)
	redisItem:lpush( LocationModel.location_list, luajson.encode( itemInfo ),  LocationModel.redis_index)
end


